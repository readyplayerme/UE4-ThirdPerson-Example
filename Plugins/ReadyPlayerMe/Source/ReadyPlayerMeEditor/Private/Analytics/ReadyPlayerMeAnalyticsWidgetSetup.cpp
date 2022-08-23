// Copyright © 2021++ Ready Player Me

#include "ReadyPlayerMeAnalyticsWidgetSetup.h"

#include "ReadyPlayerMeEditorSettings.h"
#include "ReadyPlayerMeAnalyticsEventLogger.h"
#include "EditorUtilitySubsystem.h"

static const FString STARTUP_BLUEPRINT = "/ReadyPlayerMe/Editor/BP_RPM_StartupEditor.BP_RPM_StartupEditor";

void FReadyPlayerMeAnalyticsWidgetSetup::Startup()
{
	const auto EditorSettings = GetDefault<UReadyPlayerMeEditorSettings>();
	FReadyPlayerMeAnalyticsEventLogger::Get().SetEnabled(EditorSettings->bEnableAnalytics);
	if (EditorSettings->bDontAskAgain || EditorSettings->bEnableAnalytics)
	{
		RemoveWidget();
	}
	else
	{
		AddWidget();
	}
}

void FReadyPlayerMeAnalyticsWidgetSetup::AddWidget()
{
	const auto EditorUtilitySubsystem = GetMutableDefault<UEditorUtilitySubsystem>();
	if (!EditorUtilitySubsystem->StartupObjects.Contains(STARTUP_BLUEPRINT))
	{
		EditorUtilitySubsystem->StartupObjects.Add(STARTUP_BLUEPRINT);
		EditorUtilitySubsystem->SaveConfig();
	}
}

void FReadyPlayerMeAnalyticsWidgetSetup::RemoveWidget()
{
	const auto EditorUtilitySubsystem = GetMutableDefault<UEditorUtilitySubsystem>();
	if (EditorUtilitySubsystem->StartupObjects.Contains(STARTUP_BLUEPRINT))
	{
		EditorUtilitySubsystem->StartupObjects.Remove(STARTUP_BLUEPRINT);
		EditorUtilitySubsystem->SaveConfig();
	}
}
