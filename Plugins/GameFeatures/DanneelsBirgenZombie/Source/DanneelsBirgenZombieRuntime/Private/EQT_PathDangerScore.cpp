#include "EQT_PathDangerScore.h"

#include "AIController.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Point.h"

#include "DanneelsBirgenZombieRuntime/StudentPerceptor.h"
#include "Zombies/BaseZombie.h"

UEnvQueryTest_PathDangerScore::UEnvQueryTest_PathDangerScore()
{
	Cost = EEnvTestCost::High;
	ValidItemType = UEnvQueryItemType_Point::StaticClass();

	SetWorkOnFloatValues(true);
}

void UEnvQueryTest_PathDangerScore::RunTest(FEnvQueryInstance& QueryInstance) const
{
	UObject* Owner = QueryInstance.Owner.Get();
	if (!Owner) return;

	APawn* QueryPawn = Cast<APawn>(Owner);

	if (!QueryPawn)
	{
		if (const AAIController* AIController = Cast<AAIController>(Owner))
		{
			QueryPawn = AIController->GetPawn();
		}
	}

	if (!QueryPawn) return;

	UWorld* World = QueryPawn->GetWorld();
	if (!World) return;

	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(World);
	if (!NavSys) return;

	const UStudentPerceptor* Perceptor = QueryPawn->FindComponentByClass<UStudentPerceptor>();
	if (!Perceptor) return;

	TArray<FVector> ZombieLocations;

	for (const TObjectPtr<ABaseZombie>& Zombie : Perceptor->GetZombiesSeen())
	{
		if (IsValid(Zombie))
		{
			ZombieLocations.Add(Zombie->GetActorLocation());
		}
	}

	if (ZombieLocations.Num() == 0)
	{
		for (FEnvQueryInstance::ItemIterator It(this, QueryInstance); It; ++It)
		{
			It.SetScore(TestPurpose, FilterType, 1.f, 0.f, 1.f);
		}

		return;
	}

	const FVector StartLocation = QueryPawn->GetActorLocation();

	for (FEnvQueryInstance::ItemIterator It(this, QueryInstance); It; ++It)
	{
		const FVector TargetLocation = GetItemLocation(QueryInstance, It.GetIndex());

		// Get path to candidate point
		UNavigationPath* Path = NavSys->FindPathToLocationSynchronously(
			World,
			StartLocation,
			TargetLocation,
			QueryPawn
		);

		if (!Path || !Path->IsValid() || Path->PathPoints.Num() < 2)
		{
			It.ForceItemState(EEnvItemStatus::Failed);
			continue;
		}

		float WorstDanger = 0.f;
		bool bCriticalPath = false;

		const TArray<FVector>& Points = Path->PathPoints;

		for (int32 i = 0; i < Points.Num() - 1; ++i)
		{
			const FVector A = Points[i];
			const FVector B = Points[i + 1];

			const float SegmentLength = FVector::Dist2D(A, B);
			const int Samples = FMath::Max(1, FMath::CeilToInt(SegmentLength / PathSampleStep));

			// Create and check sample points between path points for zombies
			for (int s = 0; s <= Samples; ++s)
			{
				const float Alpha = static_cast<float>(s) / static_cast<float>(Samples);
				const FVector SamplePoint = FMath::Lerp(A, B, Alpha);

				// Ignore points in close proximity to the start point so when the survivor is being hugged by a zombie it will still return a good path
				if (FVector::Dist2D(SamplePoint, StartLocation) < IgnoreStartRadius)
					continue;

				for (const FVector& ZombieLocation : ZombieLocations)
				{
					const float Distance = FVector::Dist2D(SamplePoint, ZombieLocation);

					if (Distance <= CriticalRadius)
					{
						bCriticalPath = true;
						WorstDanger = 1.f;
						break;
					}

					if (Distance <= DangerRadius)
					{
						const float Danger = 1.f - Distance / DangerRadius;
						WorstDanger = FMath::Max(WorstDanger, Danger);
					}
				}

				if (bCriticalPath)
					break;
			}

			if (bCriticalPath)
				break;
		}

		if (bCriticalPath)
		{
			It.SetScore(TestPurpose, FilterType, 0.f, 0.f, 1.f);
			continue;
		}

		const float SafetyScore = 1.f - WorstDanger;
		It.SetScore(TestPurpose, FilterType, SafetyScore,0.f,1.f);
	}
}
