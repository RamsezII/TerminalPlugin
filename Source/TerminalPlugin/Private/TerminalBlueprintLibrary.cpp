#include "TerminalBlueprintLibrary.h"
#include "TerminalUISubsystem.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/PlayerController.h"

static UTerminalUISubsystem* GetTerminalSubsysFromPC(APlayerController* PC)
{
    if (!PC) return nullptr;
    if (ULocalPlayer* LP = Cast<ULocalPlayer>(PC->Player))
    {
        return LP->GetSubsystem<UTerminalUISubsystem>();
    }
    return nullptr;
}

void UTerminalBlueprintLibrary::ToggleTerminalForPC(APlayerController* PC)
{
    if (auto* Subsys = GetTerminalSubsysFromPC(PC))
    {
        Subsys->ToggleTerminal();
    }
}

void UTerminalBlueprintLibrary::ShowTerminalForPC(APlayerController* PC)
{
    if (auto* Subsys = GetTerminalSubsysFromPC(PC))
    {
        Subsys->ShowTerminal();
    }
}

void UTerminalBlueprintLibrary::HideTerminalForPC(APlayerController* PC)
{
    if (auto* Subsys = GetTerminalSubsysFromPC(PC))
    {
        Subsys->HideTerminal();
    }
}
