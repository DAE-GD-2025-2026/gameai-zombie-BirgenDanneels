#include "BTT_Heal.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Items/BaseItem.h"
#include "AIController.h"
#include "Common/InventoryComponent.h"

UBTT_Heal::UBTT_Heal()
{
	NodeName = "Heal";
}

EBTNodeResult::Type UBTT_Heal::ExecuteTask(UBehaviorTreeComponent& root, uint8* nodeMemory)
{
	ASurvivorPawn* Pawn = Cast<ASurvivorPawn>(root.GetAIOwner()->GetPawn());
	if (!Pawn)
		return EBTNodeResult::Failed;
	
	UBlackboardComponent* Blackboard = root.GetAIOwner()->GetBlackboardComponent();
	if (!Blackboard)
		return EBTNodeResult::Failed;
	
	UInventoryComponent* Inventory = Pawn->FindComponentByClass<UInventoryComponent>();
	if (!Inventory)
		return EBTNodeResult::Failed;
	
	int slotIdx = -1;
	
	ABaseItem* HealingItem{nullptr};
	for (ABaseItem* Item : Inventory->GetInventory())
	{
		++ slotIdx;
		if (!Item)
			continue;
		
		if (Item->GetItemType() == EItemType::Medkit)
		{
			HealingItem = Item;
			break;
		}
	}
	
	if (!HealingItem)
		return EBTNodeResult::Failed;
	
	HealingItem->UseItem(*Pawn);
	
	//Dispose in case value = 0;
	if (HealingItem->GetValue() <= 0)
		Inventory->RemoveItem(slotIdx);
	
	return EBTNodeResult::Succeeded;
}
