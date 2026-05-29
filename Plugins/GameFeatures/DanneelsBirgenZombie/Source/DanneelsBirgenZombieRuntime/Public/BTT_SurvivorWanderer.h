// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_SurvivorWanderer.generated.h"

/**
 * 
 */
UCLASS()
class DANNEELSBIRGENZOMBIERUNTIME_API UBTT_SurvivorWanderer : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTT_SurvivorWanderer();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& root, uint8* nodeMemory) override;
	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WanderRadius{100.0f};
	
};
