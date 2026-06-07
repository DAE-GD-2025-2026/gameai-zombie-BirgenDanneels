#pragma once
#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_AutoConsumeDanneelsBirgen.generated.h"

class ASurvivorPawn;
enum class EItemType : uint8;

UCLASS()
class DANNEELSBIRGENZOMBIERUNTIME_API UBTS_AutoConsumeDanneelsBirgen: public UBTService
{
	GENERATED_BODY()
public:
	UBTS_AutoConsumeDanneelsBirgen();

protected:
	virtual void TickNode(UBehaviorTreeComponent& root, uint8* nodeMemory, float deltaSeconds) override;
	
private:
	bool TryConsumeItem(ASurvivorPawn* Pawn, EItemType Type, int NeededValue);
};
