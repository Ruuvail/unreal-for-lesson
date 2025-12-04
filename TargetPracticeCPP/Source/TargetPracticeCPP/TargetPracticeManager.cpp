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
	if (!PlayerControllerRef)
	{
		UE_LOG(LogTemp, Warning, TEXT("TargetPracticeManager: PlayerController not found."));
	}

	ShowMainMenu();
}

void ATargetPracticeManager::SwitchTo(TSubclassOf<UUserWidget> WidgetClass, ETPFlow NewState)
{
	// remove previous widget
	if (CurrentWidget)
	{
		CurrentWidget->RemoveFromParent();
		CurrentWidget = nullptr;
	}

	// create and add new widget
	if (WidgetClass)
	{
		if (UWorld* World = GetWorld())
		{
			CurrentWidget = CreateWidget<UUserWidget>(World, WidgetClass);
			if (CurrentWidget)
			{
				CurrentWidget->AddToViewport();
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("TargetPracticeManager: SwitchTo called with null WidgetClass."));
	}

	State = NewState;

	// make sure controller ref exists (safety in case BeginPlay failed)
	if (!PlayerControllerRef)
	{
		PlayerControllerRef = UGameplayStatics::GetPlayerController(this, 0);
	}

	// input mode & mouse cursor
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
			if (CurrentWidget)
			{
				Mode.SetWidgetToFocus(CurrentWidget->TakeWidget());
			}
			PlayerControllerRef->SetInputMode(Mode);
		}
	}
}

void ATargetPracticeManager::ShowMainMenu()
{
	// stop timers
	GetWorldTimerManager().ClearTimer(TimerHandle_GameTick);
	GetWorldTimerManager().ClearTimer(TimerHandle_SpawnTick);

	// reset runtime state
	TimeRemaining = 0;
	Score = 0;

	SwitchTo(MainMenuClass, ETPFlow::MainMenu);
}

void ATargetPracticeManager::StartGame()
{
	TimeRemaining = RoundSeconds;
	Score = 0;

	SwitchTo(GameUIClass, ETPFlow::Playing);

	// countdown timer
	GetWorldTimerManager().SetTimer(
		TimerHandle_GameTick, this, &ATargetPracticeManager::GameTick, 1.0f, true);

	// spawn timer
	if (TargetClass)
	{
		GetWorldTimerManager().SetTimer(
			TimerHandle_SpawnTick, this, &ATargetPracticeManager::SpawnTick, SpawnInterval, true);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("TargetPracticeManager: TargetClass is not set, no targets will spawn."));
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
		UE_LOG(LogTemp, Warning, TEXT("TargetPracticeManager: SpawnTick called but TargetClass is null."));
		return;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	for (int32 i = 0; i < SpawnPerTick; ++i)
	{
		const FVector Location = GetRandomPointInArea();
		const FRotator Rotation = FRotator::ZeroRotator;

		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		World->SpawnActor<AActor>(TargetClass, Location, Rotation, Params);
	}
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
