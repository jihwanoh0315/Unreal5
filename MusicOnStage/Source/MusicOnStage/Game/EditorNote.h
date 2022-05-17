// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "EditorNote.generated.h"


UCLASS()
class MUSICONSTAGE_API AEditorNote : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AEditorNote();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 m_pressedSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 m_line;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 m_zPos;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool m_isLongNote;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool m_isGrabbed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector m_prevPos;

public:

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
		FVector SetLine(FVector cursorPos_);

	UFUNCTION(BlueprintCallable)
	void SetPreviousPos(FVector prevPos_);

	UFUNCTION(BlueprintCallable)
		void SetIsGrabbed(bool boolean_);
};
