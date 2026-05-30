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

	if (!Stimulus.WasSuccessfullySensed())
	{
	// 	if (APurgeZone * sensedPurgeZone = Cast<APurgeZone>(Actor))
	// 	{
	// 		Blackboard->SetValueAsObject(FName("TargetPurgeZone"),sensedPurgeZone);
	// 		GEngine->AddOnScreenDebugMessage(5, 1.f, FColor::Green, 
	// FString::Printf(TEXT("Saw purge!")));
	// 	}
		
	// 	if ( ABaseZombie * SensedZombie = Cast<ABaseZombie>(Actor))
	// 	{
	// 		GEngine->AddOnScreenDebugMessage(5, 1.f, FColor::Green, 
	// FString::Printf(TEXT("Saw zombie!")));
	// 		Blackboard->SetValueAsObject(FName("x"), SensedZombie);
	// 	}
	// 	
	// 	if (AFood* SensedFood = Cast<AFood>(Actor))
	// 	{
	// 		Blackboard->SetValueAsObject(FName("TargetFood"), SensedFood);
	// 		Blackboard->SetValueAsObject(FName("TargetItem"), SensedFood);
	// 		GEngine->AddOnScreenDebugMessage(5, 1.f, FColor::Green, 
	// FString::Printf(TEXT("Saw food!")));
	// 	}
		
	// 	if (AMedkit* SensedMedkit = Cast<AMedkit>(Actor))
	// 	{
	// 		Blackboard->SetValueAsObject(FName("TargetMedkit"), SensedMedkit);
	// 		Blackboard->SetValueAsObject(FName("TargetItem"), SensedMedkit);
	// 		GEngine->AddOnScreenDebugMessage(5, 1.f, FColor::Green, 
	// FString::Printf(TEXT("Saw Medkit!")));
	// 	}
		
	// 	if (APistol* SensedPistol = Cast<APistol>(Actor))//pistol
	// 	{
	// 		Blackboard->SetValueAsObject(FName("TargetPistol"), SensedPistol);
	// 		Blackboard->SetValueAsObject(FName("TargetItem"), SensedPistol);
	// 		GEngine->AddOnScreenDebugMessage(5, 1.f, FColor::Green, 
	// FString::Printf(TEXT("Saw pistol!")));
	// 	}
		
	// 	if (AShotgun* SensedShotGun = Cast<AShotgun>(Actor))//shotgun
	// 	{
	// 		Blackboard->SetValueAsObject(FName("TargetShotgun"), SensedShotGun);
	// 		Blackboard->SetValueAsObject(FName("TargetItem"), SensedShotGun);
	// 		GEngine->AddOnScreenDebugMessage(5, 1.f, FColor::Green, 
	// FString::Printf(TEXT("Saw shotgun!")));
	// 	}
		
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
}

void UStudentPerceptor::VisitHouse(AHouse* House)
{
	if (VisitedHouses.Contains(House)) return;
	VisitedHouses.Add(House);
}
