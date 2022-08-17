// Copyright © 2021++ Ready Player Me
#pragma once

#include "ReadyPlayerMeTypes.generated.h"

UENUM(BlueprintType)
enum class EAvatarGender : uint8
{
	Undefined UMETA(DisplayName = "Gender Undefined"),
	Masculine UMETA(DisplayName = "Gender Masculine"),
	Feminine UMETA(DisplayName = "Gender Feminine")
};

UENUM(BlueprintType)
enum class EAvatarBodyType : uint8
{
	Undefined UMETA(DisplayName = "Undefined Body Type"),
	FullBody UMETA(DisplayName = "Full Body Type"),
	HalfBody UMETA(DisplayName = "Half Body Type")
};

USTRUCT(BlueprintType)
struct FAvatarMetadata
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ReadyPlayerMe")
	EAvatarBodyType BodyType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ReadyPlayerMe")
	EAvatarGender OutfitGender;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ReadyPlayerMe")
	int32 OutfitVersion;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ReadyPlayerMe")
	FString LastModifiedDate;

	FAvatarMetadata()
	{
		BodyType = EAvatarBodyType::Undefined;
		OutfitGender = EAvatarGender::Undefined;
		OutfitVersion = 1;
	}
};

DECLARE_DYNAMIC_DELEGATE_TwoParams(FAvatarLoadCompleted, class UglTFRuntimeAsset*, Asset, const FAvatarMetadata&, Metadata);

DECLARE_DYNAMIC_DELEGATE_OneParam(FAvatarLoadFailed, const FString&, ErrorMessage);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDownloadImageCompleted, class UTexture*, Texture);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDownloadImageFailed, const FString&, ErrorMessage);

UENUM(BlueprintType)
enum class ERenderSceneType : uint8
{
	FullBodyPortrait UMETA(DisplayName = "Full Body Portrait"),
	HalfBodyPortrait UMETA(DisplayName = "Half Body Portrait"),
	FullBodyPortraitTransparent UMETA(DisplayName = "Full Body Portrait Transparent"),
	HalfBodyPortraitTransparent UMETA(DisplayName = "Half Body Portrait Transparent"),
	FullBodyPostureTransparent UMETA(DisplayName = "Full Body Posture Transparent")
};

struct FAvatarUri
{
	FString Guid;
	FString ModelUrl;
	FString LocalModelPath;
	FString MetadataUrl;
	FString LocalMetadataPath;
};

DECLARE_LOG_CATEGORY_EXTERN(LogReadyPlayerMe, Log, All);
