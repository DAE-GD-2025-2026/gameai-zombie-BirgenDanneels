#include "BTT_AimAtTarget.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Pawn.h"
#include "Zombies/BaseZombie.h"

UBTT_AimAtTarget::UBTT_AimAtTarget()
{
	NodeName = "Aim At Target";
	bNotifyTick = true;
	bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBTT_AimAtTarget::ExecuteTask(UBehaviorTreeComponent& root, uint8* nodeMemory)
{
	return EBTNodeResult::InProgress;
}

void UBTT_AimAtTarget::TickTask(UBehaviorTreeComponent& root, uint8* nodeMemory, float DeltaSeconds)
{
	APawn* Pawn = root.GetAIOwner()->GetPawn();
	if (!Pawn)
		FinishLatentTask(root, EBTNodeResult::Failed);
	
	UBlackboardComponent* BlackBoard = root.GetBlackboardComponent();
	if (!BlackBoard)
		FinishLatentTask(root, EBTNodeResult::Failed);
	
	ABaseZombie* Target = Cast<ABaseZombie>(BlackBoard->GetValueAsObject(TargetKey.SelectedKeyName));
	if (!IsValid(Target))
	{
		FinishLatentTask(root, EBTNodeResult::Failed);
		return;
	}

	FVector Direction = Target->GetActorLocation() - Pawn->GetActorLocation();
	Direction.Z = 0.f;

	if (Direction.IsNearlyZero())
	{
		FinishLatentTask(root, EBTNodeResult::Failed);
		return;
	}

	const FRotator DesiredRotation = Direction.Rotation();
	const FRotator CurrentRotation = Pawn->GetActorRotation();

	const FRotator NewRotation = FMath::RInterpTo(CurrentRotation,DesiredRotation,DeltaSeconds,AimRotationSpeed);
	
	Pawn->SetActorRotation(NewRotation);

	const float AngleDiff = FMath::Abs(FMath::FindDeltaAngleDegrees(Pawn->GetActorRotation().Yaw,DesiredRotation.Yaw));

	if (AngleDiff <= AcceptableAngle)
		FinishLatentTask(root, EBTNodeResult::Succeeded);
}
