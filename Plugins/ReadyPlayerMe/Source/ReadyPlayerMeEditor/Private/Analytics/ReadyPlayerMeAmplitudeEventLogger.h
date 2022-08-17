// Copyright © 2021++ Ready Player Me

#pragma once

#include "CoreMinimal.h"
#include "ReadyPlayerMeAnalyticsData.h"

class FJsonObject;

class FReadyPlayerMeAmplitudeEventLogger
{
public:
	FReadyPlayerMeAmplitudeEventLogger();

	void LogEvent(const FString& EventName) const;
	void LogAllowLogging() const;
	void IdentifyUser() const;

private:
	TSharedRef<FJsonObject> MakeUserPropertiesJson() const;
	TSharedRef<FJsonObject> MakeEventJson(const FString& EventName) const;

	static void SendEvent(TSharedRef<FJsonObject> EventJson);
	static FString JsonToString(const TSharedRef<FJsonObject> JsonObject);
	static void SendHttpRequest(const FString& Url, const FString& Content);

	const int64 SessionId;
	const FReadyPlayerMeAnalyticsData AnalyticsData;
};
