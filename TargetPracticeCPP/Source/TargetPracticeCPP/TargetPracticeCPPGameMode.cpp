#include "TargetPracticeCPPGameMode.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"

ATargetPracticeCPPGameMode::ATargetPracticeCPPGameMode()
	: Super()
{
	// Set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(
		TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));

	if (PlayerPawnClassFinder.Succeeded())
	{
		DefaultPawnClass = PlayerPawnClassFinder.Class;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("GameMode: Failed to find BP_FirstPersonCharacter."));
	}

	// Main menu widget class
	static ConstructorHelpers::FClassFinder<UUserWidget> MainMenuWidgetClass(
		TEXT("/Game/UI/WBP_MainMenu"));
	if (MainMenuWidgetClass.Succeeded())
	{
		MainMenuClass = MainMenuWidgetClass.Class;
	}

	// In-game HUD widget class
	static ConstructorHelpers::FClassFinder<UUserWidget> GameUIWidgetClass(
		TEXT("/Game/UI/WBP_GameUI"));
	if (GameUIWidgetClass.Succeeded())
	{
		GameUIClass = GameUIWidgetClass.Class;
	}

	// Game Over widget class
	static ConstructorHelpers::FClassFinder<UUserWidget> GameOverWidgetClass(
		TEXT("/Game/UI/WBP_GameOver"));
	if (GameOverWidgetClass.Succeeded())
	{
		GameOverUIClass = GameOverWidgetClass.Class;
	}
}

void ATargetPracticeCPPGameMode::BeginPlay()
{
	Super::BeginPlay();

	// Start with the main menu
	ShowMainMenu();
}

void ATargetPracticeCPPGameMode::SwitchToWidget(TSubclassOf<UUserWidget> WidgetClass)
{
	// Remove previous widget if it exists
	if (CurrentWidget)
	{
		CurrentWidget->RemoveFromParent();
		CurrentWidget = nullptr;
	}

	if (!WidgetClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("GameMode: SwitchToWidget called with null WidgetClass."));
		return;
	}

	if (UWorld* World = GetWorld())
	{
		CurrentWidget = CreateWidget<UUserWidget>(World, WidgetClass);
		if (CurrentWidget)
		{
			CurrentWidget->AddToViewport();
		}
	}
}

void ATargetPracticeCPPGameMode::ShowMainMenu()
{
	SwitchToWidget(MainMenuClass);

	if (UWorld* World = GetWorld())
	{
		if (APlayerController* PC = World->GetFirstPlayerController())
		{
			PC->bShowMouseCursor = true;

			FInputModeUIOnly InputMode;
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			if (CurrentWidget)
			{
				InputMode.SetWidgetToFocus(CurrentWidget->TakeWidget());
			}
			PC->SetInputMode(InputMode);
		}
	}
}

void ATargetPracticeCPPGameMode::StartGame()
{
	SwitchToWidget(GameUIClass);

	if (UWorld* World = GetWorld())
	{
		if (APlayerController* PC = World->GetFirstPlayerController())
		{
			PC->bShowMouseCursor = false;

			FInputModeGameOnly InputMode;
			PC->SetInputMode(InputMode);
		}
	}
}

void ATargetPracticeCPPGameMode::ShowGameOver()
{
	SwitchToWidget(GameOverUIClass);

	if (UWorld* World = GetWorld())
	{
		if (APlayerController* PC = World->GetFirstPlayerController())
		{
			PC->bShowMouseCursor = true;

			FInputModeUIOnly InputMode;
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			if (CurrentWidget)
			{
				InputMode.SetWidgetToFocus(CurrentWidget->TakeWidget());
			}
			PC->SetInputMode(InputMode);
		}
	}
}
