#include "TargetPracticeTarget.h"
#include "TargetPracticeManager.h"

#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "EngineUtils.h"

ATargetPracticeTarget::ATargetPracticeTarget()
{
	PrimaryActorTick.bCanEverTick = true;

	// Root component
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	// Mesh component
	TargetMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TargetMesh"));
	TargetMesh->SetupAttachment(Root);

	// Basic collision so that projectiles / traces can hit this actor
	TargetMesh->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	TargetMesh->SetGenerateOverlapEvents(false);
	TargetMesh->SetNotifyRigidBodyCollision(true); // needed for OnComponentHit
}

void ATargetPracticeTarget::BeginPlay()
{
	Super::BeginPlay();

	// Hit event 
	if (TargetMesh)
	{
		TargetMesh->OnComponentHit.AddDynamic(this, &ATargetPracticeTarget::OnMeshHit);
	}

	//
	if (MaxLifeTime > 0.0f)
	{
		GetWorldTimerManager().SetTimer(
			TimerHandle_LifeTime, this, &ATargetPracticeTarget::HandleAutoDestroy,
			MaxLifeTime, false);
	}
}

void ATargetPracticeTarget::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// Bas
	if (RotationSpeed != 0.0f)
	{
		const float YawDelta = RotationSpeed * DeltaSeconds;
		AddActorLocalRotation(FRotator(0.f, YawDelta, 0.f));
	}
}

void ATargetPracticeTarget::OnMeshHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse,
	const FHitResult& Hit)
{
	if (!OtherActor || OtherActor == this)
	{
		return;
	}

	// Manager BP
	if (!Manager)
	{
		if (UWorld* World = GetWorld())
		{
			for (TActorIterator<ATargetPracticeManager> It(World); It; ++It)
			{
				Manager = *It;
				break;
			}
		}
	}

	// Skor 
	if (Manager && ScoreValue != 0)
	{
		Manager->AddScore(ScoreValue);
	}

	Destroy();
}

void ATargetPracticeTarget::HandleAutoDestroy()
{
	if (!IsPendingKill())
	{
		Destroy();
	}
}
