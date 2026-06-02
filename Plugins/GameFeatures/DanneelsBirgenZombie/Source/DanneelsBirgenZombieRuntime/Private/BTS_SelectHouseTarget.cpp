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
	
	UStudentPerceptor* Perceptor = Pawn->FindComponentByClass<UStudentPerceptor>();
	if (!Perceptor) return;
	
	const auto& SeenHouses = Perceptor->GetVisitedHouses();
	if (SeenHouses.Num() == 0) return;
	
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (!Blackboard) return;

	// Pick a closest house visited before last pickup time
	float LastPickupTime = Blackboard->GetValueAsFloat("ItemPickUpTime");
	AHouse* NewTarget = nullptr;
	
	float BestDist = FLT_MAX;

	for (const TPair<TObjectPtr<AHouse>, float>& HousePair : SeenHouses)
	{
		AHouse* House = HousePair.Key;
		if (!House) continue;
		
		if (HousePair.Value > LastPickupTime) continue; //Temporarily check if the value is 0 to know if it was never checked. This will be changed later
		
		float Dist = FVector::Dist(Pawn->GetActorLocation(), House->GetActorLocation());

		if (Dist < BestDist)
		{
			BestDist = Dist;
			NewTarget = House;
		}
	}

	// 5. Set blackboard
	Blackboard->SetValueAsObject("TargetHouse", NewTarget);
}