// Copyright © 2021++ Ready Player Me


#include "Utils/ReadyPlayerMeUrlConvertor.h"
#include "Internationalization/Regex.h"

static const FString SHORTCODE_URL_PREFIX = "https://models.readyplayer.me/";
static const FString URL_PATTERN = "https?:\\/\\/(www\\.)?[-a-zA-Z0-9@:%._\\+~#=]{2,256}\\.[a-z]{2,4}\\b([-a-zA-Z0-9@:%_\\+.~#?&//=]*)";
static const FString SHORTCODE_PATTERN = "^[a-zA-Z0-9]*$";
static const TCHAR* SUFFIX_GLB = TEXT(".glb");
static const TCHAR* SUFFIX_JSON = TEXT(".json");

FString FReadyPlayerMeUrlConvertor::GetValidatedUrlShortCode(const FString& UrlShortCode)
{
	if (UrlShortCode.IsEmpty() || IsUrl(UrlShortCode))
	{
		return UrlShortCode;
	}
	FString ShortCodeUrl = GetUrlFromShortcode(UrlShortCode);

	return ShortCodeUrl;
}

FString FReadyPlayerMeUrlConvertor::GetUrlFromShortcode(const FString& Shortcode)
{
	const FRegexPattern RegexPattern(SHORTCODE_PATTERN);
	FRegexMatcher RegexMatcher(RegexPattern, Shortcode);

	return RegexMatcher.FindNext() ? SHORTCODE_URL_PREFIX + Shortcode + SUFFIX_GLB : TEXT("");
}

bool FReadyPlayerMeUrlConvertor::IsUrl(const FString& Url)
{
	const FRegexPattern RegexPattern(URL_PATTERN);
	FRegexMatcher RegexMatcher = FRegexMatcher(RegexPattern, Url);

	return RegexMatcher.FindNext();
}

FAvatarUri FReadyPlayerMeUrlConvertor::CreateAvatarUri(const FString& Url)
{
	FString UrlLink, UrlQueryString;
	if (!Url.Split(TEXT("?"), &UrlLink, &UrlQueryString))
	{
		UrlLink = Url;
	}
	FString Path, Guid, Extension;
	FPaths::Split(UrlLink, Path, Guid, Extension);
	const FString LocalFilename = FPaths::ProjectPersistentDownloadDir() + "/Avatars/" + Guid + "/" + Guid;
	const FString UrlPath = Path + "/" + Guid;
	FAvatarUri AvatarUri;
	AvatarUri.Guid = Guid;
	AvatarUri.ModelUrl = UrlPath + SUFFIX_GLB;
	AvatarUri.LocalModelPath = LocalFilename + SUFFIX_GLB;
	AvatarUri.MetadataUrl = UrlPath + SUFFIX_JSON;
	AvatarUri.LocalMetadataPath = LocalFilename + SUFFIX_JSON;
	return AvatarUri;
}
