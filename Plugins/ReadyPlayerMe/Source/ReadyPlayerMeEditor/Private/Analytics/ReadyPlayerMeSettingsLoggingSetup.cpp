// Copyright © 2021++ Ready Player Me

#include "ReadyPlayerMeSettingsLoggingSetup.h"

#include "ReadyPlayerMeAnalyticsEventLogger.h"
#include "ReadyPlayerMeSettings.h"

static const FName SETTINGS_ENABLE_CACHING = "bEnableAvatarCaching";

namespace
{
	void ProjectSettingsChanged(const FName& PropertyName, UReadyPlayerMeSettings* Settings)
	{
		if (PropertyName == SETTINGS_ENABLE_CACHING)
		{
			FReadyPlayerMeAnalyticsEventLogger::Get().LogEnableAvatarCaching(Settings->bEnableAvatarCaching);
		}
	}
}

void FReadyPlayerMeSettingsLoggingSetup::Setup()
{
	UReadyPlayerMeSettings* Settings = GetMutableDefault<UReadyPlayerMeSettings>();
	if (Settings)
	{
		Settings->SettingsChanged.BindStatic(&ProjectSettingsChanged, Settings);
	}
}

