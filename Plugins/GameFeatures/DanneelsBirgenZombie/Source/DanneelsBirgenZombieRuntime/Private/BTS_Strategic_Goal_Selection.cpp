#include "BTS_Strategic_Goal_Selection.h"

#include "AIController.h"
#include "VectorTypes.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Items/BaseItem.h"
#include "Items/ItemType.h"

#include "Survivor/SurvivorPawn.h"
#include "DanneelsBirgenZombieRuntime/StudentPerceptor.h"

#include "DrawDebugHelpers.h"

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

    ASurvivorPawn* Survivor = Cast<ASurvivorPawn>(root.GetAIOwner()->GetPawn());
    if (!Survivor)
        return;

    auto* Perceptor = Survivor->GetComponentByClass<UStudentPerceptor>();
    if (!Perceptor)
        return;

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
    		UE::Geometry::Distance(CandidateLocation, Survivor->GetActorLocation()) > UE::Geometry::Distance(BestLocation, Survivor->GetActorLocation()))
    	{
    		BestScore = ClusterScore;
    		BestLocation = CandidateLocation;
    		BestCluster = CurrentCluster;
    	}
    }

    // Update BB

    if (BestScore > 0)
    {
    	DrawDebugSphere(
	GetWorld(),          // Current world context
	BestLocation,        // The coordinate vector you just saved
	50.0f,               // Radius of the sphere
	12,                  // Segments (smoothness of the sphere)
	FColor::Green,       // Color of the shape
	false,               // Persistent lines (false = temporary)
	2.0f,                // Lifetime in seconds (how long it stays visible)
	0,                   // Depth priority
	2.0f                 // Thickness of the lines
);
        BlackBoard->SetValueAsVector("StrategicLootLocation", BestLocation);
    	CurrentClusterCenter = BestLocation;
    	BestClusterCache = BestCluster;
    	CurrentState = GoalSelectionState::CommitState;
    }
    else
    {
    	BlackBoard->ClearValue("StrategicLootLocation");
    	CurrentClusterCenter = FVector::Zero();
    	BestClusterCache.Empty();
    }
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
	
	if (UE::Geometry::Distance(CurrentClusterCenter, Survivor->GetActorLocation()) < ClusterRadius)
	{
		BlackBoard->SetValueAsBool("CanLootDuringFlee", true);
	}
}
