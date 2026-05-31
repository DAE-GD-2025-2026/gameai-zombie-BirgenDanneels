#include "BTT_EquipBestWeapon.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Common/InventoryComponent.h"
#include "Items/BaseItem.h"
#include "Survivor/SurvivorPawn.h"

EBTNodeResult::Type UBTT_EquipBestWeapon::ExecuteTask(UBehaviorTreeComponent& root, uint8* nodeMemory)
{
	ASurvivorPawn* Survivor = Cast<ASurvivorPawn>(root.GetAIOwner()->GetPawn());
	if (!Survivor) return EBTNodeResult::Failed;

	UBlackboardComponent* Blackboard = root.GetBlackboardComponent();
	if (!Blackboard) return EBTNodeResult::Failed;
	
	ABaseItem* Weapon = nullptr;

	const TArray<ABaseItem*>& InventoryArray = Survivor->GetComponentByClass<UInventoryComponent>()->GetInventory();
	
	int slotIdx = -1;
	for (ABaseItem* Item : InventoryArray)
	{
		++ slotIdx;
		if (!Item) continue;
		
		if (Item->GetValue() <= 0) continue;
		
		if (Item->GetItemType() == EItemType::Shotgun || Item->GetItemType() == EItemType::Pistol)
		{
			Weapon = Item;
			break;
		}
	}
	
	if (!Weapon) return EBTNodeResult::Failed;
	
	Blackboard->SetValueAsObject("EquipedWeapon", Weapon);
	
	return Super::ExecuteTask(root, nodeMemory);
}
