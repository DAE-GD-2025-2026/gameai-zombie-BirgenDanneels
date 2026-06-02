// Fill out your copyright notice in the Description page of Project Settings.
#include "StudentPerceptor.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Zombies/BaseZombie.h"
#include "Items/Food.h"
#include "Items/Medkit.h"
#include "Items/Pistol.h"
#include "Items/Shotgun.h"
#include "Navigation/PathFollowingComponent.h"
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
			ZombiesSeen.Add(Zombie);

			if (GetWorld())
			{
				ZombieLastSeenTimes.Add(Zombie, GetWorld()->GetTimeSeconds());
			}
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
		 // if (ABaseItem* Item = Cast<ABaseItem>(Actor))
		 // {
		 // 	if (Item == Blackboard->GetValueAsObject(FName("TargetItem")))
		 // 	{
		 // 		Blackboard->ClearValue(FName("TargetItem"));
		 // 	}
		 // }
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

	for (auto It = ZombiesSeen.CreateIterator(); It; ++It)
	{
		ABaseZombie* Zombie = *It;

		if (!IsValid(Zombie))
		{
			It.RemoveCurrent();
			ZombieLastSeenTimes.Remove(Zombie);
			continue;
		}

		const float* LastSeenTime = ZombieLastSeenTimes.Find(Zombie);

		if (!LastSeenTime || Now - *LastSeenTime > ZombieMemoryTime)
		{
			It.RemoveCurrent();
			ZombieLastSeenTimes.Remove(Zombie);
		}
	}
}
