#include "EQT_DiscoveryScore.h"

#include "EnvironmentQuery/Items/EnvQueryItemType_VectorBase.h"
#include "EnvironmentQuery/EnvQueryTypes.h"

#include "AIController.h"
#include "GameFramework/Actor.h"

#include "DiscoveryComponent.h"

UEnvQueryTest_DiscoveryScore::UEnvQueryTest_DiscoveryScore()
{
	Cost = EEnvTestCost::Low;

	ValidItemType = UEnvQueryItemType_VectorBase::StaticClass();

	SetWorkOnFloatValues(true);
}

void UEnvQueryTest_DiscoveryScore::RunTest(FEnvQueryInstance& QueryInstance) const
{
	UObject* QueryOwner = QueryInstance.Owner.Get();
	if (!QueryOwner) return;

	const AActor* OwnerActor = Cast<AActor>(QueryOwner);
	if (!OwnerActor) return;
	
	const UDiscoveryComponent* Discovery = OwnerActor->FindComponentByClass<UDiscoveryComponent>();
	if (!Discovery) return;
	
	for (FEnvQueryInstance::ItemIterator It(this, QueryInstance); It; ++It)
	{
		const FVector ItemLocation = GetItemLocation(QueryInstance, It.GetIndex());

		float Score = Discovery->GetDiscoveryScoreAt(ItemLocation);

		It.SetScore(TestPurpose, FilterType, Score, Score, Score);
	}
}
