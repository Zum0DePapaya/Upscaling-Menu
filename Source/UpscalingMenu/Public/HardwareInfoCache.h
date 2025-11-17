// ZumoDePapaya 2025 All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"
#include "HardwareInfoLibrary.h"

class FHardwareInfoCache : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;

    static const FHardwareDetails& GetCachedHardwareInfo();

private:
    static FHardwareDetails CachedHardwareInfo;
    static bool bIsInitialized;

    void InitializeHardwareInfo();
};