#include "EQC_StrategicLootLocation.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Point.h"

void UEnvQueryContext_StrategicLootLocation::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	const AActor* QueryOwner = Cast<AActor>(QueryInstance.Owner.Get());
	if (!QueryOwner) return;
	
	const APawn* QueryPawn = Cast<APawn>(QueryOwner);
	if (!QueryPawn) return;
	
	AAIController* Contr = Cast<AAIController>(QueryPawn->GetController());
	if (!Contr) return;
	UBlackboardComponent* BlackBoard = Contr->GetBlackboardComponent();
	
	if (!BlackBoard) return;
	
	const FName KeyName = TEXT("StrategicLootLocation");

	if (!BlackBoard->IsVectorValueSet(KeyName))
	{
		return;
	}

	const FVector Location = BlackBoard->GetValueAsVector(KeyName);

	
	UEnvQueryItemType_Point::SetContextHelper(ContextData, Location);
}
