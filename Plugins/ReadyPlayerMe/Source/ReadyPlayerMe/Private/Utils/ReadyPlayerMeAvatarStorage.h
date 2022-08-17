// Copyright © 2021++ Ready Player Me

#pragma once

#include "CoreMinimal.h"
#include "ReadyPlayerMeTypes.h"

class FReadyPlayerMeAvatarStorage
{
public:
	static bool SaveAvatar(const FString& GlbFilePath, const TArray<uint8>& Data);
	static bool SaveMetadata(const FString& MetadataFilePath, const FString& Content);
	
	static bool AvatarExists(const FAvatarUri& AvatarUri);
	static FString LoadMetadata(const FString& Path);

private:
	static bool CheckAndRemoveExistingFile(const FString& FilePath);
};
