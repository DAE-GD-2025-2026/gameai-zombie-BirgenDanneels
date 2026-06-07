#include "EQC_NearbyZombieLocationsDanneelsBirgen.h"

#include "AIController.h"
#include "DanneelsBirgenZombieRuntime/StudentPerceptorDanneelsBirgen.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Point.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "Zombies/BaseZombie.h"

void UEnvQueryContext_NearbyZombieLocationsDanneelsBirgen::ProvideContext(FEnvQueryInstance& QueryInstance,
                                                            FEnvQueryContextData& ContextData) const
{
	const AActor* QueryOwner = Cast<AActor>(QueryInstance.Owner.Get());
	if (!QueryOwner) return;
	
	const APawn* QueryPawn = Cast<APawn>(QueryOwner);
	if (!QueryPawn) return;
	
	const UStudentPerceptorDanneelsBirgen* Comp = QueryPawn->FindComponentByClass<UStudentPerceptorDanneelsBirgen>();
	if (!Comp) return;
	
	TArray<FVector> ZombieLocations;

	const TArray<ABaseZombie*>& ZombiesSeen = Comp->GetZombiesSeen();

	ZombieLocations.Reserve(ZombiesSeen.Num());

	for (const ABaseZombie* Zombie : ZombiesSeen)
	{
		if (IsValid(Zombie))
		{
			ZombieLocations.Add(Zombie->GetActorLocation());
		}
	}
	
	UEnvQueryItemType_Point::SetContextHelper(ContextData, ZombieLocations);
}
