#include "BTT_LookBehindDanneelsBirgen.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Pawn.h"

UBTT_LookBehindDanneelsBirgen::UBTT_LookBehindDanneelsBirgen()
{
	NodeName = "Look Behind";

	bNotifyTick = true;
	bNotifyTaskFinished = true;
	bCreateNodeInstance = true;
}

EBTNodeResult::Type UBTT_LookBehindDanneelsBirgen::ExecuteTask(UBehaviorTreeComponent& root,uint8* nodeMemory)
{
	AAIController* AIController = root.GetAIOwner();
	if (!AIController) return EBTNodeResult::Failed;
	
	APawn* Pawn = AIController->GetPawn();
	if (!Pawn || !Pawn->GetWorld()) return EBTNodeResult::Failed;

	root.GetBlackboardComponent()->SetValueAsBool("IsLookingBehind", true);
	
	HoldEndTime = Pawn->GetWorld()->GetTimeSeconds() + HoldTime;
	State = ELookBehindState::LookingBehind;

	return EBTNodeResult::InProgress;
}

void UBTT_LookBehindDanneelsBirgen::TickTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory, float deltaSeconds)
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

EBTNodeResult::Type UBTT_LookBehindDanneelsBirgen::AbortTask(UBehaviorTreeComponent& root, uint8* nodeMemory)
{
	if (AAIController* AIController = root.GetAIOwner())
	{
		AIController->ClearFocus(EAIFocusPriority::Gameplay);
	}

	root.GetBlackboardComponent()->SetValueAsBool("IsLookingBehind", false);

	return EBTNodeResult::Aborted;
}

void UBTT_LookBehindDanneelsBirgen::OnTaskFinished(UBehaviorTreeComponent& root, uint8* nodeMemory, EBTNodeResult::Type taskResult)
{
	Super::OnTaskFinished(root, nodeMemory, taskResult);
	
	if (AAIController* AIController = root.GetAIOwner())
	{
		AIController->ClearFocus(EAIFocusPriority::Gameplay);
	}

	root.GetBlackboardComponent()->SetValueAsBool("IsLookingBehind", false);
}

bool UBTT_LookBehindDanneelsBirgen::RotatePawnTowards(APawn* Pawn, float DesiredYaw, float DeltaSeconds) const
{
	if (!Pawn)
		return true;

	AAIController* AIController = Pawn->GetController<AAIController>();
	if (!AIController)
		return true;

	// Do not let MoveTo focus own rotation.
	AIController->ClearFocus(EAIFocusPriority::Move);

	const FRotator Current = AIController->GetControlRotation();
	FRotator Target = {0, DesiredYaw, 0};
	
	const FRotator NewRotation = FMath::RInterpConstantTo(Current, Target, DeltaSeconds, RotationSpeed * 360.f);
	const FVector LookPoint = Pawn->GetActorLocation() + NewRotation.Vector() * 10000.f;
	
	AIController->SetFocalPoint(LookPoint, EAIFocusPriority::Gameplay);

	return FMath::Abs(FMath::FindDeltaAngleDegrees(NewRotation.Yaw, DesiredYaw)) < 1.f;
}

float UBTT_LookBehindDanneelsBirgen::GetMovementYaw(APawn* Pawn, bool LookBehind) const
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
