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
	float Distance = TNumericLimits<float>::Max();
	
	for (const auto& Item : Perceptor->GetSeenLoot())
	{
		bool FitsNeeds = (PistoldNeed > 0 && Item->GetItemType() == EItemType::Pistol) 
		|| ShotgunNeed > 0 && Item->GetItemType() == EItemType::Shotgun 
		|| MedkitNeed > 0 && Item->GetItemType() == EItemType::Medkit 
		|| FoodNeed > 0 && Item->GetItemType() == EItemType::Food;
		
		if (FitsNeeds)
		{
			float ItemDistance = (Pawn->GetActorLocation() - Item->GetActorLocation()).Size();
			if (ItemDistance < Distance)
			{
				TargetItem = &Item;
				Distance = ItemDistance;
			}
		}
	}
	
	if (TargetItem)
	{
		DrawDebugSphere(
	GetWorld(),          // Current world context
	TargetItem->Get()->GetActorLocation(),        // The coordinate vector you just saved
	50.0f,               // Radius of the sphere
	12,                  // Segments (smoothness of the sphere)
	FColor::Green,       // Color of the shape
	false,               // Persistent lines (false = temporary)
	2.0f,                // Lifetime in seconds (how long it stays visible)
	0,                   // Depth priority
	2.0f                 // Thickness of the lines
);
		Blackboard->SetValueAsObject("TargetItem", *TargetItem);
		Blackboard->SetValueAsVector(ItemLocationKey.SelectedKeyName, TargetItem->Get()->GetActorLocation());
		return EBTNodeResult::Succeeded;
	}
	
	return EBTNodeResult::Failed;
}
