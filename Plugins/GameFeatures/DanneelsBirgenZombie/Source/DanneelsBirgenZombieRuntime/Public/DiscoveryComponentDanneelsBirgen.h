#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DiscoveryComponentDanneelsBirgen.generated.h"

USTRUCT()
struct FDiscoveryCell
{
	GENERATED_BODY()

	bool bVisited = false;
	bool bEverSeen = false;
	float LastVisitTime = -FLT_MAX;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DANNEELSBIRGENZOMBIERUNTIME_API UDiscoveryComponentDanneelsBirgen : public UActorComponent
{
	GENERATED_BODY()

public:
	UDiscoveryComponentDanneelsBirgen();

	virtual void BeginPlay() override;
	virtual void TickComponent(
		float DeltaTime,
		ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction
	) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Discovery")
	float CellSize = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Discovery")
	FVector WorldOrigin = FVector::ZeroVector;

	// Grid goes from -GridHalfSize to +GridHalfSize in both X/Y.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Discovery")
	int GridHalfSize = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Discovery|Debug")
	bool bDrawDebugGrid = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Discovery|Debug")
	float DebugHeightOffset = 20.f;

	TMap<FIntPoint, FDiscoveryCell> DiscoveryGrid;

	UFUNCTION(BlueprintCallable)
	void MarkVisited(FVector WorldLocation);

	UFUNCTION(BlueprintCallable)
	bool WasVisited(FIntPoint Cell) const;

	UFUNCTION(BlueprintCallable)
	bool IsValidCell(FIntPoint Cell) const;

	UFUNCTION(BlueprintCallable)
	FIntPoint WorldToGrid(FVector Location) const;

	UFUNCTION(BlueprintCallable)
	FVector GridToWorld(FIntPoint Cell) const;

	UFUNCTION(BlueprintCallable)
	float GetDiscoveryScoreAt(FVector WorldLocation) const;

private:
	void UpdateFromOwner();

	void AddKnownNeighborCells(FIntPoint Cell);

	bool IsFrontierCell(FIntPoint Cell) const;

	void DebugDrawGrid(float Duration) const;
};
