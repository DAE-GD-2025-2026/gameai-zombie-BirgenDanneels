#include "BTS_UpdateThreatDanneelsBirgen.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Pawn.h"
#include "DanneelsBirgenZombieRuntime/StudentPerceptorDanneelsBirgen.h"
#include "Zombies/BaseZombie.h"
#include "Survivor/SurvivorPawn.h"

UBTS_UpdateThreatDanneelsBirgen::UBTS_UpdateThreatDanneelsBirgen()
{
	NodeName = "Threat Detection";
	Interval = 0.25f;
	RandomDeviation = 0.05f;
}

void UBTS_UpdateThreatDanneelsBirgen::TickNode(UBehaviorTreeComponent& root, uint8* nodeMemory, float deltaSeconds)
{
	Super::TickNode(root, nodeMemory, deltaSeconds);

	AAIController* AIController = root.GetAIOwner();
	if (!AIController) return;

	APawn* Pawn = AIController->GetPawn();
	if (!Pawn) return;

	ASurvivorPawn* Survivor = Cast<ASurvivorPawn>(Pawn);

	UBlackboardComponent* Blackboard = root.GetBlackboardComponent();
	if (!Blackboard) return;

	UStudentPerceptorDanneelsBirgen* Perceptor = Pawn->FindComponentByClass<UStudentPerceptorDanneelsBirgen>();
	if (!Perceptor)
	{
		Blackboard->SetValueAsBool("ShouldFlee", false);
		Blackboard->ClearValue("ClosestZombie");
		Blackboard->ClearValue("ZombieDistance");

		if (Survivor && Survivor->IsRunning())
		{
			Survivor->StopRunning();
		}

		return;
	}

	const FVector MyLocation = Pawn->GetActorLocation();
	
	const TArray<ABaseZombie*>& Zombies = Perceptor->GetZombiesSeen();

	float ClosestDistSq = TNumericLimits<float>::Max();
	ABaseZombie* ClosestZombie = FindClosestZombie(Zombies, MyLocation, ClosestDistSq);

	if (!ClosestZombie || !IsValid(ClosestZombie))
	{
		Blackboard->SetValueAsBool("ShouldFlee", false);
		Blackboard->ClearValue("ClosestZombie");
		Blackboard->ClearValue("ZombieDistance");

		if (Survivor && Survivor->IsRunning())
		{
			Survivor->StopRunning();
		}

		return;
	}

	const float ZombieDistance = FMath::Sqrt(ClosestDistSq);

	Blackboard->SetValueAsObject("ClosestZombie", ClosestZombie);
	Blackboard->SetValueAsFloat("ZombieDistance", ZombieDistance);
	
	// Flee state
	const bool bWasFleeing = Blackboard->GetValueAsBool("ShouldFlee");

	bool bShouldFlee = bWasFleeing;

	if (!bWasFleeing && ZombieDistance <= StartFleeRadius)
	{
		bShouldFlee = true;
	}
	else if (bWasFleeing && ZombieDistance >= StopFleeRadius)
	{
		bShouldFlee = false;
	}

	Blackboard->SetValueAsBool("ShouldFlee", bShouldFlee);
	
	// Sprint state
	if (Survivor)
	{
		if (ZombieDistance <= StartSprintRadius)
		{
			if (!Survivor->IsRunning())
			{
				Survivor->StartRunning();
			}
		}
		else if (ZombieDistance >= StopSprintRadius)
		{
			if (Survivor->IsRunning())
			{
				Survivor->StopRunning();
			}
		}
	}
}

ABaseZombie* UBTS_UpdateThreatDanneelsBirgen::FindClosestZombie(const TArray<ABaseZombie*>& Zombies, const FVector& MyLocation, float& OutDistSq)
{
	ABaseZombie* Closest = nullptr;
	OutDistSq = TNumericLimits<float>::Max();

	for (ABaseZombie* Zombie : Zombies)
	{
		if (!IsValid(Zombie)) continue;

		const float DistSq = FVector::DistSquared(
			MyLocation,
			Zombie->GetActorLocation()
		);

		if (DistSq < OutDistSq)
		{
			OutDistSq = DistSq;
			Closest = Zombie;
		}
	}

	return Closest;
}