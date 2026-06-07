#include "BTT_ScanAroundDanneelsBirgen.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UBTT_ScanAroundDanneelsBirgen::UBTT_ScanAroundDanneelsBirgen()
{
	NodeName = "Scan Around";
	bNotifyTick = true;
}

EBTNodeResult::Type UBTT_ScanAroundDanneelsBirgen::ExecuteTask(UBehaviorTreeComponent& root, uint8* nodeMemory)
{
	TotalYaw = 0.f;

	return EBTNodeResult::InProgress;
}

void UBTT_ScanAroundDanneelsBirgen::TickTask(UBehaviorTreeComponent& root, uint8* nodeMemory, float deltaSeconds)
{
	AAIController* AIController = root.GetAIOwner();
	if (!AIController)
	{
		FinishLatentTask(root, EBTNodeResult::Failed);
		return;
	}

	APawn* Pawn = AIController->GetPawn();
	if (!Pawn)
	{
		FinishLatentTask(root, EBTNodeResult::Failed);
		return;
	}

	FRotator Rot = AIController->GetControlRotation();
	
	const float YawStep = (360.f / RotationTime) * deltaSeconds;

	Rot.Yaw += YawStep;
	Rot.Pitch = 0.f;
	Rot.Roll = 0.f;

	const FVector LookPoint =
		Pawn->GetActorLocation() + Rot.Vector() * 10000.f;

	AIController->ClearFocus(EAIFocusPriority::Move);
	AIController->SetFocalPoint(LookPoint, EAIFocusPriority::Gameplay);
	AIController->SetControlRotation(Rot);

	TotalYaw += FMath::Abs(YawStep);
	
	if (TotalYaw >= 360.f)
	{
		AIController->ClearFocus(EAIFocusPriority::Gameplay);
		FinishLatentTask(root, EBTNodeResult::Succeeded);
	}
}

EBTNodeResult::Type UBTT_ScanAroundDanneelsBirgen::AbortTask(UBehaviorTreeComponent& root, uint8* nodeMemory)
{
	if (AAIController* AIController = root.GetAIOwner())
	{
		AIController->ClearFocus(EAIFocusPriority::Gameplay);
	}
	
	return EBTNodeResult::Aborted;
}
