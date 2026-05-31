#include "EQC_VisitedLocations.h"

#include "AIController.h"
#include "DanneelsBirgenZombieRuntime/StudentPerceptor.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Point.h"
#include "EnvironmentQuery/EnvQueryTypes.h"

void UEnvQueryContext_VisitedLocations::ProvideContext(
	FEnvQueryInstance& QueryInstance,
	FEnvQueryContextData& ContextData) const
{
	const AActor* QueryOwner = Cast<AActor>(QueryInstance.Owner.Get());
	if (!QueryOwner) return;
	
	const APawn* QueryPawn = Cast<APawn>(QueryOwner);
	if (!QueryPawn) return;
	
	const UStudentPerceptor* Comp = QueryPawn->FindComponentByClass<UStudentPerceptor>();
	if (!Comp) return;

	UEnvQueryItemType_Point::SetContextHelper(ContextData, Comp->GetRecentlyVisited());
}
