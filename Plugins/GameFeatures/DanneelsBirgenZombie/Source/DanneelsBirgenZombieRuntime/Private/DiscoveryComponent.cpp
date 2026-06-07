#include "DiscoveryComponent.h"

#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"

UDiscoveryComponent::UDiscoveryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UDiscoveryComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UDiscoveryComponent::TickComponent(
	float DeltaTime,
	ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction
)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateFromOwner();
}

void UDiscoveryComponent::UpdateFromOwner()
{
	AActor* Owner = GetOwner();
	if (!Owner) return;

	MarkVisited(Owner->GetActorLocation());

	if (bDrawDebugGrid)
	{
		DebugDrawGrid(0.f);
	}
}

FIntPoint UDiscoveryComponent::WorldToGrid(FVector Location) const
{
	const int32 X = FMath::FloorToInt((Location.X - WorldOrigin.X) / CellSize);
	const int32 Y = FMath::FloorToInt((Location.Y - WorldOrigin.Y) / CellSize);

	return FIntPoint(X, Y);
}

FVector UDiscoveryComponent::GridToWorld(FIntPoint Cell) const
{
	return FVector(
		WorldOrigin.X + Cell.X * CellSize + CellSize * 0.5f,
		WorldOrigin.Y + Cell.Y * CellSize + CellSize * 0.5f,
		WorldOrigin.Z
	);
}

bool UDiscoveryComponent::IsValidCell(FIntPoint Cell) const
{
	return
		FMath::Abs(Cell.X) <= GridHalfSize &&
		FMath::Abs(Cell.Y) <= GridHalfSize;
}

void UDiscoveryComponent::MarkVisited(FVector WorldLocation)
{
	UWorld* World = GetWorld();
	if (!World) return;

	const FIntPoint Cell = WorldToGrid(WorldLocation);

	if (!IsValidCell(Cell))
	{
		return;
	}

	FDiscoveryCell& Data = DiscoveryGrid.FindOrAdd(Cell);

	Data.bVisited = true;
	Data.bEverSeen = true;
	Data.LastVisitTime = World->GetTimeSeconds();

	AddKnownNeighborCells(Cell);
}

void UDiscoveryComponent::AddKnownNeighborCells(FIntPoint Cell)
{
	static const FIntPoint Neighbors[4]
	{
		FIntPoint(1, 0),
		FIntPoint(-1, 0),
		FIntPoint(0, 1),
		FIntPoint(0, -1)
	};

	for (const FIntPoint& Offset : Neighbors)
	{
		const FIntPoint NeighborCell = Cell + Offset;

		if (!IsValidCell(NeighborCell))
		{
			continue;
		}

		FDiscoveryCell& NeighborData = DiscoveryGrid.FindOrAdd(NeighborCell);
		NeighborData.bEverSeen = true;
	}
}

bool UDiscoveryComponent::WasVisited(FIntPoint Cell) const
{
	const FDiscoveryCell* Data = DiscoveryGrid.Find(Cell);
	return Data && Data->bVisited;
}

bool UDiscoveryComponent::IsFrontierCell(FIntPoint Cell) const
{
	if (!IsValidCell(Cell))
	{
		return false;
	}

	const FDiscoveryCell* Data = DiscoveryGrid.Find(Cell);

	if (Data && Data->bVisited)
	{
		return false;
	}

	static const FIntPoint Neighbors[4]
	{
		FIntPoint(1, 0),
		FIntPoint(-1, 0),
		FIntPoint(0, 1),
		FIntPoint(0, -1)
	};

	for (const FIntPoint& Offset : Neighbors)
	{
		if (WasVisited(Cell + Offset))
		{
			return true;
		}
	}

	return false;
}

float UDiscoveryComponent::GetDiscoveryScoreAt(FVector WorldLocation) const
{
	const FIntPoint Cell = WorldToGrid(WorldLocation);

	if (!IsValidCell(Cell))
	{
		return 0.0f;
	}

	const FDiscoveryCell* Data = DiscoveryGrid.Find(Cell);

	if (Data && Data->bVisited)
	{
		return 0.0f;
	}

	static const FIntPoint Neighbors[4]
	{
		FIntPoint(1, 0),
		FIntPoint(-1, 0),
		FIntPoint(0, 1),
		FIntPoint(0, -1)
	};

	int32 VisitedNeighborCount = 0;

	for (const FIntPoint& Offset : Neighbors)
	{
		if (WasVisited(Cell + Offset))
		{
			VisitedNeighborCount++;
		}
	}

	if (VisitedNeighborCount > 0)
	{
		return VisitedNeighborCount / 4.0f;
	}

	return 0.05f;
}

void UDiscoveryComponent::DebugDrawGrid(float Duration) const
{
	UWorld* World = GetWorld();
	if (!World) return;

	const float HalfCell = CellSize * 0.5f;

	for (const auto& Pair : DiscoveryGrid)
	{
		const FIntPoint& Cell = Pair.Key;
		const FDiscoveryCell& Data = Pair.Value;

		if (!IsValidCell(Cell))
		{
			continue;
		}

		FVector Center = GridToWorld(Cell);
		Center.Z += DebugHeightOffset;

		const FVector Extent(HalfCell, HalfCell, 5.f);

		FColor Color = FColor::Blue;

		if (Data.bVisited)
		{
			Color = FColor::Green;
		}
		else if (IsFrontierCell(Cell))
		{
			Color = FColor::Yellow;
		}
		else if (Data.bEverSeen)
		{
			Color = FColor(80, 80, 80);
		}
		else
		{
			Color = FColor::Red;
		}

		// DrawDebugBox(
		// 	World,
		// 	Center,
		// 	Extent,
		// 	Color,
		// 	false,
		// 	Duration,
		// 	0,
		// 	2.f
		// );
		//
		// const float Score = GetDiscoveryScoreAt(GridToWorld(Cell));
		//
		// DrawDebugString(
		// 	World,
		// 	Center + FVector(0.f, 0.f, 30.f),
		// 	FString::Printf(TEXT("%.2f"), Score),
		// 	nullptr,
		// 	Color,
		// 	Duration
		// );
	}
}
