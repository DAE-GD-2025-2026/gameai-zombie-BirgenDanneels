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

	const AAIController* AI = Cast<AAIController>(QueryOwner->GetInstigatorController());
	if (!AI) return;

	const UStudentPerceptor* Comp = AI->FindComponentByClass<UStudentPerceptor>();
	if (!Comp) return;

	UEnvQueryItemType_Point::SetContextHelper(ContextData, Comp->GetRecentlyVisited());
}
