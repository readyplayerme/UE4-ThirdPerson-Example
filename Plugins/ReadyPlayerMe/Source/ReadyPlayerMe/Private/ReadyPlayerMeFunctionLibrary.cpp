// Copyright © 2021++ Ready Player Me


#include "ReadyPlayerMeFunctionLibrary.h"
#include "glTFRuntimeParser.h"
#include "glTFRuntimeAsset.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "UObject/UObjectGlobals.h"
#include "Internationalization/Regex.h"
#include "Templates/SharedPointer.h"
#include "Animation/AnimBlueprint.h"
//required for Android builds
#include "Components/SkeletalMeshComponent.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"

const FString UReadyPlayerMeFunctionLibrary::SHORTCODE_URL_PREFIX = "https://readyplayer.me/api/avatar/";
const FString UReadyPlayerMeFunctionLibrary::URL_PATTERN = "https?:\\/\\/(www\\.)?[-a-zA-Z0-9@:%._\\+~#=]{2,256}\\.[a-z]{2,4}\\b([-a-zA-Z0-9@:%_\\+.~#?&//=]*)";
const FString UReadyPlayerMeFunctionLibrary::SHORTCODE_PATTERN = "^[a-zA-Z0-9]*$";
const FString UReadyPlayerMeFunctionLibrary::FULLBODY_BONE_NODE = "Armature";
const FString UReadyPlayerMeFunctionLibrary::HALFBODY_BONE_NODE = "AvatarRoot";
const FString UReadyPlayerMeFunctionLibrary::GENDER_FEMININE = "Feminine";
const FString UReadyPlayerMeFunctionLibrary::GENDER_MASCULINE = "Masculine";
const FString UReadyPlayerMeFunctionLibrary::TYPE_FULLBODY = "Fullbody";
const FString UReadyPlayerMeFunctionLibrary::TYPE_HALFBODY = "Halfbody";
const FName UReadyPlayerMeFunctionLibrary::SKELETAL_MESH_NAME = "Skeletal Mesh";
const TCHAR * UReadyPlayerMeFunctionLibrary::SUFFIX_GLB = TEXT(".glb");
const TCHAR * UReadyPlayerMeFunctionLibrary::SUFFIX_JSON = TEXT(".json");
const FString UReadyPlayerMeFunctionLibrary::JSON_FIELD_OUTFIT = "outfitVersion";
const FString UReadyPlayerMeFunctionLibrary::JSON_FIELD_BODY = "bodyType";
const FString UReadyPlayerMeFunctionLibrary::JSON_FIELD_GENDER = "outfitGender";

USkeletalMeshComponent* UReadyPlayerMeFunctionLibrary::AddSkeletalMeshComponent(AActor* target)
{
	USkeletalMeshComponent* skeletalMeshComponent = NewObject<USkeletalMeshComponent>(target, SKELETAL_MESH_NAME);
	skeletalMeshComponent->SetupAttachment(target->GetRootComponent());
	skeletalMeshComponent->RegisterComponent();
	
	return skeletalMeshComponent;
}

USkeletalMeshComponent* UReadyPlayerMeFunctionLibrary::SetupSkeletalMeshComponent(AActor* targetActor, USkeletalMesh* skeletalMesh, const FAvatarMetadata& avatarMetaData)
{
	USkeletalMeshComponent* skeletalMeshComponent = targetActor->FindComponentByClass<USkeletalMeshComponent>();
	if(!skeletalMeshComponent)
	{
		skeletalMeshComponent = AddSkeletalMeshComponent(targetActor);
	}

	skeletalMeshComponent->SetSkeletalMesh(skeletalMesh);

	return skeletalMeshComponent;
}

FString UReadyPlayerMeFunctionLibrary::GetValidatedUrlShortCode(const FString& urlShortCode)
{
	if (urlShortCode.IsEmpty() || IsUrl(urlShortCode))
	{
		return urlShortCode;
	}	
	FString shortCodeUrl = GetUrlFromShortcode(urlShortCode);
	
	return shortCodeUrl;
}

FString UReadyPlayerMeFunctionLibrary::GetUrlFromShortcode(const FString& shortcode) 
{
	FRegexPattern regexPattern = FRegexPattern(TCHAR_TO_UTF8(*SHORTCODE_PATTERN));
	FRegexMatcher regexMatcher = FRegexMatcher(regexPattern, shortcode);
	
	return regexMatcher.FindNext() ? SHORTCODE_URL_PREFIX + shortcode : TEXT("");
}

bool UReadyPlayerMeFunctionLibrary::IsUrl(const FString& url)
{
	FRegexPattern regexPattern = FRegexPattern(TCHAR_TO_UTF8(*URL_PATTERN));;
	FRegexMatcher regexMatcher = FRegexMatcher(regexPattern, url);
	
	return regexMatcher.FindNext();
}

FglTFRuntimeSkeletonConfig UReadyPlayerMeFunctionLibrary::GetglTFRuntimeSkeletonConfig(const FString & rootBoneName, USkeleton* skeleton)
{
	FglTFRuntimeSkeletonConfig skeletonConfig;
	skeletonConfig.CopyRotationsFrom = skeleton;
	skeletonConfig.bAddRootBone = true;
	skeletonConfig.RootBoneName = rootBoneName;
	return skeletonConfig;
}

