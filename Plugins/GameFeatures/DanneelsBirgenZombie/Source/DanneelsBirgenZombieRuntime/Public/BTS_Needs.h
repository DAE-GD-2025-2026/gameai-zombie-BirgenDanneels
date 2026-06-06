#pragma once
#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_Needs.generated.h"

class ASurvivorPawn;
enum class EItemType : uint8;

UCLASS()
class DANNEELSBIRGENZOMBIERUNTIME_API UBTS_Needs : public UBTService
{
	GENERATED_BODY()
public:
	UBTS_Needs();
	
protected:
	virtual void TickNode(UBehaviorTreeComponent& root, uint8* nodeMemory, float deltaSeconds) override;
};
