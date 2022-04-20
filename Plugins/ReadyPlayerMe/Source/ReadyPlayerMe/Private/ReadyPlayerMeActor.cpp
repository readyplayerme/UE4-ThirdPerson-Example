// Copyright © 2021++ Ready Player Me

#include "ReadyPlayerMeActor.h"
#include "Components/SkeletalMeshComponent.h"
#include "ReadyPlayerMeActorComponent.h"

// Sets default values
AReadyPlayerMeActor::AReadyPlayerMeActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SetRootComponent(SkeletalMeshComponent);

	ReadyPlayerMeComponent = CreateDefaultSubobject<UReadyPlayerMeActorComponent>(TEXT("ReadyPlayerMe"));
	SkeletalMeshComponent->SetAnimationMode(EAnimationMode::AnimationBlueprint);
}

// Called when the game starts or when spawned
void AReadyPlayerMeActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AReadyPlayerMeActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
