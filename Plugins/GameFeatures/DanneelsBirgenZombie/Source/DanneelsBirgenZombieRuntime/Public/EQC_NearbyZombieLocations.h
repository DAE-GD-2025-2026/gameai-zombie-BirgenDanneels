#pragma once
#include "EnvironmentQuery/EnvQueryContext.h"
#include "EQC_NearbyZombieLocations.generated.h"

UCLASS()
class DANNEELSBIRGENZOMBIERUNTIME_API UEnvQueryContext_NearbyZombieLocations : public UEnvQueryContext
{
	GENERATED_BODY()
	
public:
	virtual void ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const override;
};
