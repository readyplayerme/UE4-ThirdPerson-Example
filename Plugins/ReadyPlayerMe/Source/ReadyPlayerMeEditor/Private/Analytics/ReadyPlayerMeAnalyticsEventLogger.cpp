// Copyright © 2021++ Ready Player Me

#include "ReadyPlayerMeAnalyticsEventLogger.h"
#include "ReadyPlayerMeAmplitudeEventLogger.h"

static const FString EVENT_OPEN_PROJECT = "open project";
static const FString EVENT_CLOSE_PROJECT = "close project";

FReadyPlayerMeAnalyticsEventLogger& FReadyPlayerMeAnalyticsEventLogger::Get()
{
	static FReadyPlayerMeAnalyticsEventLogger EventLogger;
	return EventLogger;
}

void FReadyPlayerMeAnalyticsEventLogger::LogOpenProject() const
{
	if (!bIsEnabled)
	{
		return;
	}
	Logger.IdentifyUser();
	Logger.LogEvent(EVENT_OPEN_PROJECT);
}

void FReadyPlayerMeAnalyticsEventLogger::LogCloseProject() const
{
	if (!bIsEnabled)
	{
		return;
	}
	Logger.LogEvent(EVENT_CLOSE_PROJECT);
}

void FReadyPlayerMeAnalyticsEventLogger::SetEnabled(bool bEnabled)
{
	bIsEnabled = bEnabled;
}

void FReadyPlayerMeAnalyticsEventLogger::EnableAnalytics()
{
	bIsEnabled = true;
	Logger.LogAllowLogging();
}

