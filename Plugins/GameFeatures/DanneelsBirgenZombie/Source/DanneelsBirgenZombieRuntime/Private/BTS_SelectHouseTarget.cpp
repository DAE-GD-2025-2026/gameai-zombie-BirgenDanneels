#include "BTS_SelectHouseTarget.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DanneelsBirgenZombieRuntime/StudentPerceptor.h"
#include "Village/House/House.h"
#include "GameFramework/Pawn.h"

UBTS_SelectHouseTarget::UBTS_SelectHouseTarget()
{
	NodeName = "Select House Target";
	Interval = 2.0f;
	RandomDeviation = 0.3f;
}

void UBTS_SelectHouseTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* AICon = OwnerComp.GetAIOwner();
	if (!AICon) return;

	APawn* Pawn = AICon->GetPawn();
	if (!Pawn) return;

	// 1. Get your perceptor (adjust this to your setup)
	UStudentPerceptor* Perceptor = Pawn->FindComponentByClass<UStudentPerceptor>();
	if (!Perceptor) return;

	// 2. Get memory
	const TSet<TObjectPtr<AHouse>>& SeenHouses = Perceptor->GetSeenHouses();
	const TSet<TObjectPtr<AHouse>>& VisitedHouses = Perceptor->GetVisitedHouses();
	if (SeenHouses.Num() == 0) return;

	// 3. Blackboard
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB) return;

	AHouse* CurrentTarget = Cast<AHouse>(BB->GetValueAsObject("TargetHouse"));

	// 4. Pick a new house
	AHouse* NewTarget = nullptr;

	float BestDist = FLT_MAX;

	for (AHouse* House : SeenHouses)
	{
		if (!House || House == CurrentTarget) continue;

		if (VisitedHouses.Contains(House)) continue;
		
		float Dist = FVector::Dist(Pawn->GetActorLocation(), House->GetActorLocation());

		if (Dist < BestDist)
		{
			BestDist = Dist;
			NewTarget = House;
		}
	}

	if (!NewTarget) return;

	// 5. Set blackboard
	BB->SetValueAsObject("TargetHouse", NewTarget);
}