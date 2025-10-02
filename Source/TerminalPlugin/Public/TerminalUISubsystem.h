#pragma once

#include "CoreMinimal.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "Blueprint/UserWidget.h"
#include "TerminalUISubsystem.generated.h"

/**
 * Subsystem léger : affiche/masque WBP_Terminal.
 * Aucun binding d'input ici — chaque projet choisit comment appeler ToggleTerminal().
 */
UCLASS()
class TERMINALPLUGIN_API UTerminalUISubsystem : public ULocalPlayerSubsystem
{
	GENERATED_BODY()

public:
	// API publique
	UFUNCTION(BlueprintCallable, Category = "Terminal")
	void ToggleTerminal();

	UFUNCTION(BlueprintCallable, Category = "Terminal")
	void ShowTerminal();

	UFUNCTION(BlueprintCallable, Category = "Terminal")
	void HideTerminal();

	UFUNCTION(BlueprintPure, Category = "Terminal")
	bool IsTerminalVisible() const { return TerminalWidget != nullptr; }

protected:
	virtual void Deinitialize() override;

private:
	// Classe du widget terminal (chargée à la demande)
	UPROPERTY() TSubclassOf<class UUserWidget> TerminalWidgetClass = nullptr;

	// Instance runtime
	UPROPERTY(Transient) class UUserWidget* TerminalWidget = nullptr;

	// Helpers
	void EnsureWidgetClassLoaded();
	void ApplyInputMode(bool bUI);

	template<typename T> TSubclassOf<T> LoadClassObj(const TCHAR* Path) const
	{
		return TSubclassOf<T>(StaticLoadClass(T::StaticClass(), nullptr, Path));
	}
};
