#pragma once
#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_SelectHouseTargetDanneelsBirgen.generated.h"

class AHouse;

UCLASS()
class DANNEELSBIRGENZOMBIERUNTIME_API UBTS_SelectHouseTargetDanneelsBirgen : public UBTService
{
	GENERATED_BODY()

public:
	UBTS_SelectHouseTargetDanneelsBirgen();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};