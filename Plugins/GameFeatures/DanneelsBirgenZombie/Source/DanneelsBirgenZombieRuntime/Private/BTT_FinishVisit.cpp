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
		
	Perceptor->VisitHouse(House);

	// Look For next possible house
	const TSet<TObjectPtr<AHouse>>& SeenHouses = Perceptor->GetSeenHouses();
	const TSet<TObjectPtr<AHouse>>& VisitedHouses = Perceptor->GetVisitedHouses();
	AHouse* NewTarget = nullptr;

	float BestDist = FLT_MAX;

	for (AHouse* TempHouse : SeenHouses)
	{
		if (!TempHouse) continue;

		if (VisitedHouses.Contains(TempHouse)) continue;
		
		float Dist = FVector::Dist(Pawn->GetActorLocation(), TempHouse->GetActorLocation());

		if (Dist < BestDist)
		{
			BestDist = Dist;
			NewTarget = TempHouse;
		}
	}

	
	Blackboard->SetValueAsObject("TargetHouse", NewTarget);
	
	return EBTNodeResult::Succeeded;
}
