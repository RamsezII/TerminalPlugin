// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/VerticalBox.h"
#include "Components/EditableTextBox.h"

#include "TerminalWindowUI.generated.h"


class UEditableTextBox;
class UVerticalBox;

/**
 * 
 */
UCLASS()
class TERMINALPLUGIN_API UTerminalWindowUI : public UUserWidget
{
	GENERATED_BODY()
	

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UEditableTextBox* InputBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UVerticalBox* VerticalBox;

	void AppendLine();
};
