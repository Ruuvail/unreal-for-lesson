#include "TargetPracticeManager.h"
#include "Components/BoxComponent.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/World.h"
#include "TimerManager.h"

ATargetPracticeManager::ATargetPracticeManager()
{
	PrimaryActorTick.bCanEverTick = false;

	SpawnArea = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnArea"));
	SetRootComponent(SpawnArea);
	SpawnArea->SetBoxExtent(FVector(600.f, 600.f, 300.f));
	SpawnArea->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ATargetPracticeManager::BeginPlay()
{
	Super::BeginPlay();

	PlayerControllerRef = UGameplayStatics::GetPlayerController(this, 0);
	ShowMainMenu();
}

void ATargetPracticeManager::SwitchTo(TSubclassOf<UUserWidget> WidgetClass, ETPFlow NewState)
{
	// Remove previous widget
	if (CurrentWidget)
	{
		CurrentWidget->RemoveFromParent();
		CurrentWidget = nullptr;
	}

	// Create and add new widget
	if (WidgetClass)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), WidgetClass);
		if (CurrentWidget)
		{
			CurrentWidget->AddToViewport();
		}
	}

	State = NewState;

	// Input mode & mouse cursor
	if (PlayerControllerRef)
	{
		if (State == ETPFlow::Playing)
		{
			PlayerControllerRef->bShowMouseCursor = false;
			PlayerControllerRef->SetInputMode(FInputModeGameOnly());
		}
		else
		{
			PlayerControllerRef->bShowMouseCursor = true;
			FInputModeUIOnly Mode;
			Mode.SetWidgetToFocus(CurrentWidget ? CurrentWidget->TakeWidget() : TSharedPtr<SWidget>());
			PlayerControllerRef->SetInputMode(Mode);
		}
	}
}

void ATargetPracticeManager::ShowMainMenu()
{
	// Clear runtime state
	GetWorldTimerManager().ClearTimer(TimerHandle_GameTick);
	GetWorldTimerManager().ClearTimer(TimerHandle_SpawnTick);

	TimeRemaining = 0;
	Score = 0;

	SwitchTo(MainMenuClass, ETPFlow::MainMenu);
}

void ATargetPracticeManager::StartGame()
{
	// Reset score/timer
	TimeRemaining = RoundSeconds;
	Score = 0;

	SwitchTo(GameUIClass, ETPFlow::Playing);

	// Start countdown
	GetWorldTimerManager().SetTimer(
		TimerHandle_GameTick, this, &ATargetPracticeManager::GameTick, 1.0f, true);

	// Start spawning
	if (TargetClass)
	{
		// NOTE: I am currently debugging the spawn system.
		// I only want to see the log output for the spawn locations for now,
		// so I disabled the actual spawning logic inside SpawnTick().
		GetWorldTimerManager().SetTimer(
			TimerHandle_SpawnTick, this, &ATargetPracticeManager::SpawnTick, SpawnInterval, true);
	}
}

void ATargetPracticeManager::ShowGameOver()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_GameTick);
	GetWorldTimerManager().ClearTimer(TimerHandle_SpawnTick);

	SwitchTo(GameOverClass, ETPFlow::GameOver);
}

void ATargetPracticeManager::GameTick()
{
	if (--TimeRemaining <= 0)
	{
		ShowGameOver();
	}
}

void ATargetPracticeManager::SpawnTick()
{
	if (!TargetClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("SpawnTick called but TargetClass is not set."));
		return;
	}

	// TODO: Temporarily disabled actual spawning because some targets were spawning
	// at weird locations outside the level. I will re-enable this loop after I fix
	// the bounding box math.
	//
	// Because of this, the timer still runs, but no targets will appear in the level.

	const FVector DebugLocation = GetRandomPointInArea();
	UE_LOG(LogTemp, Warning, TEXT("Debug SpawnTick location: %s"), *DebugLocation.ToString());

	/*
	for (int32 i = 0; i < SpawnPerTick; ++i)
	{
		const FVector Location = GetRandomPointInArea();
		const FRotator Rotation = FRotator::ZeroRotator;

		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		GetWorld()->SpawnActor<AActor>(TargetClass, Location, Rotation, Params);
	}
	*/
}

FVector ATargetPracticeManager::GetRandomPointInArea() const
{
	const FVector Origin = SpawnArea->Bounds.Origin;
	const FVector Extent = SpawnArea->Bounds.BoxExtent;
	return UKismetMathLibrary::RandomPointInBoundingBox(Origin, Extent);
}

void ATargetPracticeManager::AddScore(int32 Delta)
{
	if (State == ETPFlow::Playing && Delta != 0)
	{
		Score += Delta;
	}
}
