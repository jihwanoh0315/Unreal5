// Fill out your copyright notice in the Description page of Project Settings.


#include "EditorNote.h"
#include "PaperSpriteComponent.h"

// Sets default values
AEditorNote::AEditorNote()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void AEditorNote::BeginPlay()
{
	Super::BeginPlay();
}

// Called to bind functionality to input
void AEditorNote::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

FVector AEditorNote::SetLine(FVector cursorPos_)
{
	float cursorX = cursorPos_.X;
	int32 cursorZ = static_cast<int32>(cursorPos_.Z);
	FVector newPos(0.0f);

	if (0 <= cursorZ && cursorZ < 1008 &&
		-150.0f <= cursorX && cursorX < 150.0f)
	{
		// Set X
		if (-150.0f <= cursorX && cursorX < -90.0f)
		{
			newPos.X = -120.0f;
			m_line = 1;
		}
		else if (-90.0f <= cursorX && cursorX < -30.0f)
		{
			newPos.X = -60.0f;
			m_line = 2;
		}
		else if (-30.0f <= cursorX && cursorX < 30.0f)
		{
			newPos.X = 0.0f;
			m_line = 3;
		}
		else if (30.0f <= cursorX && cursorX < 90.0f)
		{
			newPos.X = 60.0f;
			m_line = 4;
		}
		else if (90.0f <= cursorX && cursorX < 150.0f)
		{
			newPos.X = 120.0f;
			m_line = 5;
		}

		// Set Z
		if (0 <= cursorZ && cursorZ < 1008)
		{
			int32 posGap = cursorZ % 32;
			m_zPos = cursorZ / 32;

			if (posGap <= 16)
			{
				newPos.Z = static_cast<float>(cursorZ - posGap);
			}
			else
			{
				newPos.Z = static_cast<float>(cursorZ + 32 - posGap);
				++m_zPos;
			}
		}

		SetActorLocation(newPos);
		return newPos;
	}
	return GetActorLocation();
}

void AEditorNote::SetPreviousPos(FVector prevPos_)
{
	m_prevPos = prevPos_;
}

void AEditorNote::SetIsGrabbed(bool boolean_)
{
	m_isGrabbed = boolean_;
}
