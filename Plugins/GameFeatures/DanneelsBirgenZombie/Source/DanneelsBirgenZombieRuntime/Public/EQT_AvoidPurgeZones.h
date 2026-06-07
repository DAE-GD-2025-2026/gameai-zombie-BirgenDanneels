#pragma once

#include "EnvironmentQuery/EnvQueryTest.h"
#include "EQT_AvoidPurgeZones.generated.h"

class APurgeZone;

UCLASS()
class DANNEELSBIRGENZOMBIERUNTIME_API UEnvQueryTest_AvoidPurgeZones : public UEnvQueryTest
{
	GENERATED_BODY()

public:
	UEnvQueryTest_AvoidPurgeZones();

protected:
	virtual void RunTest(FEnvQueryInstance& QueryInstance) const override;

	UPROPERTY(EditDefaultsOnly, Category="Purge Zones", meta=(ClampMin="0"))
	float PurgeZoneRadius = 50.f;

	UPROPERTY(EditDefaultsOnly, Category="Purge Zones", meta=(ClampMin="1"))
	float PathSampleStep = 150.f;

private:
	bool IsPointInsidePurgeZone(const FVector& Point, const APurgeZone* Zone) const;

	bool PathTouchesForbiddenPurgeZone(const TArray<FVector>& PathPoints, const TArray<APurgeZone*>& PurgeZones, const APurgeZone* StartPurgeZone) const;
};