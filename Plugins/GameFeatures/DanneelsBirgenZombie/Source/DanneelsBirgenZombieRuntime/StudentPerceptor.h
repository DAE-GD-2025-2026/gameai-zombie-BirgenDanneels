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
	UStudentPerceptor();
	
	virtual void BeginPlay() override;
	
	virtual void TickComponent(float DeltaTime,	ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	virtual void OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
	
	const TSet<TObjectPtr<ABaseItem>>& GetSeenLoot() const {return SeenLoot;}
	
	const TArray<ABaseZombie*>& GetZombiesSeen() const {return ResultZombieArray;};
	
	const TArray<FVector>& GetRecentlyVisited() const {return RecentlyVisited;}
	void AddVisitedLocation(const FVector& Location);
	
	void CleanUpSeenLoot();
	
	//House
	const TMap<TObjectPtr<AHouse>, float>& GetVisitedHouses() const {return LastVisitedHouseTime;}
	void MarkHouseVisited(AHouse* House);
	float GetLastVisitedHouseTime(AHouse* House) const;
	
private:
	
	UPROPERTY()
	UBlackboardComponent* Blackboard;

	UPROPERTY()
	AAIController* OwnerController;
	
	
	// Houses
	UPROPERTY()
	TMap<TObjectPtr<AHouse>, float> LastVisitedHouseTime;
	
	// Loot
	TSet<TObjectPtr<ABaseItem>> SeenLoot;
	
	// Zombies
	UPROPERTY()
	TMap<TObjectPtr<ABaseZombie>, float> ZombieLastSeenTimes;
	TArray<TObjectPtr<ABaseZombie>> ResultZombieArray;
	bool IsZombieArrayDirty = true;
	
	void CleanUpSeenZombies();

	UPROPERTY()
	float ZombieMemoryTime = 5.0f;
	
	// Visited Locations (make a task that adds to these locations, cap them and if cap is reached start removing the oldest ones)
	TArray<FVector> RecentlyVisited;
};
