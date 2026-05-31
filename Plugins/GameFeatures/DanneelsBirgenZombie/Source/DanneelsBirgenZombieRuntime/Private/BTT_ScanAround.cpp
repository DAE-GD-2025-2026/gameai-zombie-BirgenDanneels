#include "BTT_ScanAround.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UBTT_ScanAround::UBTT_ScanAround()
{
	NodeName = "Scan Around";
	bNotifyTick = true;
}

EBTNodeResult::Type UBTT_ScanAround::ExecuteTask(UBehaviorTreeComponent& root, uint8* nodeMemory)
{
	TotalYaw = 0.f;

	return EBTNodeResult::InProgress;
}

void UBTT_ScanAround::TickTask(UBehaviorTreeComponent& root, uint8* nodeMemory, float deltaSeconds)
{
	APawn* Pawn = root.GetAIOwner()->GetPawn();
	FRotator Rot = Pawn->GetActorRotation();
	
	const float YawStep = (360.f / RotationTime) * deltaSeconds;
	Rot.Yaw += YawStep;
	Pawn->SetActorRotation(Rot);
	
	TotalYaw += YawStep;
	
	if (TotalYaw >= 360.f)
	{
		FinishLatentTask(root, EBTNodeResult::Succeeded);
	}
}
