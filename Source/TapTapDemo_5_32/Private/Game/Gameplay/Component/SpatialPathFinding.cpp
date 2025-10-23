#include "Game/Gameplay/Component/SpatialPathFinding.h"

USpatialPathFinding::USpatialPathFinding()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void USpatialPathFinding::BeginPlay()
{
	Super::BeginPlay();
	// Initialize any required resources here
}

FSpatialPathFindingResult USpatialPathFinding::GetNewResult(FVector from, FVector to)
{
    FSpatialPathFindingResult Result;
    FVector toBorderPoint = from + (to - from).GetSafeNormal() * TraceRadius;
    Result.Success = false;
    auto SetResult = [&](const FVector& targetLocation) {
        Result.Success = true;
        Result.TargetLocation = targetLocation;
        Result.Direction = (targetLocation - from).GetSafeNormal();
    };
    
    // 获取 World 上下文，确保在有效的游戏世界中运行
    UWorld* World = GetWorld();
    if (World == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("USpatialPathFinding::GetNewTargetLocation - GetWorld() returned null."));
        return Result;
    }

    // 定义一个 Lambda 函数用于执行射线检测，方便复用
    auto PerformLineTrace = [&](const FVector& Start, const FVector& End) -> bool
    {
        FHitResult HitResult;
        FCollisionQueryParams CollisionParams;
        
        // 忽略函数调用者所属的 Actor
        if (this->GetOwner())
        {
            CollisionParams.AddIgnoredActor(this->GetOwner());
        }
        for (auto actor : ExcludeActors)
            CollisionParams.AddIgnoredActor(actor);
            

        bool bWasHit = World->LineTraceSingleByChannel(
            HitResult,
            Start,
            End,
            ECC_Visibility, // 使用 Visibility 通道检测视觉上的障碍物
            CollisionParams
        );

        // 如果需要调试，可以取消下面的注释来绘制射线
        // DrawDebugLine(World, Start, End, bWasHit ? FColor::Red : FColor::Green, false, 5.0f, 0, 1.0f);

        if (bWasHit)
        {
            // UE_LOG(LogTemp, Warning, TEXT("Path from %s to %s is blocked by: %s"), *Start.ToString(), *End.ToString(), *HitResult.GetActor()->GetName());
        }

        return bWasHit;
    };

    // --- 核心逻辑开始 ---

    // 1. 首先尝试直接路径，如果通畅，直接返回
    if (!PerformLineTrace(from, toBorderPoint))
    {
        SetResult(to);
        return Result;
    }

    // 计算从起点到终点的方向向量
    const FVector MainDirection = (to - from).GetSafeNormal();

    // 构建一个垂直于 MainDirection 的坐标系，用于定义“上/下/左/右”
    // 使用世界Z轴作为“上”方向的参考
    const FVector WorldUp(0.f, 0.f, 1.f);
    // 通过叉乘计算出“右”方向。这确保“右”总是水平于地面（假设MainDirection不垂直于地面）
    FVector RightVector = FVector::CrossProduct(MainDirection, WorldUp).GetSafeNormal();

    // 边缘情况处理：如果 from 和 to 在同一条垂直线上，叉乘结果会是零向量
    if (RightVector.IsNearlyZero())
    {
        // 此时，我们可以使用世界Y轴作为“右”方向
        RightVector = FVector(0.f, 1.f, 0.f);
    }
    
    // 再次叉乘，得到精确的、与 MainDirection 和 RightVector 都垂直的“上”方向
    const FVector UpVector = FVector::CrossProduct(RightVector, MainDirection);

    // 3. 生成一系列偏移方向（相对于 MainDirection）
    // 这个数组是动态生成的，因为它依赖于 from 和 to 的相对位置
    const TArray<FVector> OffsetDirections = {
        // 45度对角线方向
        0.5 * (UpVector + MainDirection),
        0.5 * ( -UpVector+ MainDirection),
        0.5 * (RightVector + MainDirection),
        0.5 * (-RightVector + MainDirection),
        // 90度方向 (上、下、左、右)
        UpVector,
        -UpVector,
        RightVector,
        -RightVector,
    };

    // 4. 遍历所有偏移方向，寻找一个可行的目标点
    for (const FVector& Direction : OffsetDirections)
    {
        // 计算新的目标位置：在原始目标点'to'的基础上，沿着偏移方向移动
        FVector NewTargetLocation = TraceRadius * Direction + from;  

        // 检测从起点'from'到这个新目标点的路径是否通畅
        if (!PerformLineTrace(from, NewTargetLocation))
        {
            // 找到了一个无障碍的路径！
            SetResult(NewTargetLocation);
            UE_LOG(LogTemp, Log, TEXT("Found an alternative clear path to: %s"), *Result.TargetLocation.ToString());
            return Result; // 立即返回成功结果
        }
    }

    // 5. 如果遍历完所有备选方案都失败了，则返回失败结果
    UE_LOG(LogTemp, Warning, TEXT("Could not find any clear path from %s towards %s"), *from.ToString(), *to.ToString());
    Result.Success = false;
    Result.TargetLocation = to; // 即使失败，也返回原始目标点供参考
    Result.Direction = FVector::Zero();
    return Result;
}

bool USpatialPathFinding::NeedRecalculateNewTargetLocation(FVector from, FVector to)
{
    auto targetDis = FVector::Dist(to, LastTargetLocation);
    if (targetDis > 1) 
        return true;
    
    auto dis = FVector::Distance(from, LastStartLocation);
    if (dis < RecalculateRadius) 
        return false;

    return true;
}

FSpatialPathFindingResult USpatialPathFinding::GetResult(FVector to)
{
    auto from = GetOwner()->GetActorLocation();
    if (!NeedRecalculateNewTargetLocation(from , to))
        return LastResult;
    LastStartLocation = from;
    LastTargetLocation = to;
    LastResult = GetNewResult(from, to);
    return LastResult;
}

void USpatialPathFinding::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// Optional per-frame path updates can be implemented here
}


