#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "TerminalBlueprintLibrary.generated.h"

/**
 * Facilité Blueprint : appelle le Subsystem depuis n’importe quel graph.
 */
UCLASS()
class TERMINALPLUGIN_API UTerminalBlueprintLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Terminal")
    static void ToggleTerminalForPC(class APlayerController* PC);

    UFUNCTION(BlueprintCallable, Category = "Terminal")
    static void ShowTerminalForPC(class APlayerController* PC);

    UFUNCTION(BlueprintCallable, Category = "Terminal")
    static void HideTerminalForPC(class APlayerController* PC);
};