FglTFRuntimeMaterialsConfig UReadyPlayerMeFunctionLibrary::GetglTFRuntimeMaterialsConfig()
{
	FglTFRuntimeMaterialsConfig materialsConfig;
	materialsConfig.bMergeSectionsByMaterial = true;
	
	return materialsConfig;
}

FglTFRuntimeSkeletalMeshConfig UReadyPlayerMeFunctionLibrary::GetglTFRuntimeSkeletalMeshConfig(const FAvatarMetadata & avatarMetaData, USkeleton* skeleton)
{
	FglTFRuntimeSkeletalMeshConfig skeletonMeshConfig;
	skeletonMeshConfig.SkeletonConfig = GetglTFRuntimeSkeletonConfig(GetRootBoneName(avatarMetaData.bodyType), skeleton);
	skeletonMeshConfig.MorphTargetsDuplicateStrategy = EglTFRuntimeMorphTargetsDuplicateStrategy::Merge;
	skeletonMeshConfig.MaterialsConfig = GetglTFRuntimeMaterialsConfig();
	if(skeleton)
	{
		skeletonMeshConfig.Skeleton = skeleton;
	}

	return skeletonMeshConfig;
}

FglTFRuntimeConfig UReadyPlayerMeFunctionLibrary::GetglTFRuntimeConfig()
{
	FglTFRuntimeConfig runtimeConfig;
	runtimeConfig.TransformBaseType = EglTFRuntimeTransformBaseType::YForward;
	
	return runtimeConfig;
}

FString UReadyPlayerMeFunctionLibrary::GetRootBoneName(const AvatarBodyType & avatarBodyType)
{
	if (avatarBodyType == Fullbody)
	{
		return FULLBODY_BONE_NODE;
	}
	
	return HALFBODY_BONE_NODE;
}

AvatarBodyType UReadyPlayerMeFunctionLibrary::GetBodyTypeFromAsset(UglTFRuntimeAsset* gltfRuntimeAsset)
{
	if(gltfRuntimeAsset->GetNodes().Num() > MAX_HALFBODY_NODES)
	{
		return Fullbody;
	}
	
	return Halfbody;
}

void UReadyPlayerMeFunctionLibrary::LoadAvatarMetadata(const FString & url, const FAvatarMetadataHttpResponse& completed, const FAvatarLoadFailed& onLoadFailed)
{
	if (url.IsEmpty())
	{
		onLoadFailed.ExecuteIfBound("Url is empty");
		return;
	}
	const FString newUrl = url.Replace(SUFFIX_GLB,SUFFIX_JSON);
#if ENGINE_MAJOR_VERSION > 4 || ENGINE_MINOR_VERSION > 25
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> httpRequest = FHttpModule::Get().CreateRequest();
#else
	TSharedRef<IHttpRequest> httpRequest = FHttpModule::Get().CreateRequest();
#endif
	httpRequest->SetURL(newUrl);
	
	httpRequest->OnProcessRequestComplete().BindLambda([](FHttpRequestPtr request, FHttpResponsePtr response, bool bSuccess, FAvatarMetadataHttpResponse Completed, FAvatarLoadFailed OnLoadFailed)
		{
			if (bSuccess && response.IsValid())
			{
				FAvatarMetadata metaData = OnResponseReceived(response->GetContentAsString());
				Completed.ExecuteIfBound(metaData);
			}
			else
			{
				OnLoadFailed.ExecuteIfBound("Failed to retrieve Avatar Meta Data");
			}
			
		}, completed, onLoadFailed);

	httpRequest->ProcessRequest();
}

FAvatarMetadata UReadyPlayerMeFunctionLibrary::OnResponseReceived(const FString & jsonString)
{
	TSharedPtr<FJsonObject> jsonObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(jsonString);

	FAvatarMetadata metaData = FAvatarMetadata();

	if (FJsonSerializer::Deserialize(Reader, jsonObject))
	{	
		if (jsonObject->HasField(JSON_FIELD_BODY))
		{
			if (jsonObject->GetStringField(JSON_FIELD_BODY).Contains(TYPE_HALFBODY))
			{
				metaData.bodyType = Halfbody;
			}
			else
			{
				metaData.bodyType = Fullbody;
			}
		}
		if (jsonObject->HasField(JSON_FIELD_OUTFIT)) {
			int32 recievedInt = jsonObject->GetIntegerField(JSON_FIELD_OUTFIT);
			metaData.outfitVersion = recievedInt;
		}
		if (jsonObject->HasField(JSON_FIELD_GENDER) && jsonObject->GetStringField(JSON_FIELD_GENDER).Contains(GENDER_FEMININE))
		{
			metaData.outfitGender = Feminine;
		}
	}

	return metaData;
}

void UReadyPlayerMeFunctionLibrary::SetMorphTargets(USkeletalMeshComponent* skeletalMeshComponent, const TMap<FString, float>& morphTargetMap)
{
	for (const TPair<FString, float>& morphTargetPair : morphTargetMap)
	{
		SetMorphTarget(skeletalMeshComponent, morphTargetPair.Key, morphTargetPair.Value);
	}
}

void UReadyPlayerMeFunctionLibrary::SetMorphTarget(USkeletalMeshComponent* skeletalMeshComponent,const FString & name, float value)
{
	FName morphTargetName = FName(name);
	UMorphTarget* morphTarget = skeletalMeshComponent->FindMorphTarget(morphTargetName);
	if (morphTarget != nullptr)
	{
		skeletalMeshComponent->SetMorphTarget(morphTargetName, value);
	}
}
