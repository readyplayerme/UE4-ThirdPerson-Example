// Copyright © 2021++ Ready Player Me

#include "ReadyPlayerMeEditorSettings.h"
#include "Analytics/ReadyPlayerMeAnalyticsWidgetSetup.h"
#include "Analytics/ReadyPlayerMeAnalyticsEventLogger.h"

USReadyPlayerMeEditorSettings::USReadyPlayerMeEditorSettings()
	: bEnableAnalytics(false)
	, bDontAskAgain(false)
{
}

void USReadyPlayerMeEditorSettings::EnableAnalytics() {
	USReadyPlayerMeEditorSettings* Settings = GetMutableDefault<USReadyPlayerMeEditorSettings>();
	if (Settings)
	{
		Settings->bEnableAnalytics = true;
		Settings->SaveConfig();
		FReadyPlayerMeAnalyticsEventLogger::Get().EnableAnalytics();
		FReadyPlayerMeAnalyticsWidgetSetup::RemoveWidget();
	}
}

void USReadyPlayerMeEditorSettings::SetDontAskAgain(bool bDontAsk)
{
	USReadyPlayerMeEditorSettings* Settings = GetMutableDefault<USReadyPlayerMeEditorSettings>();
	if (Settings)
	{
		Settings->bDontAskAgain = bDontAsk;
		Settings->SaveConfig();
		FReadyPlayerMeAnalyticsWidgetSetup::RemoveWidget();
	}
}
