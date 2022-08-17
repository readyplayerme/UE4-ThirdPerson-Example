// Copyright © 2021++ Ready Player Me


#include "ReadyPlayerMeAvatarStorage.h"

#include "HAL/PlatformFilemanager.h"
#include "GenericPlatform/GenericPlatformFile.h"
#include "Misc/Paths.h"
#include "Misc/FileHelper.h"

FString FReadyPlayerMeAvatarStorage::LoadMetadata(const FString& Path)
{
	if (!Path.IsEmpty() && FPaths::FileExists(*Path))
	{
		FString ResultStr;
		if (FFileHelper::LoadFileToString(ResultStr, *Path))
		{
			return ResultStr;
		}
		UE_LOG(LogReadyPlayerMe, Warning, TEXT("Failed to load the metadata"));
	}
	return "";
}

bool FReadyPlayerMeAvatarStorage::AvatarExists(const FAvatarUri& AvatarUri)
{
	return !AvatarUri.LocalMetadataPath.IsEmpty() && FPaths::FileExists(*AvatarUri.LocalMetadataPath)
		&& !AvatarUri.LocalModelPath.IsEmpty() && FPaths::FileExists(*AvatarUri.LocalModelPath);
}

bool FReadyPlayerMeAvatarStorage::CheckAndRemoveExistingFile(const FString& FilePath)
{
	if (FilePath.IsEmpty())
	{
		return false;
	}
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

	FString Path;
	FString Filename;
	FString Extension;
	FPaths::Split(FilePath, Path, Filename, Extension);
	if (!PlatformFile.DirectoryExists(*Path) && !PlatformFile.CreateDirectoryTree(*Path))
	{
		UE_LOG(LogReadyPlayerMe, Warning, TEXT("Failed to create a directory to save the downloaded file"));
		return false;
	}

	if (FPaths::FileExists(*FilePath) && !IFileManager::Get().Delete(*FilePath))
	{
		UE_LOG(LogReadyPlayerMe, Warning, TEXT("Failed to delete the existing file"));
		return false;
	}
	return true;
}

bool FReadyPlayerMeAvatarStorage::SaveAvatar(const FString& GlbFilePath, const TArray<uint8>& Data)
{
	if (!CheckAndRemoveExistingFile(GlbFilePath))
	{
		return false;
	}
	if (FFileHelper::SaveArrayToFile(Data, *GlbFilePath))
	{
		return true;
	}

	UE_LOG(LogReadyPlayerMe, Warning, TEXT("Failed to save the downloaded file"));
	return false;
}

bool FReadyPlayerMeAvatarStorage::SaveMetadata(const FString& MetadataFilePath, const FString& Content)
{
	if (!CheckAndRemoveExistingFile(MetadataFilePath))
	{
		return false;
	}
	if (FFileHelper::SaveStringToFile(Content, *MetadataFilePath))
	{
		return true;
	}

	UE_LOG(LogReadyPlayerMe, Warning, TEXT("Failed to save the downloaded file"));
	return false;
}
