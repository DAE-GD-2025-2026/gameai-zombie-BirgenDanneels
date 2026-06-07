#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTest.h"
#include "EQT_DiscoveryScoreDanneelsBirgen.generated.h"

UCLASS()
class DANNEELSBIRGENZOMBIERUNTIME_API UEnvQueryTest_DiscoveryScoreDanneelsBirgen : public UEnvQueryTest
{
	GENERATED_BODY()

public:
	UEnvQueryTest_DiscoveryScoreDanneelsBirgen();

	virtual void RunTest(FEnvQueryInstance& QueryInstance) const override;
};
