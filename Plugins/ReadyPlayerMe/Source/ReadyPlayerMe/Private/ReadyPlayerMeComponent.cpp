// Copyright © 2021++ Ready Player Me

#include "ReadyPlayerMeComponent.h"
#include "ReadyPlayerMeAvatarLoader.h"
#include "ReadyPlayerMeRenderLoader.h"
#include "Utils/ReadyPlayerMeGlTFConfigCreator.h"
#include "Utils/ReadyPlayerMeUrlConvertor.h"
#include "Utils/ReadyPlayerMeMetadataExtractor.h"
#include "glTFRuntimeAsset.h"
#include "UObject/UObjectGlobals.h"
#include "Components/SkeletalMeshComponent.h"
#include "Media/Public/IMediaTracks.h"

UReadyPlayerMeComponent::UReadyPlayerMeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	OnAvatarLoadCompleted.BindDynamic(this, &UReadyPlayerMeComponent::OnAvatarLoaded);
	OnSkeletalMeshCallback.BindDynamic(this, &UReadyPlayerMeComponent::OnSkeletalMeshLoaded);
}

void UReadyPlayerMeComponent::LoadAvatar(const FAvatarLoadFailed& OnLoadFailed)
{
	if (!TargetSkeleton)
	{
		(void)OnLoadFailed.ExecuteIfBound("No target skeleton set");
		return;
	}
	const bool bShouldLoadMetadata = FReadyPlayerMeUrlConvertor::IsUrl(UrlShortcode);

	UrlShortcode = FReadyPlayerMeUrlConvertor::GetValidatedUrlShortCode(UrlShortcode);

	if (UrlShortcode.IsEmpty())
	{
		(void)OnLoadFailed.ExecuteIfBound("Url invalid");
		return;
	}

	AvatarLoader = NewObject<UReadyPlayerMeAvatarLoader>(this,TEXT("AvatarLoader"));
	AvatarLoader->LoadAvatar(UrlShortcode, OnAvatarLoadCompleted, OnLoadFailed, bShouldLoadMetadata);
}

void UReadyPlayerMeComponent::LoadNewAvatar(const FString& Url, const FAvatarLoadFailed& OnLoadFailed)
{
	UrlShortcode = Url;
	LoadAvatar(OnLoadFailed);
}

void UReadyPlayerMeComponent::OnAvatarLoaded(UglTFRuntimeAsset* Asset, const FAvatarMetadata& Metadata)
{
	AvatarMetadata = Metadata;
	LoadSkeletalMesh(Asset);
}

void UReadyPlayerMeComponent::LoadSkeletalMesh(UglTFRuntimeAsset* Asset)
{
	if (AvatarMetadata.BodyType == EAvatarBodyType::Undefined)
	{
		AvatarMetadata.BodyType = FReadyPlayerMeMetadataExtractor::GetBodyTypeFromAsset(Asset);
	}
	const FString RootBoneName = FReadyPlayerMeMetadataExtractor::GetRootBoneName(AvatarMetadata.BodyType);
	Asset->LoadSkeletalMeshRecursiveAsync(RootBoneName, {}, OnSkeletalMeshCallback, FReadyPlayerMeGlTFConfigCreator::GetGlTFRuntimeSkeletalMeshConfig(RootBoneName, TargetSkeleton));
}

void UReadyPlayerMeComponent::OnSkeletalMeshLoaded(USkeletalMesh* SkeletalMesh)
{
	InitSkeletalMeshComponent();
	SkeletalMeshComponent->SetSkeletalMesh(SkeletalMesh);
}

void UReadyPlayerMeComponent::InitSkeletalMeshComponent()
{
	AActor* ThisActor = GetOwner();
	SkeletalMeshComponent = ThisActor->FindComponentByClass<USkeletalMeshComponent>();
	if (!SkeletalMeshComponent)
	{
		SkeletalMeshComponent = NewObject<USkeletalMeshComponent>(ThisActor, TEXT("SkeletalMesh"));
		SkeletalMeshComponent->SetupAttachment(ThisActor->GetRootComponent());
		SkeletalMeshComponent->RegisterComponent();
	}
}

void UReadyPlayerMeComponent::LoadRender(const ERenderSceneType& SceneType, const FDownloadImageCompleted& OnCompleted, const FDownloadImageFailed& OnFailed)
{
	UrlShortcode = FReadyPlayerMeUrlConvertor::GetValidatedUrlShortCode(UrlShortcode);
	if (UrlShortcode.IsEmpty())
	{
		(void)OnFailed.ExecuteIfBound("Url invalid");
		return;
	}
	RenderLoader = NewObject<UReadyPlayerMeRenderLoader>(this,TEXT("RenderLoader"));
	RenderLoader->Load(UrlShortcode, SceneType, AvatarMetadata.OutfitGender, OnCompleted, OnFailed);
}
