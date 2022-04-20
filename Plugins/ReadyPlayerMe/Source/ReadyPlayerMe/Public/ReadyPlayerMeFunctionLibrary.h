// Copyright © 2021++ Ready Player Me

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ReadyPlayerMeTypes.h"
#include "ReadyPlayerMeFunctionLibrary.generated.h"

class USkeletalMeshComponent;

UCLASS()
class READYPLAYERME_API UReadyPlayerMeFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

		
public:

		UFUNCTION(BlueprintCallable, meta = (DefaultToSelf = "target", DisplayName = "RPM Add Skeletal Mesh Component"), Category = "Ready Player Me")
		static USkeletalMeshComponent* AddSkeletalMeshComponent(AActor* target);

		UFUNCTION(BlueprintCallable, meta = (DefaultToSelf = "targetActor", DisplayName = "RPM Setup Skeletal Mesh"), Category = "Ready Player Me")
		static USkeletalMeshComponent* SetupSkeletalMeshComponent(AActor* targetActor, USkeletalMesh* skeletalMesh, const FAvatarMetadata& avatarMetaData);

		UFUNCTION(BlueprintCallable, meta = (DisplayName = "Validate Url ShortCode"), Category = "Ready Player Me")
		static FString GetValidatedUrlShortCode(const FString& urlShortCode);

		UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Url From Shortcode"), Category = "Ready Player Me")
		static FString GetUrlFromShortcode(const FString& shortcode);

		UFUNCTION(BlueprintCallable, meta = (DisplayName = "Check Is Url"), Category = "Ready Player Me")
		static bool IsUrl(const FString& url);
	
		UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get glTFRuntime Skeleton Config"), Category = "Ready Player Me")
		static struct FglTFRuntimeSkeletonConfig GetglTFRuntimeSkeletonConfig(const FString& rootBoneName, USkeleton* skeleton);

		UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get glTFRuntime Materials Config"), Category = "Ready Player Me")
		static struct FglTFRuntimeMaterialsConfig GetglTFRuntimeMaterialsConfig();

		UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get glTFRuntime Skeletal Mesh Config"), Category = "Ready Player Me")
		static struct FglTFRuntimeSkeletalMeshConfig GetglTFRuntimeSkeletalMeshConfig(const FAvatarMetadata& avatarMetaData, USkeleton* skeleton = nullptr );

		UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get glTFRuntime Runtime Config"), Category = "Ready Player Me")
		static struct FglTFRuntimeConfig GetglTFRuntimeConfig();

		UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Root Bone Name"), Category = "Ready Player Me")
		static FString GetRootBoneName(const AvatarBodyType& avatarBodyType);

		UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Body Type From Asset"), Category = "Ready Player Me")	
		static AvatarBodyType GetBodyTypeFromAsset(class UglTFRuntimeAsset* gltfRuntimeAsset);

		UFUNCTION(BlueprintCallable, meta = (DisplayName = "Load Avatar Metadata" , AutoCreateRefTerm = "onLoadFailed"), Category = "Ready Player Me")
		static void LoadAvatarMetadata(const FString& url, const FAvatarMetadataHttpResponse& completed, const FAvatarLoadFailed& onLoadFailed);

		static FAvatarMetadata OnResponseReceived(const FString& jsonString);

		UFUNCTION(BlueprintCallable, meta = (DisplayName = "Set Avatar Morph targets"), Category = "Ready Player Me")
		static void SetMorphTargets(USkeletalMeshComponent* skeletalMeshComponent, const TMap<FString, float>& morphTargetMap);

		UFUNCTION(BlueprintCallable, meta = (DisplayName = "Set Avatar Morph target"), Category = "Ready Player Me")
		static void SetMorphTarget(USkeletalMeshComponent* skeletalMeshComponent, const FString & name, float value);

		static const FString DEFAULT_SKELETON_PATH;
		static const FString DEFAULT_ANIMATION_BLUEPRINT_PATH;
		static const FString SHORTCODE_URL_PREFIX;
		static const FString URL_PATTERN;
		static const FString SHORTCODE_PATTERN;
		static const int MAX_HALFBODY_NODES=60;
		static const FString FULLBODY_BONE_NODE;
		static const FString HALFBODY_BONE_NODE;
		static const int SCENE_SCALE=1;
		static const FString GENDER_FEMININE;
		static const FString GENDER_MASCULINE;
		static const FString TYPE_HALFBODY;
		static const FString TYPE_FULLBODY;
		static const FName SKELETAL_MESH_NAME;
		static const TCHAR * SUFFIX_GLB;
		static const TCHAR * SUFFIX_JSON;
		static const FString JSON_FIELD_OUTFIT;
		static const FString JSON_FIELD_BODY;
		static const FString JSON_FIELD_GENDER;
};
