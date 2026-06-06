#include "BTT_UpdateNeeds.h"

#include "AIController.h"
#include "NeedsComponent.h"
#include "Survivor/SurvivorPawn.h"

UBTT_UpdateNeeds::UBTT_UpdateNeeds()
{
	NodeName = "Update Needs";
}

EBTNodeResult::Type UBTT_UpdateNeeds::ExecuteTask(UBehaviorTreeComponent& root, uint8* nodeMemory)
{
	AAIController* AIController = root.GetAIOwner();
	if (!AIController) return EBTNodeResult::Failed;
	
	ASurvivorPawn* Survivor = Cast<ASurvivorPawn>(AIController->GetPawn());
	if (!Survivor) return EBTNodeResult::Failed;

	UBlackboardComponent* BlackBoard = root.GetBlackboardComponent();
	if (!BlackBoard) return EBTNodeResult::Failed;

	UNeedsComponent* Needs = Survivor->FindComponentByClass<UNeedsComponent>();
	if (!Needs) return EBTNodeResult::Failed;

	Needs->RecalculateNeeds(Survivor, BlackBoard);
	
	return EBTNodeResult::Succeeded;
}
