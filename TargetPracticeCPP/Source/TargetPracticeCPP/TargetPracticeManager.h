#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TargetPracticeManager.generated.h"

class UUserWidget;
class UBoxComponent;

UENUM(BlueprintType)
enum class ETPFlow : uint8
{
	MainMenu,
	Playing,
	GameOver
};

UCLASS()
class TARGETPRACTICECPP_API ATargetPracticeManager : public AActor
{
	GENERATED_BODY()

public:
	ATargetPracticeManager();

	// ---------- UI CLASSES ----------

	/** Main menu widget class */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> MainMenuClass;

	/** In-game HUD widget class */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> GameUIClass;

	/** Game over widget class */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> GameOverClass;

	// ---------- SPAWNING ----------

	/** Target to spawn (e.g., BP_TargetDummy) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
	TSubclassOf<AActor> TargetClass;

	/** Spawn area volume (random points generated inside this box) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
	UBoxComponent* SpawnArea;

	/** Seconds between spawn ticks */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning", meta = (ClampMin = "0.05"))
	float SpawnInterval = 1.5f;

	/** How many targets to spawn each tick */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning", meta = (ClampMin = "1"))
	int32 SpawnPerTick = 1;

	// ---------- GAME RULES ----------

	/** Total round duration in seconds */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game", meta = (ClampMin = "1"))
	int32 RoundSeconds = 60;

	// ---------- UI / GAME ACCESSORS ----------

	/** Current score for UI bindings */
	UFUNCTION(BlueprintCallable, Category = "Game")
	int32 GetScore() const { return Score; }

	/** Time remaining for UI bindings */
	UFUNCTION(BlueprintCallable, Category = "Game")
	int32 GetTimeRemaining() const { return TimeRemaining; }

	/** Called by targets (or hit logic) to add points */
	UFUNCTION(BlueprintCallable, Category = "Game")
	void AddScore(int32 Delta);

	// ---------- FLOW CONTROL (UI ENTRY POINTS) ----------

	UFUNCTION(BlueprintCallable, Category = "UI")
	void ShowMainMenu();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void StartGame();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void ShowGameOver();

protected:
	virtual void BeginPlay() override;

private:
	// ---------- INTERNAL HELPERS ----------

	/** Switch UI and input mode to a given state */
	void SwitchTo(TSubclassOf<UUserWidget> WidgetClass, ETPFlow NewState);

	/** 1-second countdown tick */
	void GameTick();

	/** Periodic target spawns while in Playing state */
	void SpawnTick();

	/** Returns a random point inside the SpawnArea box */
	FVector GetRandomPointInArea() const;

private:
	// ---------- RUNTIME STATE ----------

	/** Currently active widget on the viewport */
	UPROPERTY()
	UUserWidget* CurrentWidget = nullptr;

	/** Cached player controller reference */
	UPROPERTY()
	APlayerController* PlayerControllerRef = nullptr;

	/** Current game flow state */
	UPROPERTY()
	ETPFlow State = ETPFlow::MainMenu;

	/** Timer for the game countdown */
	FTimerHandle TimerHandle_GameTick;

	/** Timer for periodic target spawns */
	FTimerHandle TimerHandle_SpawnTick;

	/** Remaining time in seconds */
	int32 TimeRemaining = 0;

	/** Player score */
	int32 Score = 0;
};
