#include "BTT_PickUpLoot.h"
#include "Items/BaseItem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Common/InventoryComponent.h"

UBTT_PickUpLoot::UBTT_PickUpLoot()
{
	NodeName = "Pick Up Loot";
}

EBTNodeResult::Type UBTT_PickUpLoot::ExecuteTask(UBehaviorTreeComponent& root, uint8* nodeMemory)
{
	
	AAIController* AIController = root.GetAIOwner();
	if (!AIController) return EBTNodeResult::Failed;

	ASurvivorPawn* Survivor = Cast<ASurvivorPawn>(AIController->GetPawn());
	if (!Survivor) return EBTNodeResult::Failed;
	
	UBlackboardComponent* Blackboard = root.GetBlackboardComponent();
	if (!Blackboard) return EBTNodeResult::Failed;
	
	ABaseItem* TargetItem = Cast<ABaseItem>(Blackboard->GetValueAsObject("TargetItem"));
	if (!TargetItem) return EBTNodeResult::Failed;
	
	UInventoryComponent* Inventory = Survivor->GetComponentByClass<UInventoryComponent>();
	if (!Inventory) return EBTNodeResult::Failed;
	
	const float DistanceToItem = (TargetItem->GetActorLocation() - Survivor->GetActorLocation()).Size();
	if (DistanceToItem > Inventory->GetPickupRange())
		return EBTNodeResult::Failed;
	
	const TArray<ABaseItem*>& InventoryArray = Inventory->GetInventory();
	int TargetFreeSlot = GetFreeInventorySlot(InventoryArray);
	
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("TargetFreeSlot: %d"), TargetFreeSlot));
	if (TargetFreeSlot == -1) //inventory full (should be done by a service)
	{
		Blackboard->ClearValue("TargetItem");
		return EBTNodeResult::Failed;
	}
	
	if (Survivor->GetComponentByClass<UInventoryComponent>()->GrabItem(TargetFreeSlot, TargetItem))
	{
		Blackboard->ClearValue("TargetItem");
		return EBTNodeResult::Succeeded;
	}
	
	return EBTNodeResult::Failed;
}

int UBTT_PickUpLoot::GetFreeInventorySlot(const TArray<ABaseItem*>& Inventory) const
{
	for (int i = 0; i < Inventory.Num(); ++i)
	{
		if (Inventory[i] == nullptr)
		{
			return  i;
		}
	}
	
	return -1;
}
