#pragma once
#include "CoreMinimal.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/ActorComponent.h"
#include "NeedsComponentDanneelsBirgen.generated.h"

enum class EItemType : uint8;
class ASurvivorPawn;

UCLASS()
class UNeedsComponentDanneelsBirgen : public UActorComponent
{
	GENERATED_BODY()

public:
	void RecalculateNeeds(const ASurvivorPawn* Survivor, UBlackboardComponent* BlackBoard) const;
	
protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ShotgunNeedThreshold = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int PistolNeedThreshold = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MedkitNeedThreshold = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int FoodNeedThreshold = 2;
	
private:
	int GetWeaponNeed(const ASurvivorPawn* Survivor, EItemType WeaponType) const;
	int GetMedkitNeed(const ASurvivorPawn* Survivor) const;
	int GetFoodNeed(const ASurvivorPawn* Survivor) const;
};