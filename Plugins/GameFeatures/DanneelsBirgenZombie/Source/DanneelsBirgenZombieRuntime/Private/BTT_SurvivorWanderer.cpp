// Fill out your copyright notice in the Description page of Project Settings.

#include "BTT_SurvivorWanderer.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Survivor/SurvivorPawn.h"
#include "NavigationSystem.h"
#include "Navigation/PathFollowingComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTT_SurvivorWanderer::UBTT_SurvivorWanderer()
{
	NodeName = "Survivor Wander";
}

EBTNodeResult::Type UBTT_SurvivorWanderer::ExecuteTask(UBehaviorTreeComponent& root, uint8* nodeMemory)
{
	AAIController* AIController = root.GetAIOwner();
	if (!AIController) return EBTNodeResult::Failed;

	ASurvivorPawn* survivor = Cast<ASurvivorPawn>(AIController->GetPawn());
	if (!survivor) return EBTNodeResult::Failed;
	
	UBlackboardComponent* Blackboard = AIController->GetBlackboardComponent();
	if (!Blackboard) return EBTNodeResult::Failed;
	
	FNavLocation wanderLocation{};
	
	
	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	if (!NavSys) return EBTNodeResult::Failed;
	
	NavSys->GetRandomReachablePointInRadius(survivor->GetActorLocation(), WanderRadius, wanderLocation);
	
	Blackboard->SetValueAsVector("WanderLocation", wanderLocation.Location);
	
	return EBTNodeResult::Succeeded;
}
