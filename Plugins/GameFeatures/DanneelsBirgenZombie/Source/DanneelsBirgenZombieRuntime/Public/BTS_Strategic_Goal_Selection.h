#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTS_Strategic_Goal_Selection.generated.h"

class ABaseItem;

UCLASS()
class DANNEELSBIRGENZOMBIERUNTIME_API UBTS_Strategic_Goal_Selection : public UBTService
{
	GENERATED_BODY()

public:
	UBTS_Strategic_Goal_Selection();

protected:
	virtual void TickNode(UBehaviorTreeComponent& root, uint8* nodeMemory, float deltaSeconds) override;

private:

	enum class GoalSelectionState
	{
		ScanState,
		CommitState,
	};
	
	enum class GoalType
	{
		House,
		Cluster,
	};
	
	GoalType CurrentGoalType = GoalType::Cluster;
	GoalSelectionState CurrentState = GoalSelectionState::ScanState;
	FVector CurrentClusterCenter = FVector::ZeroVector;
	TArray<TObjectPtr<ABaseItem>> BestClusterCache;
	
	void ScanTick(UBehaviorTreeComponent& root);
	void CommitTick(UBehaviorTreeComponent& root);
	
	UPROPERTY(EditAnywhere, Category="Strategic Goal")
	float ClusterRadius = 1000.f;
	
	// House Cache
	float LastHouseSearchTime = 0.f;
};