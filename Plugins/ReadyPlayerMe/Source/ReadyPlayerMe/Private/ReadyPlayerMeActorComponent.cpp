// Copyright © 2021++ Ready Player Me

#include "ReadyPlayerMeActorComponent.h"
#include "ReadyPlayerMeFunctionLibrary.h"
#include "glTFRuntimeAsset.h"
#include "Serialization/JsonSerializerMacros.h"
//required for Android builds
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimBlueprint.h"

UReadyPlayerMeActorComponent::UReadyPlayerMeActorComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UReadyPlayerMeActorComponent::BeginPlay()
{
	Super::BeginPlay();
	AActor* thisActor = this->GetOwner();
	skeletalMeshComponent = thisActor->FindComponentByClass<USkeletalMeshComponent>();
	if (!skeletalMeshComponent) {
		skeletalMeshComponent = NewObject<USkeletalMeshComponent>(thisActor, UReadyPlayerMeFunctionLibrary::SKELETAL_MESH_NAME);
		skeletalMeshComponent->SetupAttachment(thisActor->GetRootComponent());
		skeletalMeshComponent->RegisterComponent();
	}
	avatarMetadataHttpResponse.BindUFunction(this, "OnMetadataReceived");
	onGltfRuntimeHttpResponse.BindUFunction(this, "OnAssetLoaded");
	onSkeletalMeshCallback.BindUFunction(this, "OnSkeletalMeshLoaded");
}

void UReadyPlayerMeActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UReadyPlayerMeActorComponent::RunAvatarSetup(const FAvatarLoadFailed& onSetupFailed)
{
	if (!targetSkeleton)
	{
		onSetupFailed.ExecuteIfBound("No target skeleton set");
		return;
	}

	const bool isShortCode = !UReadyPlayerMeFunctionLibrary::IsUrl(UrlShortcode);
	UrlShortcode = UReadyPlayerMeFunctionLibrary::GetValidatedUrlShortCode(UrlShortcode);
	if (UrlShortcode.IsEmpty()) {

		onSetupFailed.ExecuteIfBound("Url invalid");
		return;
	}

	if(isShortCode)
	{
		OnMetadataReceived(avatarMetadata);
		return;
	}

	UReadyPlayerMeFunctionLibrary::LoadAvatarMetadata(UrlShortcode, avatarMetadataHttpResponse, onSetupFailed);
}

void UReadyPlayerMeActorComponent::LoadNewAvatar(FString url,  const FAvatarLoadFailed& onLoadFailed)
{
	UrlShortcode = url;

	RunAvatarSetup(onLoadFailed);
}

void UReadyPlayerMeActorComponent::OnMetadataReceived(const FAvatarMetadata& metadata)
{
	avatarMetadata = metadata;
	LoadAssetFromUrl();
}

void UReadyPlayerMeActorComponent::LoadAssetFromUrl()
{
	FJsonSerializableKeyValueMap headers;
	UglTFRuntimeFunctionLibrary::glTFLoadAssetFromUrl(UrlShortcode, headers, onGltfRuntimeHttpResponse, UReadyPlayerMeFunctionLibrary::GetglTFRuntimeConfig());
}

void UReadyPlayerMeActorComponent::OnAssetLoaded(UglTFRuntimeAsset* asset)
{
	LoadSkeletalMesh(asset);
}

void UReadyPlayerMeActorComponent::LoadSkeletalMesh(UglTFRuntimeAsset* asset)
{

	if(avatarMetadata.bodyType == Undefined)
	{
		avatarMetadata.bodyType = UReadyPlayerMeFunctionLibrary::GetBodyTypeFromAsset(asset);
	}
	FJsonSerializableArray excludeNodes;
	asset->LoadSkeletalMeshRecursiveAsync(UReadyPlayerMeFunctionLibrary::GetRootBoneName(avatarMetadata.bodyType), excludeNodes, onSkeletalMeshCallback, UReadyPlayerMeFunctionLibrary::GetglTFRuntimeSkeletalMeshConfig(avatarMetadata, targetSkeleton));
}

void UReadyPlayerMeActorComponent::OnSkeletalMeshLoaded(USkeletalMesh* skeletalMesh)
{
	skeletalMeshComponent = UReadyPlayerMeFunctionLibrary::SetupSkeletalMeshComponent(this->GetOwner(), skeletalMesh, avatarMetadata);
}
