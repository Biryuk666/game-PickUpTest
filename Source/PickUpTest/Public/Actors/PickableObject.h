// PickUp Test Game. All Right Reserved. 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickableObject.generated.h"

class ABaseCharacter;
class UStaticMeshComponent;

UCLASS()
class PICKUPTEST_API APickableObject : public AActor
{
	GENERATED_BODY()
	
public:
	APickableObject();

	void PickUp(ABaseCharacter* Player);
	void Throw(FVector Impulse);

protected:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PickUp")
	FName PickUpSocketName = "PickUpSocket";

	virtual void BeginPlay() override;
};
