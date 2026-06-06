#include "BTT_LookBehind.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Pawn.h"

UBTT_LookBehind::UBTT_LookBehind()
{
	NodeName = "Look Behind";

	bNotifyTick = true;
	bNotifyTaskFinished = true;
	bCreateNodeInstance = true;
}

EBTNodeResult::Type UBTT_LookBehind::ExecuteTask(UBehaviorTreeComponent& root,uint8* nodeMemory)
{
	AAIController* AIController = root.GetAIOwner();
	if (!AIController) return EBTNodeResult::Failed;

	APawn* Pawn = AIController->GetPawn();
	if (!Pawn || !Pawn->GetWorld()) return EBTNodeResult::Failed;

	root.GetBlackboardComponent()->SetValueAsBool("IsLookingBehind", true);

	Pawn->bUseControllerRotationYaw = false;

	const FVector Forward = Pawn->GetActorForwardVector();

	HoldEndTime = Pawn->GetWorld()->GetTimeSeconds() + HoldTime;
	State = ELookBehindState::LookingBehind;

	return EBTNodeResult::InProgress;
}

void UBTT_LookBehind::TickTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory, float deltaSeconds)
{
	AAIController* AIController = ownerComp.GetAIOwner();
	if (!AIController)
	{
		FinishLatentTask(ownerComp, EBTNodeResult::Failed);
		return;
	}

	APawn* Pawn = AIController->GetPawn();
	if (!Pawn || !Pawn->GetWorld())
	{
		FinishLatentTask(ownerComp, EBTNodeResult::Failed);
		return;
	}

	switch (State)
	{
	case ELookBehindState::LookingBehind:
		{
			const float BehindYaw = GetMovementYaw(Pawn, true);
			RotatePawnTowards(Pawn, BehindYaw, deltaSeconds);

			if (Pawn->GetWorld()->GetTimeSeconds() >= HoldEndTime)
			{
				State = ELookBehindState::RotatingBack;
			}

			break;
		}

	case ELookBehindState::RotatingBack:
		{
			const float ForwardYaw = GetMovementYaw(Pawn, false);

			if (RotatePawnTowards(Pawn, ForwardYaw, deltaSeconds))
			{
				FinishLatentTask(ownerComp, EBTNodeResult::Succeeded);
			}

			break;
		}
	}
}

EBTNodeResult::Type UBTT_LookBehind::AbortTask(UBehaviorTreeComponent& root, uint8* nodeMemory)
{
	if (APawn* Pawn = root.GetAIOwner()->GetPawn())
	{
		FRotator Rotation = Pawn->GetActorRotation();
		Pawn->SetActorRotation(Rotation);
		
		Pawn->bUseControllerRotationYaw = false;
	}
	
	root.GetBlackboardComponent()->SetValueAsBool("IsLookingBehind", false);

	return EBTNodeResult::Aborted;
}

void UBTT_LookBehind::OnTaskFinished(UBehaviorTreeComponent& root, uint8* nodeMemory, EBTNodeResult::Type taskResult)
{
	Super::OnTaskFinished(root, nodeMemory, taskResult);
	
	if (APawn* Pawn = root.GetAIOwner()->GetPawn())
	{
		Pawn->bUseControllerRotationYaw = false;
	}
	
	root.GetBlackboardComponent()->SetValueAsBool("IsLookingBehind", false);
}

bool UBTT_LookBehind::RotatePawnTowards(APawn* Pawn, float DesiredYaw, float DeltaSeconds) const
{
	if (!Pawn)
		return true;

	const FRotator Current = Pawn->GetActorRotation();

	FRotator Target = Current;
	Target.Yaw = DesiredYaw;

	const FRotator NewRotation = FMath::RInterpConstantTo(Current, Target,DeltaSeconds, RotationSpeed * 360.f);

	Pawn->SetActorRotation(NewRotation);

	return FMath::Abs(FMath::FindDeltaAngleDegrees(NewRotation.Yaw, DesiredYaw)) < 1.f;
}

float UBTT_LookBehind::GetMovementYaw(APawn* Pawn, bool LookBehind) const
{
	if (!Pawn)
	{
		return 0.f;
	}

	FVector MoveDir = Pawn->GetVelocity();
	MoveDir.Z = 0.f;

	if (MoveDir.IsNearlyZero())
	{
		MoveDir = Pawn->GetActorForwardVector();
		MoveDir.Z = 0.f;
	}

	MoveDir.Normalize();

	if (LookBehind)
	{
		MoveDir *= -1.f;
	}

	return MoveDir.Rotation().Yaw;
}
