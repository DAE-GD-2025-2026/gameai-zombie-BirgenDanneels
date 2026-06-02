#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTest.h"
#include "EQT_DiscoveryScore.generated.h"

UCLASS()
class DANNEELSBIRGENZOMBIERUNTIME_API UEnvQueryTest_DiscoveryScore : public UEnvQueryTest
{
	GENERATED_BODY()

public:
	UEnvQueryTest_DiscoveryScore();

	virtual void RunTest(FEnvQueryInstance& QueryInstance) const override;
};
