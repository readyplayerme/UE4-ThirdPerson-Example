// Copyright © 2021++ Ready Player Me

#include "ReadyPlayerMeAnalyticsData.h"

#include "Interfaces/IPluginManager.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Misc/EngineVersion.h"

static const FString PLUGIN_NAME = "ReadyPlayerMe";
static const FString SDK_TARGET = "Unreal";
static const FString SUBDOMAIN = "Demo";

namespace
{
	FString GetPluginVersion()
	{
		const TSharedPtr<IPlugin> Plugin = IPluginManager::Get().FindPlugin(PLUGIN_NAME);
		if (Plugin) {
			const FPluginDescriptor& Descriptor = Plugin->GetDescriptor();
			return Descriptor.VersionName;
		}
		return "";
	}

	FString GetDeviceId()
	{
		const TArray<uint8> MacAddr = FPlatformMisc::GetMacAddress();
		return FMD5::HashBytes(MacAddr.GetData(), MacAddr.Num());
	}
}

FReadyPlayerMeAnalyticsData::FReadyPlayerMeAnalyticsData()
{
	const auto EngineVersion = FEngineVersion::Current();
	AppName = FApp::GetProjectName();
	SDKVersion = GetPluginVersion();
	SdkTarget = SDK_TARGET;
	Subdomain = SUBDOMAIN;
	UnrealVersion = FString::Printf(TEXT("%d.%d.%d"), EngineVersion.GetMajor(), EngineVersion.GetMinor(), EngineVersion.GetPatch());
	Platform = UGameplayStatics::GetPlatformName();
	DeviceId = GetDeviceId();
	AppIdentifier = UKismetSystemLibrary::GetGameBundleId();
}
