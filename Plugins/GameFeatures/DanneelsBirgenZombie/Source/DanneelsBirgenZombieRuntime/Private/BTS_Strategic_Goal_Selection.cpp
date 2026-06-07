#include "BTS_Strategic_Goal_Selection.h"

#include "AIController.h"
#include "VectorTypes.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Items/BaseItem.h"
#include "Items/ItemType.h"

#include "Survivor/SurvivorPawn.h"
#include "DanneelsBirgenZombieRuntime/StudentPerceptor.h"

#include "DrawDebugHelpers.h"
#include "Village/House/House.h"

UBTS_Strategic_Goal_Selection::UBTS_Strategic_Goal_Selection()
{
	NodeName = "Strategic Goal Selection";

	Interval = 2.0f;
	RandomDeviation = 0.25f;
	
}

void UBTS_Strategic_Goal_Selection::TickNode(UBehaviorTreeComponent& root, uint8* nodeMemory, float deltaSeconds)
{
	switch (CurrentState)
	{
	case GoalSelectionState::ScanState:
		ScanTick(root);
		break;
	case GoalSelectionState::CommitState:
		CommitTick(root);
		break;
	default:
		break;
	}
}

void UBTS_Strategic_Goal_Selection::ScanTick(UBehaviorTreeComponent& root)
{
	UBlackboardComponent* BlackBoard = root.GetBlackboardComponent();
    if ( !BlackBoard)
    	return;
	
	BlackBoard->SetValueAsBool("CanLootDuringFlee", false);
	BlackBoard->SetValueAsBool("SearchHouseWhileFlee", false);

    ASurvivorPawn* Survivor = Cast<ASurvivorPawn>(root.GetAIOwner()->GetPawn());
    if (!Survivor)
        return;

    auto* Perceptor = Survivor->GetComponentByClass<UStudentPerceptor>();
    if (!Perceptor)
        return;

	// Loot Logic
    const TSet<TObjectPtr<ABaseItem>>& SeenLoot =
        Perceptor->GetSeenLoot();

    int BestScore = -1;
    FVector BestLocation = FVector::ZeroVector;
	TArray<ABaseItem*> BestCluster;

    // Evaluate every needed item as a cluster center

    for (ABaseItem* Candidate : SeenLoot)
    {
        if (!Candidate)
            continue;

    	TArray<ABaseItem*> CurrentCluster;
    	
        const FVector CandidateLocation =
            Candidate->GetActorLocation();
    	
        int FoodCount = 0;
        int MedkitCount = 0;
        int PistolCount = 0;
        int ShotgunCount = 0;

        for (ABaseItem* Item : SeenLoot)
        {
            if (!Item) continue;

            const float Dist = FVector::Dist(Item->GetActorLocation(), CandidateLocation);
            if (Dist > ClusterRadius) continue;

        	CurrentCluster.Add(Item);
        	
            switch (Item->GetItemType())
            {
            case EItemType::Food:     FoodCount++; break;
            case EItemType::Medkit:   MedkitCount++; break;
            case EItemType::Pistol:   PistolCount++; break;
            case EItemType::Shotgun:  ShotgunCount++; break;
            default: break;
            }
        }
    	
    	// Calculate Cluster Score
    	const int FoodNeed   = BlackBoard->GetValueAsInt("FoodStockNeed");
    	const int MedkitNeed = BlackBoard->GetValueAsInt("MedkitStockNeed");
    	const int PistolNeed = BlackBoard->GetValueAsInt("PistolNeed");
    	const int ShotgunNeed= BlackBoard->GetValueAsInt("ShotgunNeed");
    	
    	int ClusterScore =
				FMath::Min(FoodCount, FoodNeed) +
				FMath::Min(MedkitCount, MedkitNeed) +
				FMath::Min(PistolCount, PistolNeed) +
				FMath::Min(ShotgunCount, ShotgunNeed);

        if (ClusterScore > BestScore)
        {
            BestScore = ClusterScore;
            BestLocation = CandidateLocation;
        	BestCluster = CurrentCluster;
        }
    	else if (ClusterScore == BestScore && 
    		UE::Geometry::Distance(CandidateLocation, Survivor->GetActorLocation()) < UE::Geometry::Distance(BestLocation, Survivor->GetActorLocation()))
    	{
    		BestScore = ClusterScore;
    		BestLocation = CandidateLocation;
    		BestCluster = CurrentCluster;
    	}
    }

    // Update BB

    if (BestScore > 0)
    {
        BlackBoard->SetValueAsVector("StrategicLootLocation", BestLocation);
    	CurrentClusterCenter = BestLocation;
    	BestClusterCache = BestCluster;
    	CurrentState = GoalSelectionState::CommitState;
    	CurrentGoalType = GoalType::Cluster;
    	return;
    }
    else
    {
    	CurrentClusterCenter = FVector::Zero();
    	BestClusterCache.Empty();
    }
	
	// House Logic
	BlackBoard->SetValueAsBool("SearchHouseWhileFlee", false);
	
	if (AHouse* House = Cast<AHouse>(BlackBoard->GetValueAsObject("TargetHouse")))
	{
		LastHouseSearchTime = GetWorld()->GetTimeSeconds();
		BlackBoard->SetValueAsVector("StrategicLootLocation", House->GetBounds().Origin);
		CurrentState = GoalSelectionState::CommitState;
		CurrentGoalType = GoalType::House;
		return;
	}
	
    BlackBoard->ClearValue("StrategicLootLocation");
	
}

void UBTS_Strategic_Goal_Selection::CommitTick(UBehaviorTreeComponent& root)
{
	UBlackboardComponent* BlackBoard = root.GetBlackboardComponent();
	if ( !BlackBoard)
		return;
	
	ASurvivorPawn* Survivor = Cast<ASurvivorPawn>(root.GetAIOwner()->GetPawn());
	if (!Survivor)
		return;

	auto* Perceptor = Survivor->GetComponentByClass<UStudentPerceptor>();
	if (!Perceptor)
		return;

	
	switch (CurrentGoalType)
	{
		case GoalType::Cluster:
		{
			//Loot Logic
			const TSet<TObjectPtr<ABaseItem>>& SeenLoot =
				Perceptor->GetSeenLoot();
	
			for (ABaseItem* Item : BestClusterCache)
			{
				if (!SeenLoot.Contains(Item))
				{
					CurrentState = GoalSelectionState::ScanState;
					BlackBoard->SetValueAsBool("CanLootDuringFlee", false);
					return;
				}
			}
	
			if (UE::Geometry::Distance(CurrentClusterCenter, Survivor->GetActorLocation()) < LootRadius)
			{
				BlackBoard->SetValueAsBool("CanLootDuringFlee", true);
			}
			break;
		}
		
		case GoalType::House:
		{
			//House Logic
			float NewLastHouseSearchTime = BlackBoard->GetValueAsFloat("LastHouseSearchTime");
			if (LastHouseSearchTime < NewLastHouseSearchTime)
			{
				BlackBoard->SetValueAsBool("SearchHouseWhileFlee", false);
				CurrentState = GoalSelectionState::ScanState;
				return;
			}
			else
			{
				BlackBoard->SetValueAsBool("SearchHouseWhileFlee", true);
			}
		}
			
		break;
	}
}
