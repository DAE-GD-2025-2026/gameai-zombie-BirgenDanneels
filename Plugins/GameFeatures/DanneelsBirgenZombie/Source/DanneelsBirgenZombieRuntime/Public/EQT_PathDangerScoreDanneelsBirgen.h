#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTest.h"
#include "EQT_PathDangerScoreDanneelsBirgen.generated.h"

UCLASS()
class DANNEELSBIRGENZOMBIERUNTIME_API UEnvQueryTest_PathDangerScoreDanneelsBirgen : public UEnvQueryTest
{
	GENERATED_BODY()

public:
	UEnvQueryTest_PathDangerScoreDanneelsBirgen();

protected:
	virtual void RunTest(FEnvQueryInstance& QueryInstance) const override;

	UPROPERTY(EditDefaultsOnly)
	float DangerRadius = 700.f;

	UPROPERTY(EditDefaultsOnly)
	float CriticalRadius = 250.f;

	UPROPERTY(EditDefaultsOnly)
	float IgnoreStartRadius = 150.f;

	UPROPERTY(EditDefaultsOnly)
	float PathSampleStep = 150.f; // Distance between sample points along the path
};
