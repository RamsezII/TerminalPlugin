#include "TerminalUISubsystem.h"
#include "Blueprint/UserWidget.h"
#include "Engine/LocalPlayer.h"
#include "UObject/ConstructorHelpers.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "InputAction.h"


void UTerminalUISubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// Cherche ton widget Blueprint dans le Content du plugin
	static ConstructorHelpers::FClassFinder<UUserWidget> TerminalWidgetBP(
		TEXT("/TerminalPlugin/UI/WBP_Terminal")
	);

	if (TerminalWidgetBP.Succeeded())
	{
		TerminalWidgetClass = TerminalWidgetBP.Class;
	}

	// Charge ton IMC et IA
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> IMC_Terminal(
		TEXT("/TerminalPlugin/Inputs/IMC_Terminal.IMC_Terminal")
	);
	static ConstructorHelpers::FObjectFinder<UInputAction> IA_Toggle(
		TEXT("/TerminalPlugin/Inputs/IA_ToggleTerminal.IA_ToggleTerminal")
	);

	if (IMC_Terminal.Succeeded() && IA_Toggle.Succeeded())
	{
		if (ULocalPlayer* LP = GetLocalPlayer())
		{
			if (UEnhancedInputLocalPlayerSubsystem* InputSubsystem =
				LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
			{
				InputSubsystem->AddMappingContext(IMC_Terminal.Object, 1);

				// Bind l’action IA_ToggleTerminal -> ToggleTerminal()
				if (APlayerController* PC = LP->GetPlayerController(LP->GetWorld()))
				{
					if (UEnhancedInputComponent* EIC =
						Cast<UEnhancedInputComponent>(PC->InputComponent))
					{
						EIC->BindAction(IA_Toggle.Object, ETriggerEvent::Triggered, this,
							&UTerminalUISubsystem::ToggleTerminal);
					}
				}
			}
		}
	}
}

void UTerminalUISubsystem::Deinitialize()
{
	if (TerminalInstance)
	{
		TerminalInstance->RemoveFromParent();
		TerminalInstance = nullptr;
	}

	Super::Deinitialize();
}

void UTerminalUISubsystem::ToggleTerminal()
{
	if (!TerminalWidgetClass) return;

	ULocalPlayer* LP = GetLocalPlayer();
	if (!LP) return;

	UWorld* World = LP->GetWorld();
	if (!World) return;

	if (!TerminalInstance)
	{
		TerminalInstance = CreateWidget<UUserWidget>(World, TerminalWidgetClass);
	}

	if (bIsVisible)
	{
		TerminalInstance->RemoveFromParent();
		bIsVisible = false;
	}
	else
	{
		TerminalInstance->AddToViewport(100); // Z-order haut
		bIsVisible = true;
	}
}
