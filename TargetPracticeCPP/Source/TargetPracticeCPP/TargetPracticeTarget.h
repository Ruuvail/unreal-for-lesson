#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TargetPracticeTarget.generated.h"

class UStaticMeshComponent;
class USceneComponent;
class ATargetPracticeManager;

UCLASS()
class TARGETPRACTICECPP_API ATargetPracticeTarget : public AActor
{
	GENERATED_BODY()

public:
	ATargetPracticeTarget();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	// ---------- COMPONENTS ----------

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* TargetMesh;

	// ---------- TARGET SETTINGS ----------

	/** How many points this target gives when hit */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target")
	int32 ScoreValue = 10;

	/** Life time in seconds before the target auto-destroys (<= 0 means infinite) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target")
	float MaxLifeTime = 10.0f;

	/** Simple rotation speed (degrees per second) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target|Movement")
	float RotationSpeed = 60.0f;

	/** Optional explicit reference to the manager (can also auto-find) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target")
	ATargetPracticeManager* Manager = nullptr;

private:
	FTimerHandle TimerHandle_LifeTime;

	/** Called when something hits the mesh */
	UFUNCTION()
	void OnMeshHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, FVector NormalImpulse,
		const FHitResult& Hit);

	void HandleAutoDestroy();
};

