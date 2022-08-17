// Copyright © 2021++ Ready Player Me

#include "ReadyPlayerMeEditor.h"

#include "Analytics/ReadyPlayerMeAnalyticsWidgetSetup.h"
#include "Analytics/ReadyPlayerMeAnalyticsEventLogger.h"

#define LOCTEXT_NAMESPACE "FReadyPlayerMeEditorModule"

void FReadyPlayerMeEditorModule::StartupModule()
{
	FReadyPlayerMeAnalyticsWidgetSetup::Startup();
	FReadyPlayerMeAnalyticsEventLogger::Get().LogOpenProject();
}

void FReadyPlayerMeEditorModule::ShutdownModule()
{
	FReadyPlayerMeAnalyticsEventLogger::Get().LogCloseProject();
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FReadyPlayerMeEditorModule, ReadyPlayerMeEditor)
