#pragma once
#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_AutoConsume.generated.h"

class ASurvivorPawn;
enum class EItemType : uint8;

UCLASS()
class DANNEELSBIRGENZOMBIERUNTIME_API UBTS_AutoConsume: public UBTService
{
	GENERATED_BODY()
public:
	UBTS_AutoConsume();

protected:
	virtual void TickNode(UBehaviorTreeComponent& root, uint8* nodeMemory, float deltaSeconds) override;
	
private:
	bool TryConsumeItem(ASurvivorPawn* Pawn, EItemType Type, int NeededValue);
};
