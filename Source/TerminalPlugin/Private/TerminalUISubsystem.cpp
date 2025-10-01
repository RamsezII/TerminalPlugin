#include "TerminalUISubsystem.h"

#include "Blueprint/UserWidget.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/PlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "InputMappingContext.h"

// ---- Chemins: tu as mis "Inputs" (avec un s) dans ton plugin
static const TCHAR* PATH_WBP = TEXT("/TerminalPlugin/UI/WBP_Terminal.WBP_Terminal_C");
static const TCHAR* PATH_IA = TEXT("/TerminalPlugin/Inputs/IA_ToggleTerminal.IA_ToggleTerminal");
static const TCHAR* PATH_IMC = TEXT("/TerminalPlugin/Inputs/IMC_Terminal.IMC_Terminal");

void UTerminalUISubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    // Charger les assets du plugin (runtime-safe)
    TerminalWidgetClass = LoadClassObj<UUserWidget>(PATH_WBP);
    IA_Toggle = LoadObj<UInputAction>(PATH_IA);
    IMC_Terminal = LoadObj<UInputMappingContext>(PATH_IMC);

    if (!TerminalWidgetClass)
        UE_LOG(LogTemp, Error, TEXT("[Terminal] Widget introuvable: %s"), PATH_WBP);
    if (!IA_Toggle)
        UE_LOG(LogTemp, Error, TEXT("[Terminal] IA introuvable: %s"), PATH_IA);
    if (!IMC_Terminal)
        UE_LOG(LogTemp, Error, TEXT("[Terminal] IMC introuvable: %s"), PATH_IMC);

    if (ULocalPlayer* LP = GetLocalPlayer())
    {
        // Ajouter l’Input Mapping Context (Enhanced Input)
        if (UEnhancedInputLocalPlayerSubsystem* EIS = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
        {
            if (IMC_Terminal)
            {
                EIS->AddMappingContext(IMC_Terminal, /*Priority*/1000);
                UE_LOG(LogTemp, Log, TEXT("[Terminal] IMC ajouté (prio 1000)"));
            }
        }

        if (APlayerController* PC = LP->GetPlayerController(GetWorld()))
        {
            // Premier bind immédiat
            BindInput();

            // Option "filet de sécurité" : rebinder au tick suivant
            if (UWorld* W = PC->GetWorld())
            {
                FTimerHandle Tmp;
                W->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateUObject(this, &UTerminalUISubsystem::BindInput));
            }
        }
    }
}

void UTerminalUISubsystem::Deinitialize()
{
    if (ULocalPlayer* LP = GetLocalPlayer())
        if (APlayerController* PC = LP->GetPlayerController(GetWorld()))
        {
            if (PluginInputComponent && PluginInputComponent != PC->InputComponent)
            {
                PC->PopInputComponent(PluginInputComponent);
            }
            PluginInputComponent = nullptr;
        }

    if (TerminalWidget)
    {
        TerminalWidget->RemoveFromParent();
        TerminalWidget = nullptr;
    }

    Super::Deinitialize();
}

void UTerminalUISubsystem::BindInput()
{
    if (!IA_Toggle) return;

    if (ULocalPlayer* LP = GetLocalPlayer())
        if (APlayerController* PC = LP->GetPlayerController(GetWorld()))
        {
            // S’il y a déjà un EnhancedInputComponent sur le PC, utilise-le
            if (!PluginInputComponent)
            {
                if (UEnhancedInputComponent* Existing = Cast<UEnhancedInputComponent>(PC->InputComponent))
                {
                    PluginInputComponent = Existing;
                    UE_LOG(LogTemp, Log, TEXT("[Terminal] Bind sur EnhancedInputComponent existant"));
                }
                else
                {
                    // Sinon, on crée et push notre propre EnhancedInputComponent
                    PluginInputComponent = NewObject<UEnhancedInputComponent>(PC, TEXT("TerminalPluginInputComponent"));
                    PluginInputComponent->RegisterComponent();
                    PC->PushInputComponent(PluginInputComponent);
                    UE_LOG(LogTemp, Log, TEXT("[Terminal] EnhancedInputComponent créé+push"));
                }
            }

            // (re)bind propre
            PluginInputComponent->ClearActionBindings();
            PluginInputComponent->BindAction(IA_Toggle, ETriggerEvent::Started, this, &UTerminalUISubsystem::ToggleTerminal);
            UE_LOG(LogTemp, Log, TEXT("[Terminal] Action bindée (P -> ToggleTerminal)"));
        }
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

void UTerminalUISubsystem::ToggleTerminal()
{
    UE_LOG(LogTemp, Log, TEXT("[Terminal] ToggleTerminal()"));

    if (!TerminalWidgetClass) return;

    if (ULocalPlayer* LP = GetLocalPlayer())
        if (APlayerController* PC = LP->GetPlayerController(GetWorld()))
        {
            if (TerminalWidget)
            {
                TerminalWidget->RemoveFromParent();
                TerminalWidget = nullptr;
                ApplyInputMode(false);
                return;
            }

            TerminalWidget = CreateWidget<UUserWidget>(PC, TerminalWidgetClass);
            if (TerminalWidget)
            {
                TerminalWidget->AddToViewport(1000);
                ApplyInputMode(true);
            }
        }
}
