#include "BTT_RememberItem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Items/BaseItem.h"
#include "DanneelsBirgenZombieRuntime/StudentPerceptor.h"

UBTT_RememberItem::UBTT_RememberItem()
{
	NodeName = "Remember Item";
}

//TODO: This should be a service

EBTNodeResult::Type UBTT_RememberItem::ExecuteTask(UBehaviorTreeComponent& root, uint8* nodeMemory)
{
	UBlackboardComponent* Blackboard = root.GetAIOwner()->GetBlackboardComponent();
	if (!Blackboard) return EBTNodeResult::Failed;
	
	APawn* Pawn = root.GetAIOwner()->GetPawn();
	if (!Pawn) return EBTNodeResult::Failed;
	
	UStudentPerceptor *Perceptor = Pawn->FindComponentByClass<UStudentPerceptor>();
	if (!Perceptor) return EBTNodeResult::Failed;
	
	Perceptor->CleanUpSeenLoot();
	
	int PistoldNeed = Blackboard->GetValueAsInt("PistolNeed");
	int ShotgunNeed = Blackboard->GetValueAsInt("ShotgunNeed");
	int MedkitNeed = Blackboard->GetValueAsInt("MedkitStockNeed");
	int FoodNeed = Blackboard->GetValueAsInt("FoodStockNeed");
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("PistolNeed: %d, ShotgunNeed: %d, MedkitNeed: %d, FoodNeed: %d"), PistoldNeed, ShotgunNeed, MedkitNeed, FoodNeed));
	
	const TObjectPtr<ABaseItem>* TargetItem = nullptr;
	
	//TODO: Should be done by distance but this just finds the first item of said category in the array right now
	
	for (const auto& Item : Perceptor->GetSeenLoot())
	{
		// Implement weapon finding
		if (PistoldNeed > 0 && Item->GetItemType() == EItemType::Pistol)
		{
			TargetItem = &Item;
			 break;
		}
		else if (ShotgunNeed > 0 && Item->GetItemType() == EItemType::Shotgun)
		{
			TargetItem = &Item;
			 break;
		}
		else if (MedkitNeed > 0 && Item->GetItemType() == EItemType::Medkit)
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
