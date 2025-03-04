// Fill out your copyright notice in the Description page of Project Settings.

#include "Actors/PickableObject.h"
#include "Camera/CameraComponent.h"
#include "Characters/BaseCharacter.h"
#include "DrawDebugHelpers.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 80.0f));
	CameraComponent->bUsePawnControlRotation = true;
	CameraComponent->SetupAttachment(GetRootComponent());
}

void ABaseCharacter::PickUpObject()
{
	if (!CanPickUpObject)
		return;

	FVector TraceStart;
	FVector TraceEnd;
	GetTraceData(TraceStart, TraceEnd);

	FHitResult HitResult;
	ComputeHit(HitResult, TraceStart, TraceEnd);

	if (HitResult.bBlockingHit)
	{
		const auto PickableObject = Cast<APickableObject>(HitResult.GetActor());
		if (PickableObject)
		{
			HeldObject = PickableObject;
			HeldObject->PickUp(this);
			CanPickUpObject = false;
		}
	}
}

void ABaseCharacter::ThrowObject()
{
	if (!HeldObject)
		return;

	HeldObject->Throw(CameraComponent->GetForwardVector() * ThrowImpulse);
	HeldObject = nullptr;
}

bool ABaseCharacter::IsHandsBusy() const
{
	return !HeldObject;
}

bool ABaseCharacter::CanPickUp() const
{
	return CanPickUpObject && !HeldObject;
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	check(CameraComponent);
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector TraceStart;
	FVector TraceEnd;
	GetTraceData(TraceStart, TraceEnd);

	FHitResult HitResult;
	ComputeHit(HitResult, TraceStart, TraceEnd);

	//DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 0.1f, 0, 3.0f);

	if (HitResult.bBlockingHit)
	{
		const auto PickableObject = Cast<APickableObject>(HitResult.GetActor());
		if (PickableObject || !HeldObject)
		{
			CanPickUpObject = true;
		}
	}
	else
	{
		CanPickUpObject = false;
	}
}

void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ABaseCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABaseCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &ABaseCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("TurnAround", this, &ABaseCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAction("PickUp", IE_Pressed, this, &ABaseCharacter::PickUpObject);
	PlayerInputComponent->BindAction("Throw", IE_Pressed, this, &ABaseCharacter::ThrowObject);
}

void ABaseCharacter::MoveForward(float Amount)
{
	if (!Amount)
		return;

	AddMovementInput(GetActorForwardVector(), Amount);
}

void ABaseCharacter::MoveRight(float Amount)
{
	if (!Amount)
		return;

	AddMovementInput(GetActorRightVector(), Amount);
}

void ABaseCharacter::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const
{
	FVector ViewLocation;
	FRotator ViewRotation;

	Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);

	TraceStart = ViewLocation;
	const FVector Direction = ViewRotation.Vector();
	TraceEnd = TraceStart + Direction * PickUpRange;
}

void ABaseCharacter::ComputeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd) const
{
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility);
}