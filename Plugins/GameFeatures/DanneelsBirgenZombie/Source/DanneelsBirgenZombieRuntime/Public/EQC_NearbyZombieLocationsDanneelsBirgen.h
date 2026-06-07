#pragma once
#include "EnvironmentQuery/EnvQueryContext.h"
#include "EQC_NearbyZombieLocationsDanneelsBirgen.generated.h"

UCLASS()
class DANNEELSBIRGENZOMBIERUNTIME_API UEnvQueryContext_NearbyZombieLocationsDanneelsBirgen : public UEnvQueryContext
{
	GENERATED_BODY()
	
public:
	virtual void ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const override;
};
