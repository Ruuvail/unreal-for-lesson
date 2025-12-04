#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TargetPracticeCPPGameMode.generated.h"

class UUserWidget;

UCLASS(minimalapi)
class ATargetPracticeCPPGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ATargetPracticeCPPGameMode();

protected:
	virtual void BeginPlay() override;

	// ---------- UI CLASSES ----------

	/** Main menu widget class */
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> MainMenuClass;

	/** In-game HUD widget class */
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> GameUIClass;

	/** Game over widget class */
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> GameOverUIClass;

private:
	/** Currently active widget on screen */
	UPROPERTY()
	UUserWidget* CurrentWidget = nullptr;

	/** Internal helper to switch widgets */
	void SwitchToWidget(TSubclassOf<UUserWidget> WidgetClass);

public:
	// ---------- UI FLOW ----------

	/** Show main menu screen */
	UFUNCTION(BlueprintCallable, Category = "UI")
	void ShowMainMenu();

	/** Start the game and show game HUD */
	UFUNCTION(BlueprintCallable, Category = "UI")
	void StartGame();

	/** Show game over screen */
	UFUNCTION(BlueprintCallable, Category = "UI")
	void ShowGameOver();
};
