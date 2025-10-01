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
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	UFUNCTION() void ToggleTerminal();

private:
	// ----- Assets chargés depuis le contenu du plugin
	UPROPERTY() TSubclassOf<class UUserWidget> TerminalWidgetClass = nullptr;
	UPROPERTY() class UInputAction* IA_Toggle = nullptr;
	UPROPERTY() class UInputMappingContext* IMC_Terminal = nullptr;

	// ----- Instances runtime
	UPROPERTY(Transient) class UUserWidget* TerminalWidget = nullptr;
	UPROPERTY(Transient) class UEnhancedInputComponent* PluginInputComponent = nullptr;

	// ----- Helpers
	void BindInput();
	void ApplyInputMode(bool bUI);

	template<typename T> T* LoadObj(const TCHAR* Path)
	{
		return Cast<T>(StaticLoadObject(T::StaticClass(), nullptr, Path));
	}

	template<typename T> TSubclassOf<T> LoadClassObj(const TCHAR* Path)
	{
		return TSubclassOf<T>(StaticLoadClass(T::StaticClass(), nullptr, Path));
	}
};
