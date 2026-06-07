#pragma once
#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_NeedsDanneelsBirgen.generated.h"

class ASurvivorPawn;
enum class EItemType : uint8;

UCLASS()
class DANNEELSBIRGENZOMBIERUNTIME_API UBTS_NeedsDanneelsBirgen : public UBTService
{
	GENERATED_BODY()
public:
	UBTS_NeedsDanneelsBirgen();
	
protected:
	virtual void TickNode(UBehaviorTreeComponent& root, uint8* nodeMemory, float deltaSeconds) override;
};
