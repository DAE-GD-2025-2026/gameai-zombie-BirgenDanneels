#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "BTT_ScanAround.generated.h"

UCLASS()
class DANNEELSBIRGENZOMBIERUNTIME_API UBTT_ScanAround: public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTT_ScanAround();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& root, uint8* nodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& root, uint8* nodeMemory, float deltaSeconds) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& root, uint8* nodeMemory) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RotationTime{0.5f};
	
private:
	
	float TotalYaw;
};
