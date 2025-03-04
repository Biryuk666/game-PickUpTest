// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

class UCameraComponent;
class APickableObject;

UCLASS()
class PICKUPTEST_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseCharacter();

	UFUNCTION(BlueprintCallable)
	void PickUpObject();

	UFUNCTION(BlueprintCallable)
	void ThrowObject();

	UFUNCTION(BlueprintCallable)
	bool IsHandsBusy() const;

	UFUNCTION(BlueprintCallable)
	bool CanPickUp() const;

protected:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UCameraComponent* CameraComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "PickUp")
	float PickUpRange = 100.0f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "PickUp")
	float ThrowImpulse = 1000.0f;

	UPROPERTY(VisibleAnywhere, Category = "PickUp")
	APickableObject* HeldObject;

	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	bool CanPickUpObject = false;

	void MoveForward(float Amount);
	void MoveRight(float Amount);	

	void GetTraceData(FVector& TraceStart, FVector& TraceEnd) const;
	void ComputeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd) const;
};
