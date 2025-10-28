"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const UE = require("ue");
const puerts_1 = require("puerts");
let ucls = UE.Class.Load("/Game/Game/GameMap/Blueprint/Enemy/BT_Task/BTT_FindPatrolLocationInSky.BTT_FindPatrolLocationInSky_C");
let jsCls = puerts_1.blueprint.tojs(ucls);
class BTT_FindPatrolLocationInSky {
    /** 水平搜索半径 */
    SearchRadius;
    /** 最小飞行高度偏移（相对于初始位置，负值表示可以下降） */
    MinHeightOffset;
    /** 最大飞行高度偏移（相对于初始位置，正值表示可以上升） */
    MaxHeightOffset;
    /** 最大尝试次数 */
    MaxAttempts;
    /** 障碍物检测半径（球体检测） */
    ObstacleCheckRadius;
    /** 保存巡逻位置的黑板键 */
    PatrolLocationKey;
    bShowDebug;
    ReceiveExecuteAI(OwnerController, ControlledPawn) {
        if (!OwnerController || !ControlledPawn) {
            console.error("BTT_FindPatrolLocationInSky: Invalid OwnerController or ControlledPawn");
            this.FinishExecute(false);
            return;
        }
        const BlackboardComp = OwnerController.Blackboard;
        if (!BlackboardComp) {
            console.error("BTT_FindPatrolLocationInSky: Blackboard component not found");
            this.FinishExecute(false);
            return;
        }
        const World = ControlledPawn.GetWorld();
        if (!World) {
            console.error("BTT_FindPatrolLocationInSky: World not found");
            this.FinishExecute(false);
            return;
        }
        if (!this.PatrolLocationKey || !this.PatrolLocationKey.SelectedKeyName) {
            console.error("BTT_FindPatrolLocationInSky: PatrolLocationKey is not set");
            this.FinishExecute(false);
            return;
        }
        // 获取当前位置（初始位置）
        const CurrentLocation = ControlledPawn.K2_GetActorLocation();
        const BaseHeight = CurrentLocation.Z; // 记录初始高度作为基准
        // 获取参数（使用默认值）
        const SearchRadius = this.SearchRadius > 0 ? this.SearchRadius : 1500.0;
        const MinHeightOffset = this.MinHeightOffset !== undefined ? this.MinHeightOffset : -100.0;
        const MaxHeightOffset = this.MaxHeightOffset !== undefined ? this.MaxHeightOffset : 300.0;
        const MaxAttempts = this.MaxAttempts > 0 ? this.MaxAttempts : 10;
        const ObstacleCheckRadius = this.ObstacleCheckRadius > 0 ? this.ObstacleCheckRadius : 100.0;
        const bShowDebug = this.bShowDebug !== undefined ? this.bShowDebug : false;
        if (bShowDebug) {
            console.log(`BTT_FindPatrolLocationInSky: 开始搜索 - 基准高度=${BaseHeight.toFixed(0)}, 半径=${SearchRadius.toFixed(0)}, 高度偏移=${MinHeightOffset.toFixed(0)}~${MaxHeightOffset.toFixed(0)}, 尝试=${MaxAttempts}`);
        }
        let bFoundLocation = false;
        let PatrolLocation;
        for (let i = 0; i < MaxAttempts; i++) {
            // 生成候选位置
            const CandidateLocation = this.GenerateRandomLocation(CurrentLocation, BaseHeight, SearchRadius, MinHeightOffset, MaxHeightOffset);
            // 验证位置是否可用（无障碍物）
            if (this.IsLocationValid(CandidateLocation, ObstacleCheckRadius, World, ControlledPawn)) {
                PatrolLocation = CandidateLocation;
                bFoundLocation = true;
                if (bShowDebug) {
                    const heightDiff = PatrolLocation.Z - BaseHeight;
                    console.log(`BTT_FindPatrolLocationInSky: 找到有效位置 (尝试=${i + 1}/${MaxAttempts}): ${this.VectorToString(PatrolLocation)}, 高度差=${heightDiff.toFixed(0)}`);
                }
                break;
            }
            else if (bShowDebug) {
                console.warn(`BTT_FindPatrolLocationInSky: 位置无效 (尝试=${i + 1}/${MaxAttempts}): ${this.VectorToString(CandidateLocation)}`);
            }
        }
        // 保存结果
        if (bFoundLocation && PatrolLocation) {
            BlackboardComp.SetValueAsVector(this.PatrolLocationKey.SelectedKeyName, PatrolLocation);
            // 调试可视化
            if (bShowDebug) {
                this.DrawDebugInfo(World, CurrentLocation, PatrolLocation, SearchRadius, BaseHeight, MinHeightOffset, MaxHeightOffset);
            }
            this.FinishExecute(true);
        }
        else {
            console.error(`BTT_FindPatrolLocationInSky: 所有 ${MaxAttempts} 次尝试均失败，无法找到有效巡逻点`);
            this.FinishExecute(false);
        }
    }
    /**
     * 生成随机位置（圆柱体区域内，高度相对于初始位置）
     */
    GenerateRandomLocation(BaseLocation, BaseHeight, Radius, MinHeightOffset, MaxHeightOffset) {
        // 生成随机水平偏移（圆形区域内）
        const RandomAngle = UE.KismetMathLibrary.RandomFloatInRange(0, 360);
        const RandomDistance = UE.KismetMathLibrary.RandomFloatInRange(0, Radius);
        const AngleRadians = UE.KismetMathLibrary.DegreesToRadians(RandomAngle);
        const OffsetX = Math.cos(AngleRadians) * RandomDistance;
        const OffsetY = Math.sin(AngleRadians) * RandomDistance;
        // 生成随机高度偏移（相对于基准高度）
        const HeightOffset = UE.KismetMathLibrary.RandomFloatInRange(MinHeightOffset, MaxHeightOffset);
        const TargetZ = BaseHeight + HeightOffset;
        return new UE.Vector(BaseLocation.X + OffsetX, BaseLocation.Y + OffsetY, TargetZ);
    }
    /**
     * 验证位置是否可用（使用球体检测）
     */
    IsLocationValid(Location, CheckRadius, World, ControlledPawn) {
        // 使用球体重叠检测
        const ObjectTypes = UE.NewArray(UE.EObjectTypeQuery); // 空数组，检测所有类型
        const ActorsToIgnore = UE.NewArray(UE.Actor);
        ActorsToIgnore.Add(ControlledPawn);
        const OutActors = (0, puerts_1.$ref)(UE.NewArray(UE.Actor));
        const bOverlap = UE.KismetSystemLibrary.SphereOverlapActors(World, Location, CheckRadius, ObjectTypes, UE.Actor.StaticClass(), // 检测所有 Actor
        ActorsToIgnore, OutActors);
        // 如果没有重叠，则位置可用
        if (!bOverlap) {
            return true;
        }
        const Actors = (0, puerts_1.$unref)(OutActors);
        return Actors.Num() === 0;
    }
    /**
     * 调试可视化
     */
    DrawDebugInfo(World, CurrentLocation, PatrolLocation, SearchRadius, BaseHeight, MinHeightOffset, MaxHeightOffset) {
        const DebugDuration = 10.0;
        // 计算高度范围（相对于基准高度）
        const MinHeightWorld = BaseHeight + MinHeightOffset;
        const MaxHeightWorld = BaseHeight + MaxHeightOffset;
        const MinHeightCenter = new UE.Vector(CurrentLocation.X, CurrentLocation.Y, MinHeightWorld);
        const MaxHeightCenter = new UE.Vector(CurrentLocation.X, CurrentLocation.Y, MaxHeightWorld);
        // 底部圆圈（红色）
        UE.KismetSystemLibrary.DrawDebugCircle(World, MinHeightCenter, SearchRadius, 64, new UE.LinearColor(1, 0, 0, 0.5), DebugDuration, 2.0, new UE.Vector(0, 1, 0), new UE.Vector(1, 0, 0), false);
        // 顶部圆圈（红色）
        UE.KismetSystemLibrary.DrawDebugCircle(World, MaxHeightCenter, SearchRadius, 64, new UE.LinearColor(1, 0, 0, 0.5), DebugDuration, 2.0, new UE.Vector(0, 1, 0), new UE.Vector(1, 0, 0), false);
        // 绘制基准高度平面圆圈（黄色，表示初始高度）
        const BaseHeightCenter = new UE.Vector(CurrentLocation.X, CurrentLocation.Y, BaseHeight);
        UE.KismetSystemLibrary.DrawDebugCircle(World, BaseHeightCenter, SearchRadius, 64, new UE.LinearColor(1, 1, 0, 0.3), DebugDuration, 1.5, new UE.Vector(0, 1, 0), new UE.Vector(1, 0, 0), false);
        // 绘制圆柱体边界线（4条竖线）
        for (let i = 0; i < 4; i++) {
            const Angle = (i * 90) * Math.PI / 180;
            const OffsetX = Math.cos(Angle) * SearchRadius;
            const OffsetY = Math.sin(Angle) * SearchRadius;
            const BottomPoint = new UE.Vector(CurrentLocation.X + OffsetX, CurrentLocation.Y + OffsetY, MinHeightWorld);
            const TopPoint = new UE.Vector(CurrentLocation.X + OffsetX, CurrentLocation.Y + OffsetY, MaxHeightWorld);
            UE.KismetSystemLibrary.DrawDebugLine(World, BottomPoint, TopPoint, new UE.LinearColor(1, 0, 0, 0.3), DebugDuration, 1.0);
        }
        // 绘制当前位置标记（蓝色球体）
        UE.KismetSystemLibrary.DrawDebugSphere(World, CurrentLocation, 50.0, 12, new UE.LinearColor(0, 0, 1, 1), DebugDuration, 3.0);
        // 绘制巡逻位置标记（绿色球体）
        UE.KismetSystemLibrary.DrawDebugSphere(World, PatrolLocation, 80.0, 12, new UE.LinearColor(0, 1, 0, 1), DebugDuration, 3.0);
        // 绘制连接线（黄色箭头）
        UE.KismetSystemLibrary.DrawDebugArrow(World, CurrentLocation, PatrolLocation, 100.0, new UE.LinearColor(1, 1, 0, 1), DebugDuration, 5.0);
    }
    VectorToString(Vec) {
        return `(X=${Vec.X.toFixed(0)}, Y=${Vec.Y.toFixed(0)}, Z=${Vec.Z.toFixed(0)})`;
    }
}
puerts_1.blueprint.mixin(jsCls, BTT_FindPatrolLocationInSky);
//# sourceMappingURL=BTT_FindPatrolLocationInSky.js.map