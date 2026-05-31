#include "BTT_Eat.h"
#include "Items/BaseItem.h"
#include "AIController.h"
#include "Common/InventoryComponent.h"

UBTT_Eat::UBTT_Eat()
{
	NodeName = "Eat";
}

EBTNodeResult::Type UBTT_Eat::ExecuteTask(UBehaviorTreeComponent& root, uint8* nodeMemory)
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
	
	ABaseItem* FoodItem{nullptr}; //Most used food item
	for (ABaseItem* Item : Inventory->GetInventory())
	{
		++ slotIdx;
		if (!Item)
			continue;
		
		if (Item->GetItemType() == EItemType::Food)
		{
			FoodItem = Item;
			break;
		}
	}
	
	if (!FoodItem)
		return EBTNodeResult::Failed;
	
	FoodItem->UseItem(*Pawn);
	
	//Dispose in case value = 0;
	if (FoodItem->GetValue() <= 0)
		Inventory->RemoveItem(slotIdx);
	
	return EBTNodeResult::Succeeded;
}
