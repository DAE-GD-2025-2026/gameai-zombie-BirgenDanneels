#include "BTT_FinishVisit.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DanneelsBirgenZombieRuntime/StudentPerceptor.h"
#include "Village/House/House.h"

UBTT_FinishVisit::UBTT_FinishVisit()
{
}

EBTNodeResult::Type UBTT_FinishVisit::ExecuteTask(UBehaviorTreeComponent& root, uint8* nodeMemory)
{
	APawn* Pawn = root.GetAIOwner()->GetPawn();
	if (!Pawn) return EBTNodeResult::Failed;
	
	UBlackboardComponent* Blackboard = root.GetAIOwner()->GetBlackboardComponent();
	if (!Blackboard) return EBTNodeResult::Failed;
	
	UStudentPerceptor* Perceptor = Pawn->FindComponentByClass<UStudentPerceptor>();
	if (!Perceptor) return EBTNodeResult::Failed;
		
	AHouse* House = Cast<AHouse>(Blackboard->GetValueAsObject("TargetHouse"));
	if (!House) return EBTNodeResult::Failed;
		
	Perceptor->MarkHouseVisited(House);
	
	return EBTNodeResult::Succeeded;
}
