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
	
	virtual void TickComponent(float DeltaTime,	ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	virtual void OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
	
	const TSet<TObjectPtr<AHouse>>& GetSeenHouses() const {return SeenHouses;}
	const TSet<TObjectPtr<AHouse>>& GetVisitedHouses() const {return VisitedHouses;}
	const TSet<TObjectPtr<ABaseItem>>& GetSeenLoot() const {return SeenLoot;}
	
	const TSet<TObjectPtr<ABaseZombie>>& GetZombiesSeen() const {return ZombiesSeen;}
	
	const TArray<FVector>& GetRecentlyVisited() const {return RecentlyVisited;}
	void AddVisitedLocation(const FVector& Location);
	
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
	TSet<TObjectPtr<ABaseZombie>> ZombiesSeen;
	
	void CleanUpSeenZombies();

	UPROPERTY()
	float ZombieMemoryTime = 5.0f;

	UPROPERTY()
	TMap<TObjectPtr<ABaseZombie>, float> ZombieLastSeenTimes;
	
	// Visited Locations (make a task that adds to these locations, cap them and if cap is reached start removing the oldest ones)
	TArray<FVector> RecentlyVisited;
};
