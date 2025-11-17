// ZumoDePapaya 2025 All Rights Reserved

#include "HardwareInfoCache.h"
#include "Misc/CoreDelegates.h"
#include "Modules/ModuleManager.h"

FHardwareDetails FHardwareInfoCache::CachedHardwareInfo;
bool FHardwareInfoCache::bIsInitialized = false;

void FHardwareInfoCache::StartupModule()
{
    FCoreDelegates::OnPostEngineInit.AddRaw(this, &FHardwareInfoCache::InitializeHardwareInfo);
}

void FHardwareInfoCache::ShutdownModule()
{
    FCoreDelegates::OnPostEngineInit.RemoveAll(this);
    bIsInitialized = false;
}

void FHardwareInfoCache::InitializeHardwareInfo()
{
    if (!bIsInitialized)
    {
        CachedHardwareInfo = UHardwareInfoLibrary::GetParsedHardwareData_Internal();
        bIsInitialized = true;
    }
}

const FHardwareDetails& FHardwareInfoCache::GetCachedHardwareInfo()
{
    if (!bIsInitialized)
    {
        CachedHardwareInfo = UHardwareInfoLibrary::GetParsedHardwareData_Internal();
        bIsInitialized = true;
    }
    return CachedHardwareInfo;
}

IMPLEMENT_MODULE(FHardwareInfoCache, UpscalingMenu)