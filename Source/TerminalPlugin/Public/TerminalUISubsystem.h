#pragma once

#include "CoreMinimal.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "Blueprint/UserWidget.h"
#include "TerminalUISubsystem.generated.h"

UCLASS()
class TERMINALPLUGIN_API UTerminalUISubsystem : public ULocalPlayerSubsystem
{
    GENERATED_BODY()

public:
    // Crée et affiche le terminal
    UFUNCTION(BlueprintCallable)
    void ToggleTerminal();

private:
    // Classe du widget (Blueprint que tu as fait -> WBP_Terminal)
    TSubclassOf<UUserWidget> TerminalWidgetClass;

    // Instance courante
    UPROPERTY()
    UUserWidget* TerminalInstance;

    bool bIsVisible = false;

protected:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;
};
