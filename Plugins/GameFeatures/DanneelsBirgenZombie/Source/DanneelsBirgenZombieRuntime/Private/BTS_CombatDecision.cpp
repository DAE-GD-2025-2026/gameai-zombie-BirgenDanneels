#include "BTS_CombatDecision.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Common/InventoryComponent.h"
#include "DanneelsBirgenZombieRuntime/StudentPerceptor.h"
#include "Items/BaseItem.h"
#include "Items/ItemType.h"
#include "Survivor/SurvivorPawn.h"
#include "Zombies/BaseZombie.h"
#include "Engine/World.h"

UBTS_CombatDecision::UBTS_CombatDecision()
{
	NodeName = "Combat Decision";

	Interval = 0.20f;
	RandomDeviation = 0.05f;
	bNotifyCeaseRelevant = true;
}

void UBTS_CombatDecision::TickNode(UBehaviorTreeComponent& root, uint8* nodeMemory, float deltaSeconds)
{
	Super::TickNode(root, nodeMemory, deltaSeconds);

	AAIController* AIController = root.GetAIOwner();
	if (!AIController) return;

	APawn* Pawn = AIController->GetPawn();
	if (!Pawn) return;

	UBlackboardComponent* BlackBoard = root.GetBlackboardComponent();
	if (!BlackBoard) return;

	ABaseZombie* Target = FindBestTarget(Pawn);

	if (!Target)
	{
		BlackBoard->ClearValue(TargetKey.SelectedKeyName);
		BlackBoard->ClearValue(WeaponKey.SelectedKeyName);
		BlackBoard->SetValueAsBool(HasLineOfSightKey.SelectedKeyName, false);
		BlackBoard->SetValueAsBool(ShouldShootKey.SelectedKeyName, false);
		return;
	}

	const float Dist = FVector::Dist2D(Pawn->GetActorLocation(), Target->GetActorLocation());
	const bool bInRange = Dist <= MaxShootRange;
	const bool bLOS = bInRange && HasLOS(Pawn, Target);

	const int CloseZombieCount = CountZombiesNearTarget(Pawn, Target, ShotgunClusterRadius);
	ABaseItem* BestWeapon = ChooseBestWeapon(Pawn, Target, CloseZombieCount);

	BlackBoard->SetValueAsObject(TargetKey.SelectedKeyName, Target);
	BlackBoard->SetValueAsObject(WeaponKey.SelectedKeyName, BestWeapon);
	BlackBoard->SetValueAsBool(HasLineOfSightKey.SelectedKeyName, bLOS);
	BlackBoard->SetValueAsBool(ShouldShootKey.SelectedKeyName, bLOS && BestWeapon != nullptr);
	
	if (!root.GetBlackboardComponent()->GetValueAsBool("IsLookingBehind"))
	{
		const bool bShouldManualAim = bLOS && (bLOS && BestWeapon != nullptr);
		Pawn->bUseControllerRotationYaw = !bShouldManualAim;
	}
}

void UBTS_CombatDecision::OnCeaseRelevant(UBehaviorTreeComponent& root, uint8* nodeMemory)
{
	Super::OnCeaseRelevant(root, nodeMemory);
	
	APawn* Pawn = root.GetAIOwner() ? root.GetAIOwner()->GetPawn() : nullptr;
	if (!Pawn) return;

	Pawn->bUseControllerRotationYaw = true;
}

//Finds closest target
ABaseZombie* UBTS_CombatDecision::FindBestTarget(const APawn* Pawn) const
{
	if (!Pawn) return nullptr;

	UStudentPerceptor* Perceptor = Pawn->FindComponentByClass<UStudentPerceptor>();
	if (!Perceptor) return nullptr;

	ABaseZombie* BestTarget = nullptr;
	float BestDistSq = TNumericLimits<float>::Max();

	const FVector PawnLocation = Pawn->GetActorLocation();

	for (ABaseZombie* Zombie : Perceptor->GetZombiesSeen())
	{
		if (!IsValid(Zombie)) continue;

		const float DistSq = FVector::DistSquared2D(PawnLocation, Zombie->GetActorLocation());

		if (DistSq < BestDistSq)
		{
			BestDistSq = DistSq;
			BestTarget = Zombie;
		}
	}

	return BestTarget;
}

bool UBTS_CombatDecision::HasLOS(const APawn* Pawn, const ABaseZombie* Target) const
{
	if (!Pawn || !Target) return false;

	UWorld* World = Pawn->GetWorld();
	if (!World) return false;

	FVector Start = Pawn->GetActorLocation();
	FVector End = Target->GetActorLocation();

	Start.Z += 60.f;
	End.Z += 60.f;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Pawn);
	Params.AddIgnoredActor(Target);
	Params.bTraceComplex = false;

	FHitResult Hit;

	const bool bHit = World->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params);

	return !bHit;
}

int UBTS_CombatDecision::CountZombiesNearTarget(const APawn* Pawn, const ABaseZombie* Target, float Radius) const
{
	if (!Pawn || !Target) return 0;

	UStudentPerceptor* Perceptor = Pawn->FindComponentByClass<UStudentPerceptor>();
	if (!Perceptor) return 0;

	int Count = 0;
	const FVector TargetLocation = Target->GetActorLocation();

	for (ABaseZombie* Zombie : Perceptor->GetZombiesSeen())
	{
		if (!IsValid(Zombie)) continue;

		const float Dist = FVector::Dist2D(TargetLocation, Zombie->GetActorLocation());

		if (Dist <= Radius)
		{
			++Count;
		}
	}

	return Count;
}

ABaseItem* UBTS_CombatDecision::ChooseBestWeapon(APawn* Pawn, ABaseZombie* Target, int CloseZombieCount) const
{
	if (!Pawn) return nullptr;

	ASurvivorPawn* Survivor = Cast<ASurvivorPawn>(Pawn);
	if (!Survivor) return nullptr;

	UInventoryComponent* Inventory = Survivor->FindComponentByClass<UInventoryComponent>();
	if (!Inventory) return nullptr;

	ABaseItem* Pistol = nullptr;
	ABaseItem* Shotgun = nullptr;

	const TArray<ABaseItem*>& Items = Inventory->GetInventory();

	for (ABaseItem* Item : Items)
	{
		if (!IsValid(Item)) continue;
		if (Item->GetValue() <= 0) continue;

		if (Item->GetItemType() == EItemType::Pistol)
		{
			Pistol = Item;
		}
		else if (Item->GetItemType() == EItemType::Shotgun)
		{
			Shotgun = Item;
		}
	}

	const bool bPreferShotgun = CloseZombieCount >= ShotgunMinZombieCount;

	if (bPreferShotgun && Shotgun)
	{
		return Shotgun;
	}

	if (Pistol)
	{
		return Pistol;
	}

	return Shotgun;
}