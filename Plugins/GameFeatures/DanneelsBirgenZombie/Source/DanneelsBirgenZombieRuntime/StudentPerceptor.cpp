// Fill out your copyright notice in the Description page of Project Settings.
#include "StudentPerceptor.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "AIController.h"
#include "Zombies/BaseZombie.h"
#include "Items/Medkit.h"
#include "PurgeZones/PurgeZone.h"
#include "Village/House/House.h"

UStudentPerceptor::UStudentPerceptor()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UStudentPerceptor::BeginPlay()
{
	Super::BeginPlay();
	
	if (auto PerceptionComp = GetOwner()->GetComponentByClass<UAIPerceptionComponent>())
	{
		PerceptionComp->OnTargetPerceptionUpdated.AddDynamic(this, &UStudentPerceptor::OnPerceptionUpdated);
	}
	
	APawn* Pawn = Cast<APawn>(GetOwner());
	if (!Pawn) return;
	
	OwnerController = Cast<AAIController>(Pawn->GetController());
	if (OwnerController)
	{
		Blackboard = OwnerController->GetBlackboardComponent();
	}
	
	if (!OwnerController)
	{
		GEngine->AddOnScreenDebugMessage(5, 1.f, FColor::Red,FString::Printf(TEXT("No controller!") ) );
	}
	else if (!Blackboard)
	{
		GEngine->AddOnScreenDebugMessage(5, 1.f, FColor::Red,FString::Printf(TEXT("No BB!") ) );
	}
}

void UStudentPerceptor::OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (!Actor || !Blackboard)
	{
		GEngine->AddOnScreenDebugMessage(5, 1.f, FColor::Red,FString::Printf(TEXT("Actor or BB missing!") ) );
		return;
	}

	if (Stimulus.WasSuccessfullySensed())
	{
		if (ABaseZombie* Zombie = Cast<ABaseZombie>(Actor))
		{
			ZombieLastSeenTimes.Add(Zombie, -1.f);
			IsZombieArrayDirty = true;
		}
	
		if (ABaseItem* Item = Cast<ABaseItem>(Actor))
		{
			if (Item->GetItemType() != EItemType::Garbage)
			{
				SeenLoot.Add(Item);
			}
		}
		
		if (AHouse* House = Cast<AHouse>(Actor)) //house
		{
			if (!LastVisitedHouseTime.Contains(House))
			{
				LastVisitedHouseTime.Add(House, 0.f);
				
				GEngine->AddOnScreenDebugMessage(5, 1.f, FColor::Green, "Saw new house!");
			}
		}
		return;
	}
	else
	{
		if (ABaseZombie* Zombie = Cast<ABaseZombie>(Actor))
		{
			if (GetWorld())
			{
				ZombieLastSeenTimes.Add(Zombie, GetWorld()->GetTimeSeconds());
				IsZombieArrayDirty = true;
			}
		}
	}
}

void UStudentPerceptor::TickComponent(float DeltaTime, ELevelTick TickType,	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CleanUpSeenZombies();
}

void UStudentPerceptor::AddVisitedLocation(const FVector& Location)
{
	if (RecentlyVisited.Num() > 10)
	{
		RecentlyVisited.RemoveAt(0);
	}
	
	RecentlyVisited.Add(Location);
}

void UStudentPerceptor::CleanUpSeenLoot()
{
	for (auto It = SeenLoot.CreateIterator(); It; ++It)
	{
		ABaseItem* Item = *It;

		if (!IsValid(Item) || Item->IsHidden())
		{
			It.RemoveCurrent();
		}
	}
}

void UStudentPerceptor::MarkHouseVisited(AHouse* House)
{
	if (!House || !GetWorld())
	{
		return;
	}

	LastVisitedHouseTime.FindOrAdd(House) = GetWorld()->GetTimeSeconds();
	Blackboard->SetValueAsFloat("LastHouseSearchTime", GetWorld()->GetTimeSeconds());
}

float UStudentPerceptor::GetLastVisitedHouseTime(AHouse* House) const
{
	if (!House)
		return 0.f;
	
	if (LastVisitedHouseTime.Contains(House))
		return LastVisitedHouseTime[House];
	
	return 0.f;
}

void UStudentPerceptor::CleanUpSeenZombies()
{
	if (!GetWorld()) return;

	const float Now = GetWorld()->GetTimeSeconds();
	
	for (auto It = ZombieLastSeenTimes.CreateIterator(); It; ++It)
	{
		ABaseZombie* Zombie = It.Key();

		if (!IsValid(Zombie))
		{
			It.RemoveCurrent();
			IsZombieArrayDirty = true;
			continue;
		}

		const float LastSeenTime = It.Value();
		
		if (Now - LastSeenTime > ZombieMemoryTime && LastSeenTime > 0.f)
		{
			It.RemoveCurrent();
			IsZombieArrayDirty = true;
			continue;
		}
	}
	
	if (IsZombieArrayDirty)
	{
		ResultZombieArray.Reset();
		
		for (auto It = ZombieLastSeenTimes.CreateIterator(); It; ++It)
		{
			ABaseZombie* Zombie = It.Key();

			if (IsValid(Zombie))
			{
				ResultZombieArray.Add(Zombie);
			}
		}
		
		IsZombieArrayDirty = false;
	}
}
