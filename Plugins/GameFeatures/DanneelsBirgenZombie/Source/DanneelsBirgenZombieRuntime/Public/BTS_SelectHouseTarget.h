#pragma once
#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_SelectHouseTarget.generated.h"

class AHouse;

UCLASS()
class DANNEELSBIRGENZOMBIERUNTIME_API UBTS_SelectHouseTarget : public UBTService
{
	GENERATED_BODY()

public:
	UBTS_SelectHouseTarget();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};