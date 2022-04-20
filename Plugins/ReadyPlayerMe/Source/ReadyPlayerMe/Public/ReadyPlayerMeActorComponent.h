// Copyright © 2021++ Ready Player Me

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ReadyPlayerMeTypes.h"
#include "glTFRuntimeFunctionLibrary.h"
#include "ReadyPlayerMeActorComponent.generated.h"

class UAnimBlueprint;
class USkeleton;
class UglTFRuntimeAsset;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class READYPLAYERME_API UReadyPlayerMeActorComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:
	UReadyPlayerMeActorComponent();

protected:
	virtual void BeginPlay() override;

	FAvatarMetadata avatarMetadata;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, Category = "Ready Player Me")
	FString UrlShortcode;

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Run Avatar Setup", AutoCreateRefTerm = "onSetupFailed"), Category = "Ready Player Me")
	void RunAvatarSetup(const FAvatarLoadFailed& onSetupFailed);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Load New Avatar", AutoCreateRefTerm = "onLoadFailed"), Category = "Ready Player Me")
	void LoadNewAvatar(FString url, const FAvatarLoadFailed& onLoadFailed);

	FAvatarMetadataHttpResponse avatarMetadataHttpResponse;

	FglTFRuntimeHttpResponse onGltfRuntimeHttpResponse;

	FglTFRuntimeSkeletalMeshAsync onSkeletalMeshCallback;

	UPROPERTY(EditAnywhere, Category="Ready Player Me")
	USkeleton* targetSkeleton;

	class USkeletalMeshComponent* skeletalMeshComponent;

private:
	
	UFUNCTION()
	void OnMetadataReceived(const FAvatarMetadata& metadata);

	void LoadAssetFromUrl();

	UFUNCTION()
	void OnAssetLoaded(UglTFRuntimeAsset* asset);

	void LoadSkeletalMesh(UglTFRuntimeAsset* asset);

	UFUNCTION()
	void OnSkeletalMeshLoaded(USkeletalMesh* skeletalMesh);
	
};
