"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const UE = require("ue");
const puerts_1 = require("puerts");
let ucls = UE.Class.Load("/Game/Game/GameMap/Blueprint/Enemy/BT_Task/BTS_FlyToTarget.BTS_FlyToTarget_C");
let jsCls = puerts_1.blueprint.tojs(ucls);
/**
 * 飞行目标移动服务
 * 使用 SpatialPathFinding 组件持续计算飞行路径并移动敌人
 *
 * 工作流程：
 * 1. 每帧从黑板获取目标位置
 * 2. 调用 SpatialPathFinding.GetResult(目标位置) 获取移动方向
 * 3. 沿着计算出的方向移动 Pawn
 * 4. 可选：让 Pawn 旋转朝向移动方向
 */
class BTS_FlyToTarget {
    // ========== 蓝图变量 ==========
    /** 目标位置黑板键 */
    TargetLocationKey;
    /** 移动速度（单位/秒） */
    MoveSpeed;
    /** 旋转插值速度（越大旋转越快） */
    RotationInterpSpeed;
    /** 是否显示调试信息 */
    bShowDebug;
    // ========== 生命周期方法 ==========
    /**
     * 服务激活时调用
     */
    ReceiveActivationAI(OwnerController, ControlledPawn) {
        if (this.bShowDebug) {
            console.log("BTS_FlyToTarget: Service activated");
        }
    }
    /**
     * 每帧更新
     */
    ReceiveTickAI(OwnerController, ControlledPawn, DeltaSeconds) {
        if (!OwnerController || !ControlledPawn) {
            return;
        }
        const World = ControlledPawn.GetWorld();
        if (!World) {
            return;
        }
        // 获取黑板组件
        const BlackboardComp = OwnerController.Blackboard;
        if (!BlackboardComp) {
            if (this.bShowDebug) {
                console.error("BTS_FlyToTarget: Blackboard component not found");
            }
            return;
        }
        // 检查黑板键是否设置
        if (!this.TargetLocationKey || !this.TargetLocationKey.SelectedKeyName) {
            if (this.bShowDebug) {
                console.error("BTS_FlyToTarget: TargetLocationKey is not set");
            }
            return;
        }
        // 从黑板获取目标位置
        const TargetLocation = BlackboardComp.GetValueAsVector(this.TargetLocationKey.SelectedKeyName);
        // 检查目标位置是否有效
        if (!TargetLocation || (TargetLocation.X === 0 && TargetLocation.Y === 0 && TargetLocation.Z === 0)) {
            if (this.bShowDebug) {
                console.warn("BTS_FlyToTarget: Target location is invalid or zero");
            }
            return;
        }
        // 获取 SpatialPathFinding 组件
        const PathFindingComp = ControlledPawn.GetComponentByClass(UE.SpatialPathFinding.StaticClass());
        if (!PathFindingComp) {
            if (this.bShowDebug) {
                console.error("BTS_FlyToTarget: SpatialPathFinding component not found on pawn");
            }
            return;
        }
        // 调用 SpatialPathFinding.GetResult() 获取路径结果
        const PathResult = PathFindingComp.GetResult(TargetLocation);
        // 检查寻路是否成功
        if (!PathResult || !PathResult.Success) {
            if (this.bShowDebug) {
                console.warn("BTS_FlyToTarget: PathFinding failed");
            }
            return;
        }
        // 获取移动方向
        const Direction = PathResult.Direction;
        // 检查方向是否有效
        if (!Direction || Direction.SizeSquared() < 0.001) {
            if (this.bShowDebug) {
                console.warn("BTS_FlyToTarget: Direction is invalid or too small");
            }
            return;
        }
        // 归一化方向
        const NormalizedDirection = UE.KismetMathLibrary.Normal(Direction, 0.0001);
        // 计算移动速度（使用蓝图设置的速度，默认 600）
        const Speed = this.MoveSpeed > 0 ? this.MoveSpeed : 600.0;
        // 计算本帧的移动距离
        const MoveVelocity = UE.KismetMathLibrary.Multiply_VectorFloat(NormalizedDirection, Speed * DeltaSeconds);
        // 获取当前位置
        const CurrentLocation = ControlledPawn.K2_GetActorLocation();
        // 计算新位置
        const NewLocation = UE.KismetMathLibrary.Add_VectorVector(CurrentLocation, MoveVelocity);
        // 移动 Pawn（不进行碰撞检测，适合飞行单位）
        ControlledPawn.K2_SetActorLocation(NewLocation, false, undefined, false);
        // 让 Pawn 朝向移动方向（平滑旋转）
        const CurrentRotation = ControlledPawn.K2_GetActorRotation();
        const TargetRotation = UE.KismetMathLibrary.MakeRotFromX(NormalizedDirection);
        // 使用插值平滑旋转（默认插值速度 5.0）
        const InterpSpeed = this.RotationInterpSpeed > 0 ? this.RotationInterpSpeed : 5.0;
        const NewRotation = UE.KismetMathLibrary.RInterpTo(CurrentRotation, TargetRotation, DeltaSeconds, InterpSpeed);
        ControlledPawn.K2_SetActorRotation(NewRotation, false);
        // 调试可视化
        if (this.bShowDebug) {
            const DrawDuration = 0.0; // 仅绘制一帧
            const DrawThickness = 3.0;
            // 计算到目标的距离
            const DistanceToTarget = UE.KismetMathLibrary.Vector_Distance(CurrentLocation, TargetLocation);
            // 绘制当前位置到目标位置的线（黄色虚线）
            UE.KismetSystemLibrary.DrawDebugLine(World, CurrentLocation, TargetLocation, new UE.LinearColor(1, 1, 0, 0.5), // 黄色半透明
            DrawDuration, 1.5);
            // 绘制移动方向箭头（绿色）
            const ArrowEnd = UE.KismetMathLibrary.Add_VectorVector(CurrentLocation, UE.KismetMathLibrary.Multiply_VectorFloat(NormalizedDirection, 150.0));
            UE.KismetSystemLibrary.DrawDebugArrow(World, CurrentLocation, ArrowEnd, 50.0, new UE.LinearColor(0, 1, 0, 1), // 绿色
            DrawDuration, DrawThickness);
            // 绘制当前位置标记（蓝色球体）
            UE.KismetSystemLibrary.DrawDebugSphere(World, CurrentLocation, 30.0, 12, new UE.LinearColor(0, 0, 1, 1), // 蓝色
            DrawDuration, 2.0);
            // 绘制目标位置标记（红色球体）
            UE.KismetSystemLibrary.DrawDebugSphere(World, TargetLocation, 40.0, 12, new UE.LinearColor(1, 0, 0, 1), // 红色
            DrawDuration, 2.0);
            // 每秒输出一次日志（避免刷屏）
            const CurrentTime = UE.KismetSystemLibrary.GetGameTimeInSeconds(World);
            if (Math.floor(CurrentTime) !== Math.floor(CurrentTime - DeltaSeconds)) {
                console.log(`BTS_FlyToTarget: 飞向目标 - 距离=${DistanceToTarget.toFixed(0)}, 速度=${Speed.toFixed(0)}`);
            }
        }
    }
    /**
     * 服务停用时调用
     */
    ReceiveDeactivationAI(OwnerController, ControlledPawn) {
        if (this.bShowDebug) {
            console.log("BTS_FlyToTarget: Service deactivated");
        }
    }
}
puerts_1.blueprint.mixin(jsCls, BTS_FlyToTarget);
//# sourceMappingURL=BTS_FlyToTarget.js.map