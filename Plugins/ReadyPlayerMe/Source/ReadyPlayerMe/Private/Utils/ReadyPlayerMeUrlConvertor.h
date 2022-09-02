// Copyright © 2021++ Ready Player Me

#pragma once

#include "CoreMinimal.h"
#include "ReadyPlayerMeTypes.h"

class FReadyPlayerMeUrlConvertor
{
public:
	static FString GetValidatedUrlShortCode(const FString& UrlShortCode);

	static bool IsUrl(const FString& Url);

	static FAvatarUri CreateAvatarUri(const FString& Url);

private:
	static FString GetUrlFromShortcode(const FString& Shortcode);
};
