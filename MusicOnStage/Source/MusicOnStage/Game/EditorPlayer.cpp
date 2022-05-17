// Fill out your copyright notice in the Description page of Project Settings.


#include "EditorPlayer.h"
#include "EditorNote.h"
#include "EditorWidget.h"
#include "Engine.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraActor.h"
#include "PaperSpriteActor.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h" //DeprojectMouse
#include "Blueprint/UserWidget.h"

// Sets default values
AEditorPlayer::AEditorPlayer() : m_grabMode(false), m_isGrabbed(false), m_noteCreated(false)
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AEditorPlayer::BeginPlay()
{
	Super::BeginPlay();
	m_playerController = UGameplayStatics::GetPlayerController(this, 0);

	/***********************************
		For note Edit Section	
	************************************/
	// Set Camera to TopDown
	TArray<AActor*> loopActors;
	
	TSubclassOf<class ACameraActor> FindClassType = ACameraActor::StaticClass();
	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), FindClassType, "TopDownCam", loopActors);
	if (loopActors.IsEmpty())
	{
		GEngine->AddOnScreenDebugMessage(1, 10.0f, FColor::Red, FString("ERROR : THERE IS NO SELECTED NOTE BORDER"));
	}

	for (AActor* camera : loopActors)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Orange, FString("Find Camera : " + camera->GetName()));
		m_playerController->SetViewTarget(camera);
	}

	TSubclassOf<class APaperSpriteActor> FindClassType2 = APaperSpriteActor::StaticClass();
	// Set Note Border of selected
	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), FindClassType2,"NoteSelected", loopActors);

	if (loopActors.IsEmpty())
	{
		GEngine->AddOnScreenDebugMessage(1, 10.0f, FColor::Red, FString("ERROR : THERE IS NO SELECTED NOTE BORDER"));
	}
	for (AActor* border : loopActors)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Orange, FString("Find Border : " + border->GetName()));
		m_selectedBorder = border;
	}

	/***********************************
		For Widget Section
	************************************/
	if (m_editorWidgetClass)
	{
		m_editorWidget = Cast<UEditorWidget>(CreateWidget(GetWorld(), m_editorWidgetClass));

		if (m_editorWidget)
		{
			m_editorWidget->AddToViewport();
		}
	}

	GrabMode();

}

// Called every frame
void AEditorPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MoveNote();
}

// Called to bind functionality to input
void AEditorPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("GrabMode", IE_Pressed, this, &AEditorPlayer::GrabMode);
	PlayerInputComponent->BindAction("MouseLeftClick", IE_Pressed, this, &AEditorPlayer::Grab);
	PlayerInputComponent->BindAction("MouseLeftClick", IE_Released, this, &AEditorPlayer::Drop);
}

void AEditorPlayer::MoveNote()
{
	if (m_grabMode && m_isGrabbed)
	{
		// Do the ray-tracing to pick note
		FVector mouseLocation;
		FVector mouseDirection;

		m_playerController->DeprojectMousePositionToWorld(mouseLocation, mouseDirection);

		FVector start = mouseLocation;
		FVector end = start + mouseDirection * 10000.0f;

		FCollisionQueryParams params;
		params.AddIgnoredActor(m_selectedBorder);

		FHitResult hit; // Hit result struct

		if (GetWorld()->LineTraceSingleByChannel(hit, start, end, ECollisionChannel::ECC_Visibility, params))
		{
			FVector newLocation = m_selectedNote->SetLine(hit.Location);
			m_selectedBorder->SetActorLocation(FVector(newLocation.X, 1.0, newLocation.Z));
		}
	}
}

void AEditorPlayer::GrabMode()
{
	m_grabMode = !m_grabMode;
	m_playerController->SetShowMouseCursor(m_grabMode);
}

void AEditorPlayer::Grab()
{
	// not grab but place note when new note was created
	if (m_noteCreated)
	{
		m_noteCreated = false;
		return;
	}

	// When Grab Mode is active
	if (!m_grabMode)
		return;

	// Do the ray-tracing to pick note
	FVector mouseLocation;
	FVector mouseDirection;

	m_playerController->DeprojectMousePositionToWorld(mouseLocation, mouseDirection);

	FVector start = mouseLocation;
	FVector end = start + mouseDirection * 10000.0f;

	FCollisionQueryParams params;
	params.AddIgnoredActor(m_selectedBorder);

	FHitResult hit; // Hit result struct


	if (GetWorld()->LineTraceSingleByChannel(hit, start, end, ECollisionChannel::ECC_Visibility, params))
	{
		AActor* hitNote = hit.GetActor();
		if (hitNote->IsRootComponentMovable())
		{
			m_isGrabbed = true;
			// Set grabbed note
			m_selectedNote = Cast<AEditorNote>(hitNote);
			m_grabbedLocation = m_selectedNote->GetActorLocation();
			m_selectedNote->SetPreviousPos(m_grabbedLocation);
			m_selectedNote->SetIsGrabbed(true);
		}
	}

}

void AEditorPlayer::Drop()
{
	if (!m_isGrabbed)
		return;

	m_isGrabbed = false;

	if (m_selectedNote)
	{
		m_selectedNote->SetIsGrabbed(false);
	}

	// Have to return to previous position when try to move on the other notes
	for (auto& currBar : m_selectedNote->Tags)
	{
		TArray<AActor*> loopActors;
		TSubclassOf<class AEditorNote> FindClassType = AEditorNote::StaticClass();
		UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), FindClassType, currBar, loopActors);
		for (AActor* note : loopActors)
		{
			if (note != m_selectedNote)
			{
				//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Orange, FString("Check Overlap : " + note->GetName() + " && " + m_selectedNote->GetName()));
				if (m_selectedNote->GetActorLocation() == note->GetActorLocation())
				{
					m_selectedNote->SetActorLocation(m_grabbedLocation);
					m_selectedBorder->SetActorLocation(FVector(m_grabbedLocation.X, 1.0, m_grabbedLocation.Z));
				}
			}
		}
	}
}


