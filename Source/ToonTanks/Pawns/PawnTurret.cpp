// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnTurret.h"
#include "PawnTank.h"
#include "Kismet/GameplayStatics.h"

// Called when the game starts or when spawned
void APawnTurret::BeginPlay()
{
	Super::BeginPlay();
	
    GetWorld()->GetTimerManager().SetTimer(FireRateTimerHandle, this, &APawnTurret::CheckFireCondition, FireRate, true);

    PlayerPawn = Cast<APawnTank>(UGameplayStatics::GetPlayerPawn(this, 0));
}

// Called every frame
void APawnTurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (!PlayerPawn || GetDistanceToPlayer() > FireRange)
    {
        return;
    }

    RotateTurret(PlayerPawn->GetActorLocation());
}

void APawnTurret::CheckFireCondition() 
{
    if(!PlayerPawn) {return;}

    if(GetDistanceToPlayer() <= FireRange)
    {
        Fire();
    }

}

float APawnTurret::GetDistanceToPlayer() 
{
    if(!PlayerPawn)
    {
        return 0.f;
    }

    return FVector::Dist(PlayerPawn->GetActorLocation(), GetActorLocation());
}

void APawnTurret::RotateTurret(FVector LookAtTarget) 
{
	FVector LookAtTargetClean = FVector(LookAtTarget.X, LookAtTarget.Y, TurretMesh->GetComponentLocation().Z);
	FVector StartLocation = TurretMesh->GetComponentLocation();
	FRotator TurretRotation = FVector(LookAtTargetClean - StartLocation).Rotation();

    // float NewRotationYaw = FMath::Lerp(TurretMesh->GetComponentRotation().Yaw, TurretRotation.Yaw, GetWorld()->DeltaTimeSeconds * TurretRotationSpeed);
    // FRotator NewRotation = FRotator(TurretRotation.Pitch, NewRotationYaw, TurretRotation.Roll);
    FRotator NewRotation = FMath::Lerp(TurretMesh->GetComponentRotation(), TurretRotation, GetWorld()->DeltaTimeSeconds * TurretRotationSpeed);
    UE_LOG(LogTemp, Warning, TEXT("%s"), *NewRotation.ToString());
    TurretMesh->SetWorldRotation(NewRotation);
}
