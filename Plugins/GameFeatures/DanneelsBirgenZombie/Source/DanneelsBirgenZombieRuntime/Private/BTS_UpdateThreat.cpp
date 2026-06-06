#include "BTS_UpdateThreat.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Pawn.h"
#include "DanneelsBirgenZombieRuntime/StudentPerceptor.h"
#include "Zombies/BaseZombie.h"
#include "Survivor/SurvivorPawn.h"

UBTS_UpdateThreat::UBTS_UpdateThreat()
{
	NodeName = "Threat Detection";
	Interval = 0.25f;
	RandomDeviation = 0.05f;
}

void UBTS_UpdateThreat::TickNode(UBehaviorTreeComponent& root, uint8* nodeMemory, float deltaSeconds)
{
	Super::TickNode(root, nodeMemory, deltaSeconds);

	AAIController* AIController = root.GetAIOwner();
	if (!AIController) return;

	APawn* Pawn = AIController->GetPawn();
	if (!Pawn) return;

	ASurvivorPawn* Survivor = Cast<ASurvivorPawn>(Pawn);

	UBlackboardComponent* Blackboard = root.GetBlackboardComponent();
	if (!Blackboard) return;

	UStudentPerceptor* Perceptor = Pawn->FindComponentByClass<UStudentPerceptor>();
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
	const bool bShouldFlee = ZombieDistance <= DangerRadius;
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

	if (GEngine)
	{
		const FString DebugText = FString::Printf(
			TEXT("Zombie Dist: %.0f | Flee: %s | Running: %s"),
			ZombieDistance,
			bShouldFlee ? TEXT("true") : TEXT("false"),
			Survivor && Survivor->IsRunning() ? TEXT("true") : TEXT("false")
		);

		GEngine->AddOnScreenDebugMessage(
			5,
			0.25f,
			bShouldFlee ? FColor::Red : FColor::Yellow,
			DebugText
		);
	}
}

ABaseZombie* UBTS_UpdateThreat::FindClosestZombie(const TArray<ABaseZombie*>& Zombies, const FVector& MyLocation, float& OutDistSq)
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