#include "BTT_EnterHouse.h"
#include "BehaviorTree/Blackboardcomponent.h"
#include "AIController.h"
#include "Survivor/SurvivorPawn.h"
#include "Village/House/House.h"

UBTT_EnterHouse::UBTT_EnterHouse()
{
	NodeName = "Enter House";
}

EBTNodeResult::Type UBTT_EnterHouse::ExecuteTask(UBehaviorTreeComponent& root, uint8* nodeMemory)
{
	AAIController* AIController = root.GetAIOwner();
	if (!AIController) return EBTNodeResult::Failed;

	ASurvivorPawn* survivor = Cast<ASurvivorPawn>(AIController->GetPawn());
	if (!survivor) return EBTNodeResult::Failed;
	
	UBlackboardComponent* Blackboard = AIController->GetBlackboardComponent();
	
	AHouse* house = Cast<AHouse>(Blackboard->GetValueAsObject("TargetHouse"));
	if (!house) return EBTNodeResult::Failed;
	
	FVector houseLocation{house->GetBounds().Origin};
	Blackboard->SetValueAsVector("HouseLocation", houseLocation);
	
	return EBTNodeResult::Succeeded;
}
