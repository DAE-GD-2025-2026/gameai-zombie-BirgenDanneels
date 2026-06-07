// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Perception/AISense_Damage.h"
#include "Zombies/BaseZombie.h"
#include "PurgeZones/PurgeZone.h"
#include "StudentPerceptorDanneelsBirgen.generated.h"

class AHouse;
class ABaseItem;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DANNEELSBIRGENZOMBIERUNTIME_API UStudentPerceptorDanneelsBirgen : public UActorComponent
{
	GENERATED_BODY()

public:
	UStudentPerceptorDanneelsBirgen();
	
	virtual void BeginPlay() override;
	
	virtual void TickComponent(float DeltaTime,	ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	virtual void OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
	
	const TSet<TObjectPtr<ABaseItem>>& GetSeenLoot() const {return SeenLoot;}
	
	const TArray<ABaseZombie*>& GetZombiesSeen() const {return ZombieMemory.ResultArray;};
	
	void CleanUpSeenLoot();
	
	//House
	const TMap<TObjectPtr<AHouse>, float>& GetVisitedHouses() const {return LastVisitedHouseTime;}
	void MarkHouseVisited(AHouse* House);
	float GetLastVisitedHouseTime(AHouse* House) const;
	
	//Purge Zones
	const TArray<APurgeZone*>& GetPurgeZones() const {return PurgeZoneMemory.ResultArray;}
	
private:
	
	template<typename T>
	struct MemoryStruct
	{
		TMap<TWeakObjectPtr<T>, float> LastSeenTimes;
		TArray<T*> ResultArray;
		bool IsDirty = true;
		
		void Add(T* Actor, float Time)
		{
			LastSeenTimes.Add(Actor, Time);
			IsDirty = true;
		}
	};
	
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
	MemoryStruct<ABaseZombie> ZombieMemory;

	UPROPERTY()
	float MemoryTime = 5.0f;
	
	// Purge Zones
	MemoryStruct<APurgeZone> PurgeZoneMemory;
	
	template<typename T>
	void CleanUpArray(MemoryStruct<T>& Memory);
};

template <typename T>
void UStudentPerceptorDanneelsBirgen::CleanUpArray(MemoryStruct<T>& Memory)
{
	if (!GetWorld()) return;

	const float Now = GetWorld()->GetTimeSeconds();
	
	for (auto It = Memory.LastSeenTimes.CreateIterator(); It; ++It)
	{
		auto Object = It.Key();

		if (!Object.IsValid())
		{
			It.RemoveCurrent();
			Memory.IsDirty = true;
			continue;
		}

		const float LastSeenTime = It.Value();
		
		if (Now - LastSeenTime > MemoryTime && LastSeenTime > 0.f)
		{
			It.RemoveCurrent();
			Memory.IsDirty = true;
			continue;
		}
	}
	
	if (Memory.IsDirty)
	{
		Memory.ResultArray.Reset();
		
		for (auto It = Memory.LastSeenTimes.CreateIterator(); It; ++It)
		{
			auto Object = It.Key();

			if (Object.IsValid())
			{
				Memory.ResultArray.Add(Object.Get());
			}
		}
		
		Memory.IsDirty = false;
	}
}
