#include "BTS_AutoConsume.h"

#include "AIController.h"
#include "Common/InventoryComponent.h"
#include "Items/BaseItem.h"
#include "Items/ItemType.h"
#include "Survivor/SurvivorPawn.h"

UBTS_AutoConsume::UBTS_AutoConsume()
{
	NodeName = "AutoConsume";
}

void UBTS_AutoConsume::TickNode(UBehaviorTreeComponent& root, uint8* nodeMemory, float deltaSeconds)
{
	AAIController* AIController = root.GetAIOwner();
	if (!AIController) return;

	ASurvivorPawn* Pawn = Cast<ASurvivorPawn>(AIController->GetPawn());
	if (!Pawn) return;

	UInventoryComponent* Inventory = Pawn->GetComponentByClass<UInventoryComponent>();
	UHealthComponent* HealthComponent = Pawn->GetComponentByClass<UHealthComponent>();
	UStaminaComponent* StaminaComponent = Pawn->GetComponentByClass<UStaminaComponent>();

	// Get needed hp and stamina and the most fitting medkit and food
	int NeededHealth = HealthComponent->GetMaxHealth() - HealthComponent->GetHealth();
	int NeededStamina = StaminaComponent->GetMaxStamina() - StaminaComponent->GetCurrentStamina();
	
	if (NeededHealth > 0)
		TryConsumeItem(Pawn, EItemType::Medkit, NeededHealth);
	
	if (NeededStamina > 0)
		TryConsumeItem(Pawn, EItemType::Food, NeededStamina);
}

bool UBTS_AutoConsume::TryConsumeItem(ASurvivorPawn* Pawn, EItemType Type, int NeededValue)
{
	UInventoryComponent* Inventory = Pawn->GetComponentByClass<UInventoryComponent>();
	if (!Inventory) return false;
	
	int SlotIdx = -1;
	
	ABaseItem* BestItem = nullptr; // Best item is the item closest to the needed value that does not exceed it
	int ItemSlotIdx = -1;
	
	for (ABaseItem* Item : Inventory->GetInventory())
	{
		++SlotIdx;

		if (!Item)
			continue;

		if (Item->GetItemType() != Type)
			continue;

		if (Item->GetValue() < NeededValue && (BestItem == nullptr || Item->GetValue() > BestItem->GetValue()))
		{
			BestItem = Item;
			ItemSlotIdx = SlotIdx;
		}
	}
	
	if (BestItem)
	{
		BestItem->UseItem(*Pawn);
		
		if (BestItem->GetValue() <= 0)
			Inventory->RemoveItem(ItemSlotIdx);
		
		return true;
	}
	
	return false;
}
