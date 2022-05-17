// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EditorWidget.generated.h"

/**
 * 
 */
UCLASS()
class MUSICONSTAGE_API UEditorWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativePreConstruct() override;

	UPROPERTY(Meta = (BindWidget))
		class UComboBoxString* NoteTypeComboBox;
	UPROPERTY(Meta = (BindWidget))
		class UEditableText* Line;
};
