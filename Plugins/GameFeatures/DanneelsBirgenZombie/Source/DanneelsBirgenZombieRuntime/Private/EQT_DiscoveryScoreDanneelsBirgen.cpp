#include "EQT_DiscoveryScoreDanneelsBirgen.h"

#include "EnvironmentQuery/Items/EnvQueryItemType_VectorBase.h"
#include "EnvironmentQuery/EnvQueryTypes.h"

#include "AIController.h"
#include "GameFramework/Actor.h"

#include "DiscoveryComponentDanneelsBirgen.h"

UEnvQueryTest_DiscoveryScoreDanneelsBirgen::UEnvQueryTest_DiscoveryScoreDanneelsBirgen()
{
	Cost = EEnvTestCost::Low;

	ValidItemType = UEnvQueryItemType_VectorBase::StaticClass();

	SetWorkOnFloatValues(true);
}

void UEnvQueryTest_DiscoveryScoreDanneelsBirgen::RunTest(FEnvQueryInstance& QueryInstance) const
{
	UObject* QueryOwner = QueryInstance.Owner.Get();
	if (!QueryOwner) return;

	const AActor* OwnerActor = Cast<AActor>(QueryOwner);
	if (!OwnerActor) return;
	
	const UDiscoveryComponentDanneelsBirgen* Discovery = OwnerActor->FindComponentByClass<UDiscoveryComponentDanneelsBirgen>();
	if (!Discovery) return;
	
	for (FEnvQueryInstance::ItemIterator It(this, QueryInstance); It; ++It)
	{
		const FVector ItemLocation = GetItemLocation(QueryInstance, It.GetIndex());

		float Score = Discovery->GetDiscoveryScoreAt(ItemLocation);

		It.SetScore(TestPurpose, FilterType, Score, Score, Score);
	}
}
