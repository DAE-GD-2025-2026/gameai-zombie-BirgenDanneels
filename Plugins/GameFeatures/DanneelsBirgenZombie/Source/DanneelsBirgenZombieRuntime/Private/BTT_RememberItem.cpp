#include "BTT_RememberItem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Items/BaseItem.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
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
	
	int PistoldNeed = Blackboard->GetValueAsInt("PistolNeed");
	int ShotgunNeed = Blackboard->GetValueAsInt("ShotgunNeed");
	int MedkitNeed = Blackboard->GetValueAsInt("MedkitStockNeed");
	int FoodNeed = Blackboard->GetValueAsInt("FoodStockNeed");
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("PistolNeed: %d, ShotgunNeed: %d, MedkitNeed: %d, FoodNeed: %d"), PistoldNeed, ShotgunNeed, MedkitNeed, FoodNeed));
	
	TObjectPtr<ABaseItem> TargetItem = nullptr;
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
				TargetItem = Item;
				Distance = ItemDistance;
			}
		}
	}
	
	if (TargetItem)
	{
		ABaseItem* Item = TargetItem;

		// Find reachable point near item
		FVector ReachableItemLocation;
		if (!FindReachableLocationNearItem(Pawn, Item->GetActorLocation(), ReachableItemLocation))
		{
			return EBTNodeResult::Failed;
		}

		Blackboard->SetValueAsObject("TargetItem", Item);
		Blackboard->SetValueAsVector(ItemLocationKey.SelectedKeyName, ReachableItemLocation);

		return EBTNodeResult::Succeeded;
	}
		
	
	return EBTNodeResult::Failed;
}

bool UBTT_RememberItem::IsLocationReachable(APawn* Pawn, const FVector& Location) const
{
	if (!Pawn) return false;

	UWorld* World = Pawn->GetWorld();
	if (!World) return false;

	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(World);
	if (!NavSys) return false;

	UNavigationPath* Path = NavSys->FindPathToLocationSynchronously(World,Pawn->GetActorLocation(),Location,Pawn);

	return Path && Path->IsValid() && !Path->IsPartial();
}

bool UBTT_RememberItem::FindReachableLocationNearItem(APawn* Pawn, const FVector& ItemLocation, FVector& OutLocation) const
{
	if (!Pawn) return false;

	UWorld* World = Pawn->GetWorld();
	if (!World) return false;

	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(World);
	if (!NavSys) return false;

	FNavLocation ProjectedLocation;

	// First try the item location itself
	if (NavSys->ProjectPointToNavigation(ItemLocation,ProjectedLocation,FVector(100.f, 100.f, 300.f))
		&& IsLocationReachable(Pawn, ProjectedLocation.Location))
	{
		OutLocation = ProjectedLocation.Location;
		return true;
	}

	// Otherwise search around the item
	const float SearchRadius = 50.f;
	const int Samples = 16;

	float BestDistanceToItem = TNumericLimits<float>::Max();
	bool bFoundLocation = false;

	for (int i = 0; i < Samples; ++i)
	{
		const float Angle = (2.f * PI * i) / Samples;

		const FVector SampleLocation = ItemLocation + FVector(FMath::Cos(Angle), FMath::Sin(Angle), 0.f) * SearchRadius;

		FNavLocation SampleNavLocation;

		if (!NavSys->ProjectPointToNavigation(
			SampleLocation,
			SampleNavLocation,
			FVector(150.f, 150.f, 300.f)))
		{
			continue;
		}

		if (!IsLocationReachable(Pawn, SampleNavLocation.Location))
		{
			continue;
		}

		const float DistanceToItem =
			FVector::DistSquared(SampleNavLocation.Location, ItemLocation);

		if (DistanceToItem < BestDistanceToItem)
		{
			BestDistanceToItem = DistanceToItem;
			OutLocation = SampleNavLocation.Location;
			bFoundLocation = true;
		}
	}

	return bFoundLocation;
}
