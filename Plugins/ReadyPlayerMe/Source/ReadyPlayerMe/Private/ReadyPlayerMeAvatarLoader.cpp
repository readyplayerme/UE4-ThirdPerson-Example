// Copyright © 2021++ Ready Player Me


#include "ReadyPlayerMeAvatarLoader.h"
#include "Utils/ReadyPlayerMeGlTFConfigCreator.h"
#include "Utils/ReadyPlayerMeMetadataExtractor.h"
#include "Utils/ReadyPlayerMeAvatarStorage.h"
#include "Utils/ReadyPlayerMeRequestCreator.h"
#include "Utils/ReadyPlayerMeUrlConvertor.h"

static const FString HEADER_LAST_MODIFIED = "Last-Modified";
constexpr float METADATA_REQUEST_TIMEOUT = 20.f;
constexpr float AVATAR_REQUEST_TIMEOUT = 60.f;

UReadyPlayerMeAvatarLoader::UReadyPlayerMeAvatarLoader()
	: GlTFRuntimeAsset(nullptr)
	, bIsTryingToUpdate(false)
{
}

void UReadyPlayerMeAvatarLoader::LoadAvatar(const FString& Url, const FAvatarLoadCompleted& OnLoadCompleted, const FAvatarLoadFailed& OnLoadFailed, bool bShouldLoadMetadata)
{
	if (Url.IsEmpty())
	{
		(void)OnLoadFailed.ExecuteIfBound("Url is empty");
		return;
	}
	Reset();
	OnAvatarLoadCompleted = OnLoadCompleted;
	OnAvatarLoadFailed = OnLoadFailed;
	AvatarUri = FReadyPlayerMeUrlConvertor::CreateAvatarUri(Url);
	if (FReadyPlayerMeAvatarStorage::AvatarExists(*AvatarUri))
	{
		bIsTryingToUpdate = true;
		LoadAvatarMetadata();
	}
	else
	{
		LoadMetadataAndModel(bShouldLoadMetadata);
	}
}

void UReadyPlayerMeAvatarLoader::LoadMetadataAndModel(bool bShouldLoadMetadata)
{
	if (bShouldLoadMetadata)
	{
		LoadAvatarMetadata();
	}
	else
	{
		AvatarMetadata = FAvatarMetadata();
	}
	LoadAvatarModel();
}

bool UReadyPlayerMeAvatarLoader::IsMetadataChanged() const
{
	const FString MetadataStr = FReadyPlayerMeAvatarStorage::LoadMetadata(AvatarUri->LocalMetadataPath);
	if (!MetadataStr.IsEmpty())
	{
		const FAvatarMetadata LocalMetadata = FReadyPlayerMeMetadataExtractor::ExtractAvatarMetadata(MetadataStr);
		if (LocalMetadata.LastModifiedDate == AvatarMetadata->LastModifiedDate)
		{
			return false;
		}
	}
	return true;
}

void UReadyPlayerMeAvatarLoader::ProcessReceivedMetadata(const FString& ResponseContent)
{
	if (!bIsTryingToUpdate)
	{
		SaveMetadata(ResponseContent);
		ExecuteSuccessCallback();
		return;
	}
	if (IsMetadataChanged())
	{
		SaveMetadata(ResponseContent);
		LoadAvatarModel();
		return;
	}
	GlTFRuntimeAsset = UglTFRuntimeFunctionLibrary::glTFLoadAssetFromFilename(AvatarUri->LocalModelPath, false, FReadyPlayerMeGlTFConfigCreator::GetGlTFRuntimeConfig());
	if (GlTFRuntimeAsset != nullptr)
	{
		ExecuteSuccessCallback();
	}
	else
	{
		UE_LOG(LogReadyPlayerMe, Warning, TEXT("Failed to load the model from the local storage"));
		LoadAvatarModel();
	}
}

void UReadyPlayerMeAvatarLoader::ExecuteSuccessCallback()
{
	if (GlTFRuntimeAsset != nullptr && AvatarMetadata.IsSet())
	{
		(void)OnAvatarLoadCompleted.ExecuteIfBound(GlTFRuntimeAsset, AvatarMetadata.GetValue());
		Reset();
	}
}

void UReadyPlayerMeAvatarLoader::ExecuteFailureCallback(const FString& ErrorMessage)
{
	(void)OnAvatarLoadFailed.ExecuteIfBound(ErrorMessage);
	Reset();
}

void UReadyPlayerMeAvatarLoader::SaveMetadata(const FString& ResponseContent)
{
	const FString UpdatedMetadataStr = FReadyPlayerMeMetadataExtractor::AddModifiedDateToMetadataJson(ResponseContent, AvatarMetadata->LastModifiedDate);
	if (!FReadyPlayerMeAvatarStorage::SaveMetadata(AvatarUri->LocalMetadataPath, UpdatedMetadataStr))
	{
		UE_LOG(LogReadyPlayerMe, Warning, TEXT("Failed to save the avatar metadata"));
	}
}

void UReadyPlayerMeAvatarLoader::OnMetadataReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess)
{
	if (bSuccess && Response.IsValid())
	{
		AvatarMetadata = FReadyPlayerMeMetadataExtractor::ExtractAvatarMetadata(Response->GetContentAsString());
		AvatarMetadata->LastModifiedDate = Response->GetHeader(HEADER_LAST_MODIFIED);
		ProcessReceivedMetadata(Response->GetContentAsString());
	}
	else
	{
		ExecuteFailureCallback("Failed to retrieve avatar metadata");
	}
}

void UReadyPlayerMeAvatarLoader::OnAvatarModelReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess)
{
	if (bSuccess && Response.IsValid())
	{
		const TArray<uint8>& AvatarData = Response->GetContent();
		GlTFRuntimeAsset = UglTFRuntimeFunctionLibrary::glTFLoadAssetFromData(AvatarData, FReadyPlayerMeGlTFConfigCreator::GetGlTFRuntimeConfig());
		if (GlTFRuntimeAsset == nullptr)
		{
			ExecuteFailureCallback("Failed to load the avatar model");
			return;
		}
		if (!FReadyPlayerMeAvatarStorage::SaveAvatar(AvatarUri->LocalModelPath, AvatarData))
		{
			UE_LOG(LogReadyPlayerMe, Warning, TEXT("Failed to save the avatar model"));
		}
		ExecuteSuccessCallback();
	}
	else
	{
		ExecuteFailureCallback("Failed to retrieve Avatar Meta Data");
	}
}

void UReadyPlayerMeAvatarLoader::LoadAvatarMetadata()
{
	auto HttpRequest = FReadyPlayerMeRequestCreator::MakeHttpRequest(AvatarUri->MetadataUrl, METADATA_REQUEST_TIMEOUT);
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &UReadyPlayerMeAvatarLoader::OnMetadataReceived);
	HttpRequest->ProcessRequest();
}

void UReadyPlayerMeAvatarLoader::LoadAvatarModel()
{
	auto HttpRequest = FReadyPlayerMeRequestCreator::MakeHttpRequest(AvatarUri->ModelUrl, AVATAR_REQUEST_TIMEOUT);
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &UReadyPlayerMeAvatarLoader::OnAvatarModelReceived);
	HttpRequest->ProcessRequest();
}

void UReadyPlayerMeAvatarLoader::Reset()
{
	bIsTryingToUpdate = false;
	GlTFRuntimeAsset = nullptr;
	AvatarMetadata.Reset();
	AvatarUri.Reset();
	OnAvatarLoadCompleted.Unbind();
	OnAvatarLoadFailed.Unbind();
}
