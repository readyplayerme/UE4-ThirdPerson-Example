// Copyright © 2021++ Ready Player Me

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ReadyPlayerMeTypes.h"
#include "glTFRuntimeFunctionLibrary.h"
#include "Interfaces/IHttpRequest.h"
#include "ReadyPlayerMeAvatarLoader.generated.h"
/**
 * It's responsible for Loading the avatar from the url and storing it in the local storage.
 * ReadyPlayerMeAvatarLoader is used by ReadyPlayerMeActorComponent for loading the avatar.
 */
UCLASS(BlueprintType)
class READYPLAYERME_API UReadyPlayerMeAvatarLoader : public UObject
{
	GENERATED_BODY()

public:
	/** Default constructor. */
	UReadyPlayerMeAvatarLoader();

	/**
	 * Downloads the avatar asset from the Url and saves it in the local storage.
	 *
	 * @param Url Avatar url or shortcode.
	 * @param OnLoadCompleted Success callback. Called when the avatar asset is loaded.
	 * @param OnLoadFailed Failure callback. If the avatar fails to load, the failure callback will be called.
	 * @param bShouldLoadMetadata Boolean flag for loading the metadata.
	 */
	UFUNCTION(BlueprintCallable, Category = "Ready Player Me", meta = (DisplayName = "Load Avatar", AutoCreateRefTerm = "OnLoadFailed"))
	void LoadAvatar(const FString& Url, const FAvatarLoadCompleted& OnLoadCompleted, const FAvatarLoadFailed& OnLoadFailed, bool bShouldLoadMetadata = true);

private:
	static bool IsCachingEnabled();

	void OnMetadataReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess);
	
	void OnAvatarModelReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess);
	
	void LoadAvatarMetadata();
	
	void LoadAvatarModel();

	bool IsMetadataChanged() const;

	void ProcessReceivedMetadata(const FString& ResponseContent);

	void SaveMetadata(const FString& ResponseContent);

	void ExecuteSuccessCallback();

	void ExecuteFailureCallback(const FString& ErrorMessage);
	
	void LoadMetadataAndModel(bool bShouldLoadMetadata);
	
	void Reset();

	UPROPERTY()
	UglTFRuntimeAsset* GlTFRuntimeAsset;

	TOptional<FAvatarMetadata> AvatarMetadata;
	TOptional<FAvatarUri> AvatarUri;

	FAvatarLoadCompleted OnAvatarLoadCompleted;
	FAvatarLoadFailed OnAvatarLoadFailed;

	bool bIsTryingToUpdate;

};
