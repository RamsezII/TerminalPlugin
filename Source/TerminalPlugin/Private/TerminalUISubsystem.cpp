#include "TerminalUISubsystem.h"
#include "Blueprint/UserWidget.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/PlayerController.h"



// Adapte ce chemin si tu renommes/déplaces ton BP
static const TCHAR* PATH_WBP = TEXT("/TerminalPlugin/UI/WBP_Terminal.WBP_Terminal_C");

void UTerminalUISubsystem::EnsureWidgetClassLoaded()
{
    if (!TerminalWidgetClass)
    {
        TerminalWidgetClass = LoadClassObj<UUserWidget>(PATH_WBP);
        if (!TerminalWidgetClass)
        {
            UE_LOG(LogTemp, Error, TEXT("[Terminal] Widget class introuvable : %s"), PATH_WBP);
        }
    }
}

void UTerminalUISubsystem::Deinitialize()
{
    HideTerminal();
    Super::Deinitialize();
}

void UTerminalUISubsystem::ToggleTerminal()
{
    if (IsTerminalVisible()) { HideTerminal(); }
    else { ShowTerminal(); }
}

void UTerminalUISubsystem::ShowTerminal()
{
    EnsureWidgetClassLoaded();
    if (!TerminalWidgetClass) return;

    if (ULocalPlayer* LP = GetLocalPlayer())
        if (APlayerController* PC = LP->GetPlayerController(GetWorld()))
        {
            if (!TerminalWidget)
            {
                TerminalWidget = CreateWidget<UUserWidget>(PC, TerminalWidgetClass);
            }

            if (TerminalWidget && !TerminalWidget->IsInViewport())
            {
                TerminalWidget->AddToViewport(1000);
                ApplyInputMode(true);
            }
        }
}

void UTerminalUISubsystem::HideTerminal()
{
    if (TerminalWidget)
    {
        if (TerminalWidget->IsInViewport())
        {
            TerminalWidget->RemoveFromParent();
        }
        TerminalWidget = nullptr;
    }
    ApplyInputMode(false);
}

void UTerminalUISubsystem::ApplyInputMode(bool bUI)
{
    if (ULocalPlayer* LP = GetLocalPlayer())
        if (APlayerController* PC = LP->GetPlayerController(GetWorld()))
        {
            PC->bShowMouseCursor = bUI;

            if (bUI)
            {
                FInputModeUIOnly Mode;
                Mode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
                Mode.SetWidgetToFocus(TerminalWidget ? TerminalWidget->TakeWidget() : TSharedPtr<SWidget>());
                PC->SetInputMode(Mode);
            }
            else
            {
                PC->SetInputMode(FInputModeGameOnly{});
            }
        }
}
