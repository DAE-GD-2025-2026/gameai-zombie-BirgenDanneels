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
		if (AHouse* House = Cast<AHouse>(Actor)) //house
		{
			Blackboard->SetValueAsObject(FName("TargetHouse"), House);
			GEngine->AddOnScreenDebugMessage(5, 1.f, FColor::Green, 
	FString::Printf(TEXT("Saw house!")));
		}
		return;
	}
}
