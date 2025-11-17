// ZumoDePapaya 2025 All Rights Reserved

#include "HardwareInfoLibrary.h"
#include "GenericPlatform/GenericPlatformMisc.h"
#include "GenericPlatform/GenericPlatformDriver.h"
#include "Internationalization/Regex.h"
#include "HAL/IConsoleManager.h"
#include "HardwareInfo.h"
#include "RHI.h"
#include "RHIStrings.h"
#include "HAL/PlatformProcess.h"
#include "Interfaces/IPluginManager.h"
#include "Misc/FileHelper.h"
#include "Serialization/JsonSerializer.h"
#include "HardwareInfoCache.h"

#if PLATFORM_WINDOWS
#include "Windows/WindowsHWrapper.h"
#include "Windows/MinWindows.h"
#include <winreg.h>
#endif

FHardwareDetails UHardwareInfoLibrary::GetHardwareInfo()
{
	return FHardwareInfoCache::GetCachedHardwareInfo();
}

FString UHardwareInfoLibrary::GetCurrentRHIFeatureLevel()
{
	FString Out;
	GetFeatureLevelName(GMaxRHIFeatureLevel, Out);
	return Out;
}

bool UHardwareInfoLibrary::IsCVarRegistered(FString CVarName)
{
	return IConsoleManager::Get().FindConsoleVariable(*CVarName) != nullptr;
}

FString UHardwareInfoLibrary::GetOperatingSystemInfo()
{
	return FPlatformProperties::PlatformName();
}

FString UHardwareInfoLibrary::GetOSBuildInfo()
{
	return FPlatformMisc::GetOSVersion();
}

FString UHardwareInfoLibrary::GetCurrentRHIBackend()
{
	return FHardwareInfo::GetHardwareInfo(NAME_RHI);
}

FString UHardwareInfoLibrary::GetGPUDriverVersion()
{
	FGPUDriverInfo DriverInfo = FPlatformMisc::GetGPUDriverInfo(FPlatformMisc::GetPrimaryGPUBrand());
	return DriverInfo.UserDriverVersion;
}

FString UHardwareInfoLibrary::GetGPUDriverInternalVersion()
{
	FGPUDriverInfo DriverInfo = FPlatformMisc::GetGPUDriverInfo(FPlatformMisc::GetPrimaryGPUBrand());
	return DriverInfo.InternalDriverVersion;
}

FString UHardwareInfoLibrary::GetGPUDriverProvider()
{
	FGPUDriverInfo DriverInfo = FPlatformMisc::GetGPUDriverInfo(FPlatformMisc::GetPrimaryGPUBrand());
	return DriverInfo.ProviderName;
}

bool UHardwareInfoLibrary::IsGPUSchedulingEnabled()
{
#if PLATFORM_WINDOWS
	HKEY hKey = nullptr;
	const TCHAR* SubKey = TEXT("SYSTEM\\CurrentControlSet\\Control\\GraphicsDrivers");
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, SubKey, 0, KEY_READ, &hKey) == ERROR_SUCCESS)
	{
		DWORD Value = 0, Size = sizeof(Value);
		LSTATUS Status = RegQueryValueEx(hKey, TEXT("HwSchMode"), nullptr, nullptr,
			reinterpret_cast<LPBYTE>(&Value), &Size);
		RegCloseKey(hKey);
		return (Status == ERROR_SUCCESS && Value == 2);
	}
#endif
	return false;
}

bool UHardwareInfoLibrary::IsDLSSdllFound()
{
	return FPlatformProcess::GetDllHandle(TEXT("nvngx_dlss.dll")) != nullptr;
}

TMap<FString, bool> UHardwareInfoLibrary::GetPluginStatus()
{
	TMap<FString, bool> OutPluginStatus;

	const TSharedPtr<IPlugin> ThisPlugin = IPluginManager::Get().FindPlugin(TEXT("UpscalingMenu"));
	if (!ThisPlugin.IsValid())
		return OutPluginStatus;

	FString JsonString;
	if (!FFileHelper::LoadFileToString(JsonString, *ThisPlugin->GetDescriptorFileName()))
		return OutPluginStatus;

	TSharedPtr<FJsonObject> JsonObject;
	const TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);
	if (!FJsonSerializer::Deserialize(Reader, JsonObject) || !JsonObject.IsValid())
		return OutPluginStatus;

	const TArray<TSharedPtr<FJsonValue>>* PluginsArray;
	if (!JsonObject->TryGetArrayField(TEXT("Plugins"), PluginsArray))
		return OutPluginStatus;

	TSet<FString> EnabledNames;
	for (const TSharedRef<IPlugin>& Plugin : IPluginManager::Get().GetEnabledPlugins())
		EnabledNames.Add(Plugin->GetName());

	for (const TSharedPtr<FJsonValue>& Val : *PluginsArray)
	{
		const TSharedPtr<FJsonObject>* Obj;
		FString Name;
		if (Val->TryGetObject(Obj) && (*Obj)->TryGetStringField(TEXT("Name"), Name))
			OutPluginStatus.Add(MoveTemp(Name), EnabledNames.Contains(Name));
	}

	return OutPluginStatus;
}

FHardwareDetails UHardwareInfoLibrary::GetParsedHardwareData_Internal()
{
	FHardwareDetails Info;
	Info.OperatingSystem = GetOperatingSystemInfo();
	Info.OSBuild = GetOSBuildInfo();
	Info.bIsGPUSchedulingEnabled = IsGPUSchedulingEnabled();
	Info.FullGPUName = FPlatformMisc::GetPrimaryGPUBrand();
	Info.CompanyName = TEXT("Unknown");
	Info.SeriesNumber = 0;
	Info.bFoundSeries = false;

	const FString Lower = Info.FullGPUName.ToLower();

	static const TPair<FString, FString> VendorMap[] =
	{
		{TEXT("nvidia"), TEXT("NVIDIA")},
		{TEXT("geforce"), TEXT("NVIDIA")},
		{TEXT("amd"), TEXT("AMD")},
		{TEXT("radeon"), TEXT("AMD")},
		{TEXT("intel"), TEXT("Intel")},
		{TEXT("arc"), TEXT("Intel")}
	};

	for (const auto& It : VendorMap)
	{
		if (Lower.Contains(It.Key))
		{
			Info.CompanyName = It.Value;
			break;
		}
	}

	static const FRegexPattern Pattern(TEXT("(\\d+)"), ERegexPatternFlags::CaseInsensitive);
	FRegexMatcher Matcher(Pattern, Info.FullGPUName);
	if (Matcher.FindNext())
	{
		int32 FullNumber = FCString::Atoi(*Matcher.GetCaptureGroup(1));
		if (FullNumber >= 1000)
		{
			Info.SeriesNumber = (FullNumber / 1000) * 1000;
			Info.bFoundSeries = true;
		}
		else if (FullNumber >= 100)
		{
			Info.SeriesNumber = (FullNumber / 100) * 100;
			Info.bFoundSeries = true;
		}
	}

	Info.GPUDriverVersion = GetGPUDriverVersion();
	Info.GPUDriverInternalVersion = GetGPUDriverInternalVersion();
	Info.GPUDriverProvider = GetGPUDriverProvider();
	Info.bIsDLSSdllFound = IsDLSSdllFound();
	Info.PluginStatus = GetPluginStatus();
	Info.RHIBackend = GetCurrentRHIBackend();
	Info.RHIFeatureLevel = GetCurrentRHIFeatureLevel();

	return Info;
}