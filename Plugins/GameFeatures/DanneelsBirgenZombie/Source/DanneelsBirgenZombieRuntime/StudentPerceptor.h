// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Perception/AISense_Damage.h"
#include "StudentPerceptor.generated.h"

class AHouse;
class ABaseItem;
class ABaseZombie;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DANNEELSBIRGENZOMBIERUNTIME_API UStudentPerceptor : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UStudentPerceptor();
	
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
	
	const TSet<TObjectPtr<AHouse>>& GetSeenHouses() const {return SeenHouses;}
	const TSet<TObjectPtr<AHouse>>& GetVisitedHouses() const {return VisitedHouses;}
	const TSet<TObjectPtr<ABaseItem>>& GetSeenLoot() const {return SeenLoot;}
	
	const TSet<TObjectPtr<ABaseZombie>>& GetZombiesInVision() const {return ZombiesInVision;}
	const TSet<TObjectPtr<ABaseZombie>>& GetZombiesSeen() const {return ZombiesSeen;}
	
	void VisitHouse(AHouse* House);
	void CleanUpSeenLoot();
	
private:
	
	UPROPERTY()
	UBlackboardComponent* Blackboard;

	UPROPERTY()
	AAIController* OwnerController;
	
	
	// Houses
	UPROPERTY()
	TSet<TObjectPtr<AHouse>> SeenHouses;
	UPROPERTY()
	TSet<TObjectPtr<AHouse>> VisitedHouses;
	
	// Loot
	TSet<TObjectPtr<ABaseItem>> SeenLoot;
	
	// Zombies
	UPROPERTY()
	TSet<TObjectPtr<ABaseZombie>> ZombiesInVision;
	UPROPERTY()
	TSet<TObjectPtr<ABaseZombie>> ZombiesSeen;
};
