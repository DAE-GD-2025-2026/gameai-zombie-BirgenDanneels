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
	
	ABaseItem* Weapon = nullptr;
	// Equip gun (select gun, should be done by another task or service)
	// For now just equip first gun in inventory
	//Implement pickup range
	const TArray<ABaseItem*>& InventoryArray = Survivor->GetComponentByClass<UInventoryComponent>()->GetInventory();
	
	for (ABaseItem* Item : InventoryArray)
	{
		if (!Item) continue;
		
		if (Item->GetItemType() == EItemType::Shotgun || Item->GetItemType() == EItemType::Pistol)
		{
			Weapon = Item;
			break;
		}
	}
	
	if (!Weapon) return EBTNodeResult::Failed;
	
	// Aim at target
	ABaseZombie* Target = Cast<ABaseZombie>(Blackboard->GetValueAsObject("TargetZombie"));
	if (!Target) return EBTNodeResult::Failed;
	
	// Get positions
	const FVector TargetLocation = Target->GetActorLocation();
	const FVector PawnLocation = Survivor->GetActorLocation();

	// Aim direction (flat or full 3D depending on your game)
	FVector AimDir = TargetLocation - PawnLocation;
	AimDir.Z = 0.f; // remove this if you want vertical aiming

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
	
	return EBTNodeResult::Succeeded;
}

EBTNodeResult::Type UBTT_Shoot::AbortTask(UBehaviorTreeComponent& root, uint8* nodeMemory)
{
	return Super::AbortTask(root, nodeMemory);
}
