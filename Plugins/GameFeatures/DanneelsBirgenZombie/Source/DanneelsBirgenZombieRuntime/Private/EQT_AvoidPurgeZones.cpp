#include "EQT_AvoidPurgeZones.h"

#include "AIController.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Point.h"

#include "DanneelsBirgenZombieRuntime/StudentPerceptor.h"
#include "PurgeZones/PurgeZone.h"

UEnvQueryTest_AvoidPurgeZones::UEnvQueryTest_AvoidPurgeZones()
{
	Cost = EEnvTestCost::High;
	ValidItemType = UEnvQueryItemType_Point::StaticClass();

	SetWorkOnFloatValues(false);
}

void UEnvQueryTest_AvoidPurgeZones::RunTest(FEnvQueryInstance& QueryInstance) const
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

	TArray<APurgeZone*> PurgeZones = Perceptor->GetPurgeZones();
	
	if (PurgeZones.Num() == 0)
	{
		for (FEnvQueryInstance::ItemIterator It(this, QueryInstance); It; ++It)
		{
			It.SetScore(TestPurpose, FilterType, true, true);
		}
		return;
	}

	const FVector StartLocation = QueryPawn->GetActorLocation();

	APurgeZone* StartPurgeZone = nullptr;
	
	// Get Start Zone
	for (APurgeZone* Zone : PurgeZones)
	{
		if (IsPointInsidePurgeZone(StartLocation, Zone))
		{
			StartPurgeZone = Zone;
			break;
		}
	}

	for (FEnvQueryInstance::ItemIterator It(this, QueryInstance); It; ++It)
	{
		const FVector CandidateLocation = GetItemLocation(QueryInstance, It.GetIndex());
		
		// Candidate may not be inside any purge zone
		for (const TObjectPtr<APurgeZone>& Zone : PurgeZones)
		{
			if (IsPointInsidePurgeZone(CandidateLocation, Zone))
			{
				It.ForceItemState(EEnvItemStatus::Failed);
				goto NextItem;
			}
		}
		
		{
			UNavigationPath* Path = NavSys->FindPathToLocationSynchronously(World, StartLocation,CandidateLocation, QueryPawn);

			if (!Path || !Path->IsValid() || Path->PathPoints.Num() < 2)
			{
				It.ForceItemState(EEnvItemStatus::Failed);
				goto NextItem;
			}

			if (PathTouchesForbiddenPurgeZone(Path->PathPoints, PurgeZones, StartPurgeZone))
			{
				It.ForceItemState(EEnvItemStatus::Failed);
				goto NextItem;
			}
		}

		It.SetScore(TestPurpose, FilterType, true, true);

	NextItem:
		continue;
	}
}

bool UEnvQueryTest_AvoidPurgeZones::IsPointInsidePurgeZone(const FVector& Point, const APurgeZone* Zone) const
{
	return FVector::DistSquared2D(Point, Zone->GetActorLocation()) <= FMath::Square(PurgeZoneRadius);
}

bool UEnvQueryTest_AvoidPurgeZones::PathTouchesForbiddenPurgeZone(const TArray<FVector>& PathPoints, const TArray<APurgeZone*>& PurgeZones, const APurgeZone* StartPurgeZone) const
{
	for (int i = 0; i < PathPoints.Num() - 1; ++i)
	{
		const FVector A = PathPoints[i];
		const FVector B = PathPoints[i + 1];

		const float SegmentLength = FVector::Dist2D(A, B);
		const int Samples = FMath::Max(1, FMath::CeilToInt(SegmentLength / PathSampleStep));

		for (int s = 0; s <= Samples; ++s)
		{
			const float Alpha = static_cast<float>(s) / static_cast<float>(Samples);
			const FVector SamplePoint = FMath::Lerp(A, B, Alpha);

			for (APurgeZone* Zone : PurgeZones)
			{
				// Ignore only the purge zone we started inside while checking the path.
				if (Zone == StartPurgeZone)
					continue;

				if (IsPointInsidePurgeZone(SamplePoint, Zone))
					return true;
			}
		}
	}

	return false;
}