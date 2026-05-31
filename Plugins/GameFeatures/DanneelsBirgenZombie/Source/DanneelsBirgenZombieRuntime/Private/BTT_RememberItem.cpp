#include "BTT_RememberItem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Items/BaseItem.h"
#include "DanneelsBirgenZombieRuntime/StudentPerceptor.h"

UBTT_RememberItem::UBTT_RememberItem()
{
	NodeName = "Remember Item";
}

EBTNodeResult::Type UBTT_RememberItem::ExecuteTask(UBehaviorTreeComponent& root, uint8* nodeMemory)
{
	UBlackboardComponent* Blackboard = root.GetAIOwner()->GetBlackboardComponent();
	if (!Blackboard) return EBTNodeResult::Failed;
	
	APawn* Pawn = root.GetAIOwner()->GetPawn();
	if (!Pawn) return EBTNodeResult::Failed;
	
	UStudentPerceptor *Perceptor = Pawn->FindComponentByClass<UStudentPerceptor>();
	if (!Perceptor) return EBTNodeResult::Failed;
	
	Perceptor->CleanUpSeenLoot();
	
	int AmmoNeed = Blackboard->GetValueAsInt("AmmoStockNeed");
	int MedkitNeed = Blackboard->GetValueAsInt("MedkitStockNeed");
	int FoodNeed = Blackboard->GetValueAsInt("FoodStockNeed");
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("AmmoNeed: %d, MedkitNeed: %d, FoodNeed: %d"), AmmoNeed, MedkitNeed, FoodNeed));
	
	const TObjectPtr<ABaseItem>* TargetItem = nullptr;
	
	for (const auto& Item : Perceptor->GetSeenLoot())
	{
		// Implement weapon finding
		if (MedkitNeed > 0 && Item->GetItemType() == EItemType::Medkit)
		{
			TargetItem = &Item;
			break;
		}
		else if (FoodNeed > 0 && Item->GetItemType() == EItemType::Food)
		{
			TargetItem = &Item;
			break;
		}
	}
	
	if (TargetItem)
	{
		Blackboard->SetValueAsObject("TargetItem", *TargetItem);
		Blackboard->SetValueAsVector("ItemLocation", TargetItem->Get()->GetActorLocation());
		return EBTNodeResult::Succeeded;
	}
	
	return EBTNodeResult::Failed;
}
