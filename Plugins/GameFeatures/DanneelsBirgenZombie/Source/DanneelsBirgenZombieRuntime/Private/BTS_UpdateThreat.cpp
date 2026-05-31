#include "BTS_UpdateThreat.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Pawn.h"
#include "DanneelsBirgenZombieRuntime/StudentPerceptor.h"
#include "Zombies/BaseZombie.h"

UBTS_UpdateThreat::UBTS_UpdateThreat()
{
	NodeName = "Threat Detection";
	Interval = 0.25f;        // performance-friendly
	RandomDeviation = 0.05f; // avoids sync spikes
}

void UBTS_UpdateThreat::TickNode(UBehaviorTreeComponent& root, uint8* nodeMemory, float deltaSeconds)
{
	AAIController* AIController = root.GetAIOwner();
	if (!AIController) return;

	APawn* Pawn = AIController->GetPawn();
	if (!Pawn) return;

	UBlackboardComponent* Blackboard = root.GetBlackboardComponent();
	if (!Blackboard) return;

	UStudentPerceptor* Perceptor = Pawn->FindComponentByClass<UStudentPerceptor>();
	if (!Perceptor)
	{
		Blackboard->SetValueAsBool("ShouldFlee", false);
		Blackboard->ClearValue("ClosestZombie");
		return;
	}

	const FVector MyLocation = Pawn->GetActorLocation();

	const TSet<TObjectPtr<ABaseZombie>>& Zombies = Perceptor->GetZombiesSeen();
	
	// Find Closest Zombie
	float ClosestDistSq = TNumericLimits<float>::Max();
	ABaseZombie* ClosestZombie = FindClosestZombie(Zombies, MyLocation, ClosestDistSq);
	
	// Decide fleeing
	bool ShouldFlee = (ClosestZombie && ClosestDistSq <= FMath::Square(DangerRadius));

	// Write to Blackboard
	Blackboard->SetValueAsBool("ShouldFlee", ShouldFlee);

	if (ClosestZombie)
	{
		Blackboard->SetValueAsObject("ClosestZombie", ClosestZombie);
		Blackboard->SetValueAsFloat("ZombieDistance", FMath::Sqrt(ClosestDistSq));
	}
	else
	{
		Blackboard->ClearValue("ClosestZombie");
		Blackboard->ClearValue("ZombieDistance");
	}
}

ABaseZombie* UBTS_UpdateThreat::FindClosestZombie(const TSet<TObjectPtr<ABaseZombie>>& Zombies, const FVector& MyLocation, float& OutDistSq)
{
	ABaseZombie* Closest = nullptr;
	OutDistSq = TNumericLimits<float>::Max();

	for (ABaseZombie* Zombie : Zombies)
	{
		if (!Zombie) continue;

		const float DistSq =
			FVector::DistSquared(MyLocation, Zombie->GetActorLocation());

		if (DistSq < OutDistSq)
		{
			OutDistSq = DistSq;
			Closest = Zombie;
		}
	}

	return Closest;
}
