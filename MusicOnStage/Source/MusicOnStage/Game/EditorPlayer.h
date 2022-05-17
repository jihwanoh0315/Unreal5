// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "EditorPlayer.generated.h"

class UEditorWidget;
class AEditorNote;

UCLASS()
class MUSICONSTAGE_API AEditorPlayer : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AEditorPlayer();

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

public:	

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool m_grabMode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool m_isGrabbed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool m_noteCreated;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 m_currentMeasure;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AEditorNote* m_selectedNote;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector m_grabbedLocation;
	// Border of selected Note
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AActor* m_selectedBorder;
private:
	// Widget Variable
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widget", Meta = (AllowPrivateAccess = true))
		TSubclassOf<class UUserWidget> m_editorWidgetClass;
	UPROPERTY()
		UEditorWidget* m_editorWidget;


	// Note Drag Drop
	void MoveNote();
	void GrabMode();
	void Grab();
	void Drop();

	UPROPERTY()
		APlayerController* m_playerController;

};
