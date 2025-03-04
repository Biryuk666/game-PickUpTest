// PickUp Test Game. All Right Reserved. 

#include "Actors/PickableObject.h"
#include "Characters/BaseCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework//Character.h"

APickableObject::APickableObject()
{
	PrimaryActorTick.bCanEverTick = false;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	MeshComponent->SetSimulatePhysics(true);
	SetRootComponent(MeshComponent);
}

void APickableObject::PickUp(ABaseCharacter* Player)
{
	FVector ScaleTransform = MeshComponent->GetComponentScale();
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
	MeshComponent->SetSimulatePhysics(false);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AttachToComponent(Player->GetMesh(), AttachmentRules, PickUpSocketName);
	MeshComponent->SetRelativeScale3D(ScaleTransform);	
}

void APickableObject::Throw(FVector Impulse)
{
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	MeshComponent->SetSimulatePhysics(true);
	MeshComponent->AddImpulse(Impulse);
}

void APickableObject::BeginPlay()
{
	Super::BeginPlay();

	check(MeshComponent);
}

