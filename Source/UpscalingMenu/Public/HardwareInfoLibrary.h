// ZumoDePapaya 2025 All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "HardwareInfoLibrary.generated.h"

USTRUCT(BlueprintType)
struct FHardwareDetails
{
    GENERATED_BODY()

    // System Info
    UPROPERTY(BlueprintReadOnly, Category = "System")
    FString OperatingSystem;

    UPROPERTY(BlueprintReadOnly, Category = "System")
    FString OSBuild;

    UPROPERTY(BlueprintReadOnly, Category = "System")
    bool bIsGPUSchedulingEnabled = false;

    // GPU Info
    UPROPERTY(BlueprintReadOnly, Category = "GPU")
    FString CompanyName;

    UPROPERTY(BlueprintReadOnly, Category = "GPU")
    FString FullGPUName;

    UPROPERTY(BlueprintReadOnly, Category = "GPU")
    int32 SeriesNumber = 0;

    UPROPERTY(BlueprintReadOnly, Category = "GPU")
    bool bFoundSeries = false;

    UPROPERTY(BlueprintReadOnly, Category = "GPU")
    FString GPUDriverVersion;

    UPROPERTY(BlueprintReadOnly, Category = "GPU")
    FString GPUDriverInternalVersion;

    UPROPERTY(BlueprintReadOnly, Category = "GPU")
    FString GPUDriverProvider;

    UPROPERTY(BlueprintReadOnly, Category = "GPU", meta = (DisplayName = "DLSS.dll Found"))
    bool bIsDLSSdllFound = false;

    // Plugin Info
    UPROPERTY(BlueprintReadOnly, Category = "Plugins")
    TMap<FString, bool> PluginStatus;

    // Rendering Info
    UPROPERTY(BlueprintReadOnly, Category = "Rendering")
    FString RHIBackend;

    UPROPERTY(BlueprintReadOnly, Category = "Rendering")
    FString RHIFeatureLevel;
};

UCLASS()
class UPSCALINGMENU_API UHardwareInfoLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintPure, meta = (DisplayName = "Get Hardware Info"), Category = "Hardware Info")
    static FHardwareDetails GetHardwareInfo();

    static FHardwareDetails GetParsedHardwareData_Internal();

    UFUNCTION(BlueprintPure, Category = "Rendering")
    static FString GetCurrentRHIFeatureLevel();

    UFUNCTION(BlueprintPure, Category = "Hardware Info")
    static bool IsCVarRegistered(FString CVarName);

    UFUNCTION(BlueprintPure, Category = "System Info")
    static FString GetOperatingSystemInfo();

    UFUNCTION(BlueprintPure, Category = "System Info")
    static FString GetOSBuildInfo();

    UFUNCTION(BlueprintPure, Category = "Rendering")
    static FString GetCurrentRHIBackend();

    UFUNCTION(BlueprintPure, Category = "GPU Info")
    static FString GetGPUDriverVersion();

    UFUNCTION(BlueprintPure, Category = "GPU Info")
    static FString GetGPUDriverInternalVersion();

    UFUNCTION(BlueprintPure, Category = "GPU Info")
    static FString GetGPUDriverProvider();

    UFUNCTION(BlueprintPure, Category = "System Info")
    static bool IsGPUSchedulingEnabled();

    UFUNCTION(BlueprintPure, meta = (DisplayName = "Is DLSS.dll Found"), Category = "GPU Info")
    static bool IsDLSSdllFound();

    UFUNCTION(BlueprintCallable, Category = "Hardware Info", meta = (DisplayName = "Get Plugin Status"))
    static TMap<FString, bool> GetPluginStatus();
};