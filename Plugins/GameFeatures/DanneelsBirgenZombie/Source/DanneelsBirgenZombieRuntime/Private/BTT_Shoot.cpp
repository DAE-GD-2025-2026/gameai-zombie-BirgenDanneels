#include "BTT_Shoot.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Common/InventoryComponent.h"
#include "Items/BaseItem.h"
#include "Survivor/SurvivorPawn.h"

UBTT_Shoot::UBTT_Shoot()
{
	NodeName = "Shoot";
	bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBTT_Shoot::ExecuteTask(UBehaviorTreeComponent& root, uint8* nodeMemory)
{
	AAIController* AIController = root.GetAIOwner();
	if (!AIController) return EBTNodeResult::Failed;

	ASurvivorPawn* Survivor = Cast<ASurvivorPawn>(AIController->GetPawn());
	if (!Survivor) return EBTNodeResult::Failed;

	UBlackboardComponent* BlackBoard = root.GetBlackboardComponent();
	if (!BlackBoard) return EBTNodeResult::Failed;

	if (!BlackBoard->GetValueAsBool(ShouldShootKey.SelectedKeyName))
	{
		return EBTNodeResult::Failed;
	}

	ABaseItem* Weapon = Cast<ABaseItem>(BlackBoard->GetValueAsObject(WeaponKey.SelectedKeyName));
	if (!IsValid(Weapon)) return EBTNodeResult::Failed;

	UInventoryComponent* Inventory = Survivor->FindComponentByClass<UInventoryComponent>();
	if (!Inventory) return EBTNodeResult::Failed;

	if (Weapon->GetValue() <= 0)
	{
		return EBTNodeResult::Failed;
	}

	Weapon->UseItem(*Survivor);

	if (Weapon->GetValue() <= 0)
	{
		const int32 SlotIdx = GetItemSlot(Inventory->GetInventory(), Weapon);

		if (SlotIdx != -1)
		{
			Inventory->RemoveItem(SlotIdx);
		}

		BlackBoard->ClearValue(WeaponKey.SelectedKeyName);
		BlackBoard->SetValueAsBool(ShouldShootKey.SelectedKeyName, false);
	}

	return EBTNodeResult::Succeeded;
}

void UBTT_Shoot::OnTaskFinished(UBehaviorTreeComponent& root, uint8* nodeMemory, EBTNodeResult::Type result)
{
	APawn* Pawn = Cast<APawn>(root.GetAIOwner()->GetPawn());
	
	if (Pawn)
	{
		Pawn->bUseControllerRotationYaw = true;
	}
}

int UBTT_Shoot::GetItemSlot(const TArray<ABaseItem*>& Inventory, ABaseItem* SlotItem) const
{
	for (int32 i = 0; i < Inventory.Num(); ++i)
	{
		if (Inventory[i] == SlotItem)
		{
			return i;
		}
	}

	return -1;
}


