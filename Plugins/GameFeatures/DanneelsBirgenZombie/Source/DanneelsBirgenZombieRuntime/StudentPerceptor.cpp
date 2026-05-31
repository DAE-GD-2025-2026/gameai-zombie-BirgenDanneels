// Fill out your copyright notice in the Description page of Project Settings.
#include "StudentPerceptor.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "AIController.h"
#include "Zombies/BaseZombie.h"
#include "Items/Food.h"
#include "Items/Medkit.h"
#include "Items/Pistol.h"
#include "Items/Shotgun.h"
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
			if (!ZombiesInVision.Contains(Zombie))
			{
				ZombiesInVision.Add(Zombie);
			}
			
			ZombiesSeen.Add(Zombie);
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
			if (!SeenHouses.Contains(House))
			{
				SeenHouses.Add(House);
			}
			
			GEngine->AddOnScreenDebugMessage(5, 1.f, FColor::Green, 
	FString::Printf(TEXT("Saw house!")));
		}
		return;
	}
	else
	{
		if (ABaseZombie* Zombie = Cast<ABaseZombie>(Actor))
			ZombiesInVision.Remove(Zombie);
		
		 // if (ABaseItem* Item = Cast<ABaseItem>(Actor))
		 // {
		 // 	if (Item == Blackboard->GetValueAsObject(FName("TargetItem")))
		 // 	{
		 // 		Blackboard->ClearValue(FName("TargetItem"));
		 // 	}
		 // }
	}
}

void UStudentPerceptor::VisitHouse(AHouse* House)
{
	if (VisitedHouses.Contains(House)) return;
	VisitedHouses.Add(House);
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
