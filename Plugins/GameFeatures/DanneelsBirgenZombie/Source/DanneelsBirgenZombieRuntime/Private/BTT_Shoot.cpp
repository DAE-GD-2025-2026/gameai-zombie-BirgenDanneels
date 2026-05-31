#include "BTT_Shoot.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Common/InventoryComponent.h"
#include "Items/BaseItem.h"
#include "Survivor/SurvivorPawn.h"
#include "Zombies/BaseZombie.h"

UBTT_Shoot::UBTT_Shoot()
{
	NodeName = "Shoot";
}

EBTNodeResult::Type UBTT_Shoot::ExecuteTask(UBehaviorTreeComponent& root, uint8* nodeMemory)
{
	AAIController* AIController = root.GetAIOwner();
	if (!AIController) return EBTNodeResult::Failed;

	ASurvivorPawn* Survivor = Cast<ASurvivorPawn>(AIController->GetPawn());
	if (!Survivor) return EBTNodeResult::Failed;

	UBlackboardComponent* Blackboard = root.GetBlackboardComponent();
	if (!Blackboard) return EBTNodeResult::Failed;
	
	ABaseItem* Weapon = Cast<ABaseItem>(Blackboard->GetValueAsObject("EquipedWeapon"));
	if (!Weapon) return EBTNodeResult::Failed;
	
	const TArray<ABaseItem*>& InventoryArray = Survivor->GetComponentByClass<UInventoryComponent>()->GetInventory();
	
	// Aim at target
	ABaseZombie* Target = Cast<ABaseZombie>(Blackboard->GetValueAsObject("TargetZombie"));
	if (!Target) return EBTNodeResult::Failed;
	
	// Get positions
	const FVector TargetLocation = Target->GetActorLocation();
	const FVector PawnLocation = Survivor->GetActorLocation();

	// Aim direction
	FVector AimDir = TargetLocation - PawnLocation;
	AimDir.Z = 0.f;

	AimDir = AimDir.GetSafeNormal();

	if (AimDir.IsNearlyZero())
	{
		return EBTNodeResult::Failed;
	}

	// Convert to rotation
	FRotator DesiredRotation = AimDir.Rotation();

	// Apply rotation
	Survivor->SetActorRotation(DesiredRotation);
	
	// Shoot
	Weapon->UseItem(*Survivor);
	
	int SlotIdx = GetItemSlot(InventoryArray, Weapon);
	if (Weapon->GetValue() <= 0)
	{
		Survivor->GetComponentByClass<UInventoryComponent>()->RemoveItem(SlotIdx);
	}
	
	return EBTNodeResult::Succeeded;
}

int UBTT_Shoot::GetItemSlot(const TArray<ABaseItem*>& Inventory, ABaseItem* SlotItem) const
{
	int SlotIdx = -1;
	
	for (ABaseItem* Item : Inventory)
	{
		++ SlotIdx;
		if (Item == SlotItem)
			return SlotIdx;
	}
	
	return -1;
}
