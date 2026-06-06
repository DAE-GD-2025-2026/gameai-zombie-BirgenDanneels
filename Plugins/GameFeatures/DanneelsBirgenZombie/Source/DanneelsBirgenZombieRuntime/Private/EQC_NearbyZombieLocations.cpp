#include "EQC_NearbyZombieLocations.h"

#include "AIController.h"
#include "DanneelsBirgenZombieRuntime/StudentPerceptor.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Point.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "Zombies/BaseZombie.h"

void UEnvQueryContext_NearbyZombieLocations::ProvideContext(FEnvQueryInstance& QueryInstance,
                                                            FEnvQueryContextData& ContextData) const
{
	const AActor* QueryOwner = Cast<AActor>(QueryInstance.Owner.Get());
	if (!QueryOwner) return;
	
	const APawn* QueryPawn = Cast<APawn>(QueryOwner);
	if (!QueryPawn) return;
	
	const UStudentPerceptor* Comp = QueryPawn->FindComponentByClass<UStudentPerceptor>();
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
	
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, FString::Printf(TEXT("Found %d zombies to query"), ZombieLocations.Num()));
	
	UEnvQueryItemType_Point::SetContextHelper(ContextData, ZombieLocations);
}
