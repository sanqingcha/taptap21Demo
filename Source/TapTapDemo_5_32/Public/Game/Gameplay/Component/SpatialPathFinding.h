#pragma once

#include "CoreMinimal.h"
#include "SpatialPathFindingResult.h"
#include "Components/ActorComponent.h"
#include "Kismet/GameplayStaticsTypes.h"
#include "SpatialPathFinding.generated.h"

/**
 * SpatialPathFinding Component
 * Handles pathfinding functionality for actors in 3D space.
 * Override FindPath() to implement custom pathfinding algorithms.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TAPTAPDEMO_5_32_API USpatialPathFinding : public UActorComponent
{
    GENERATED_BODY()

public:    
    USpatialPathFinding();

protected:
    // Called when the game starts
    virtual void BeginPlay() override;
    FSpatialPathFindingResult GetNewResult(FVector from, FVector to);
    bool NeedRecalculateNewTargetLocation(FVector from, FVector to);
public:

    UPROPERTY(EditAnywhere)
    float RecalculateRadius = 50.f;
    UPROPERTY(EditAnywhere)
    float TraceRadius = 100.f;
    UPROPERTY(EditAnywhere)
    TSet<AActor*> ExcludeActors = {}; 
    
    UPROPERTY(EditAnywhere)
    FVector LastStartLocation = {-233,-233,-233};
    UPROPERTY(EditAnywhere)
    FVector LastTargetLocation = {-233,-233,-233};
    UPROPERTY(EditAnywhere)
    FSpatialPathFindingResult LastResult;
    
    // Called every frame
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    /**
     * Initiates pathfinding request
     * @param to - World space target position
     * @return result
     */
    UFUNCTION(BlueprintCallable, Category = "Pathfinding")
    FSpatialPathFindingResult GetResult(FVector to);
};