// Copyright © 2021++ Ready Player Me

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ReadyPlayerMeActor.generated.h"

UCLASS()
class READYPLAYERME_API AReadyPlayerMeActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AReadyPlayerMeActor();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = ReadyPlayerMe, meta = (ExposeFunctionCategories = "Ready Player Me", AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* SkeletalMeshComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = ReadyPlayerMe, meta = (ExposeFunctionCategories = "Ready Player Me", AllowPrivateAccess = "true"))
	class UReadyPlayerMeActorComponent* ReadyPlayerMeComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
