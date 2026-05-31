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
	virtual void TickNode(UBehaviorTreeComponent& root, uint8* nodeMemory, float deltaSeconds) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ShotgunNeedThreshold = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int PistolNeedThreshold = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MedkitNeedThreshold = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MinimumHealthThreshold = 3;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MinimumStaminaThreshold = 5;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int FoodNeedThreshold = 2;
	
private:
	
	int GetWeaponNeed(ASurvivorPawn* Survivor, EItemType WeaponType);
	int GetMedkitNeed(ASurvivorPawn* Survivor);
	int GetFoodNeed(ASurvivorPawn* Survivor);
};
