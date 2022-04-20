// Copyright © 2021++ Ready Player Me
#pragma once

#include "ReadyPlayerMeTypes.generated.h"

UENUM(BlueprintType)
enum AvatarGender
{
	Masculine UMETA(DisplayName = GENDER_MASCULINE),
	Feminine UMETA(DisplayName = GENDER_FEMININE)
};

UENUM(BlueprintType)
enum AvatarBodyType
{
	Undefined UMETA(DisplayName = TYPE_UNDEFINED),
	Fullbody UMETA(DisplayName = TYPE_FULLBODY),
	Halfbody UMETA(DisplayName = TYPE_HALFBODY)
};


USTRUCT(BlueprintType)
struct FAvatarMetadata
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ReadyPlayerMe")
		TEnumAsByte<AvatarBodyType> bodyType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ReadyPlayerMe")
		TEnumAsByte<AvatarGender> outfitGender;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ReadyPlayerMe")
		int32 outfitVersion;

	FAvatarMetadata()
	{
		bodyType = Undefined;
		outfitGender = Masculine;
		outfitVersion = 1;
	}
};

DECLARE_DYNAMIC_DELEGATE_OneParam(FAvatarMetadataHttpResponse, const FAvatarMetadata&, AvataMetadata);

DECLARE_DYNAMIC_DELEGATE_OneParam(FAvatarLoadFailed, const FString, errorMessage);
