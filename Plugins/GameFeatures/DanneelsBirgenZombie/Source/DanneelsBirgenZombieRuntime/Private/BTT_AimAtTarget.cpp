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

	UBlackboardComponent* BlackBoard = root.GetBlackboardComponent();
	if (!BlackBoard)
	{
		FinishLatentTask(root, EBTNodeResult::Failed);
		return;
	}

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

	FRotator DesiredRotation = Direction.Rotation();
	DesiredRotation.Pitch = 0.f;
	DesiredRotation.Roll = 0.f;

	const FRotator CurrentRotation = AIController->GetControlRotation();

	const FRotator NewRotation = FMath::RInterpTo(CurrentRotation, DesiredRotation, DeltaSeconds, AimRotationSpeed);

	const FVector LookPoint = Pawn->GetActorLocation() + NewRotation.Vector() * 10000.f;

	// Do not let MoveTo focus own rotation.
	AIController->ClearFocus(EAIFocusPriority::Move);
	
	AIController->SetFocalPoint(LookPoint, EAIFocusPriority::Gameplay);

	const float AngleDiff = FMath::Abs(FMath::FindDeltaAngleDegrees(NewRotation.Yaw, DesiredRotation.Yaw));

	if (AngleDiff <= AcceptableAngle)
	{
		FinishLatentTask(root, EBTNodeResult::Succeeded);
	}
}
