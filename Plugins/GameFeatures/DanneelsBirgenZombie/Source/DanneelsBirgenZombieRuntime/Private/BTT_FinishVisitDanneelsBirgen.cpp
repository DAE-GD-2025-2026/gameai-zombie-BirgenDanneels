#include "BTT_FinishVisitDanneelsBirgen.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DanneelsBirgenZombieRuntime/StudentPerceptorDanneelsBirgen.h"
#include "Village/House/House.h"

UBTT_FinishVisitDanneelsBirgen::UBTT_FinishVisitDanneelsBirgen()
{
}

EBTNodeResult::Type UBTT_FinishVisitDanneelsBirgen::ExecuteTask(UBehaviorTreeComponent& root, uint8* nodeMemory)
{
	APawn* Pawn = root.GetAIOwner()->GetPawn();
	if (!Pawn) return EBTNodeResult::Failed;
	
	UBlackboardComponent* Blackboard = root.GetAIOwner()->GetBlackboardComponent();
	if (!Blackboard) return EBTNodeResult::Failed;
	
	UStudentPerceptorDanneelsBirgen* Perceptor = Pawn->FindComponentByClass<UStudentPerceptorDanneelsBirgen>();
	if (!Perceptor) return EBTNodeResult::Failed;
		
	AHouse* House = Cast<AHouse>(Blackboard->GetValueAsObject("TargetHouse"));
	if (!House) return EBTNodeResult::Failed;
	
	const FHouseBounds Bounds = House->GetBounds();

	const FVector PawnLocation = Pawn->GetActorLocation();

	const bool bInsideBounds =
		FMath::Abs(PawnLocation.X - Bounds.Origin.X) <= Bounds.Extent.X &&
		FMath::Abs(PawnLocation.Y - Bounds.Origin.Y) <= Bounds.Extent.Y &&
		FMath::Abs(PawnLocation.Z - Bounds.Origin.Z) <= Bounds.Extent.Z;
	
	if (bInsideBounds)
	{
		Perceptor->MarkHouseVisited(House);
		Blackboard->ClearValue("TargetHouse");
		
		return EBTNodeResult::Succeeded;
	}
	
	return EBTNodeResult::Failed;
}
