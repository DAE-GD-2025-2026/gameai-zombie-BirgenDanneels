#include "BTT_UpdateNeedsDanneelsBirgen.h"

#include "AIController.h"
#include "NeedsComponentDanneelsBirgen.h"
#include "Survivor/SurvivorPawn.h"

UBTT_UpdateNeedsDanneelsBirgen::UBTT_UpdateNeedsDanneelsBirgen()
{
	NodeName = "Update Needs";
}

EBTNodeResult::Type UBTT_UpdateNeedsDanneelsBirgen::ExecuteTask(UBehaviorTreeComponent& root, uint8* nodeMemory)
{
	AAIController* AIController = root.GetAIOwner();
	if (!AIController) return EBTNodeResult::Failed;
	
	ASurvivorPawn* Survivor = Cast<ASurvivorPawn>(AIController->GetPawn());
	if (!Survivor) return EBTNodeResult::Failed;

	UBlackboardComponent* BlackBoard = root.GetBlackboardComponent();
	if (!BlackBoard) return EBTNodeResult::Failed;

	UNeedsComponentDanneelsBirgen* Needs = Survivor->FindComponentByClass<UNeedsComponentDanneelsBirgen>();
	if (!Needs) return EBTNodeResult::Failed;

	Needs->RecalculateNeeds(Survivor, BlackBoard);
	
	return EBTNodeResult::Succeeded;
}
