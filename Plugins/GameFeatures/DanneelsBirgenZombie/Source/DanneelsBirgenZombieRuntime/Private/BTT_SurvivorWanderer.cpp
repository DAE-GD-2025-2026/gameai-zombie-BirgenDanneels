// Fill out your copyright notice in the Description page of Project Settings.

#include "BTT_SurvivorWanderer.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Survivor/SurvivorPawn.h"
#include "NavigationSystem.h"
#include "Navigation/PathFollowingComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DanneelsBirgenZombieRuntime/StudentPerceptor.h"
#include "EnvironmentQuery/EnvQueryManager.h"

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
	
	//Temp
	UStudentPerceptor* Perceptor = survivor->FindComponentByClass<UStudentPerceptor>();
	
	FEnvQueryRequest Request = FEnvQueryRequest(WanderQuery, AIController->GetPawn());

	Request.Execute(
	EEnvQueryRunMode::RandomBest5Pct,
	FQueryFinishedSignature::CreateLambda(
		[Blackboard, Perceptor](TSharedPtr<FEnvQueryResult> Result)
		{
			if (!Result.IsValid() || !Result->IsSuccessful())
				return;

			FVector Location = Result->GetItemAsLocation(0);
			Blackboard->SetValueAsVector(TEXT("WanderLocation"), Location);
			Perceptor->AddVisitedLocation(Location);
			
		}
		)
	);
	
	return EBTNodeResult::Succeeded;
}
