"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const UE = require("ue");
const puerts_1 = require("puerts");
let ucls = UE.Class.Load("/Game/Game/GameMap/Blueprint/Enemy/BT_Task/BTT_FlyToTarget.BTT_FlyToTarget_C");
let jsCls = puerts_1.blueprint.tojs(ucls);
class BTT_FlyToTarget {
    Target;
    MoveSpeed;
    /** 到达目标的接受半径（单位） */
    AcceptanceRadius;
    /** 任务超时时间（秒，0=无限制） */
    TaskTimeout;
    bShowDebug;
    StartTime;
    /** 基准高度（任务开始时的高度） */
    BaseHeight;
    ReceiveExecuteAI(OwnerController, ControlledPawn) {
        if (!OwnerController || !ControlledPawn) {
            console.error("BTT_FlyToTarget: Invalid OwnerController or ControlledPawn");
            this.FinishExecute(false);
            return;
        }
        const BlackboardComp = OwnerController.Blackboard;
        if (!BlackboardComp) {
            if (this.bShowDebug) {
                console.error("BTS_FlyToTarget: Blackboard component not found");
            }
            return;
        }
        const World = ControlledPawn.GetWorld();
        if (World) {
            BlackboardComp.SetValueAsFloat(this.StartTime.SelectedKeyName, 0);
        }
        // 记录初始高度作为基准高度
        const InitialLocation = ControlledPawn.K2_GetActorLocation();
        BlackboardComp.SetValueAsFloat(this.BaseHeight.SelectedKeyName, InitialLocation.Z);
    }
    GetTargetLocation(BlackboardComp) {
        if (!this.Target || !this.Target.SelectedKeyName) {
            return null;
        }
        const KeyName = this.Target.SelectedKeyName;
        const VectorLocation = BlackboardComp.GetValueAsVector(KeyName);
        if (VectorLocation && !VectorLocation.IsZero()) {
            return VectorLocation;
        }
        const TargetActor = BlackboardComp.GetValueAsObject(KeyName);
        if (TargetActor) {
            return TargetActor.K2_GetActorLocation();
        }
        return null;
    }
    ReceiveTickAI(OwnerController, ControlledPawn, DeltaSeconds) {
        if (!OwnerController || !ControlledPawn) {
            return;
        }
        const World = ControlledPawn.GetWorld();
        if (!World) {
            return;
        }
        const BlackboardComp = OwnerController.Blackboard;
        if (!BlackboardComp) {
            if (this.bShowDebug) {
                console.error("BTS_FlyToTarget: Blackboard component not found");
            }
            return;
        }
        if (!this.Target || !this.Target.SelectedKeyName) {
            if (this.bShowDebug) {
                console.error("BTT_FlyToTarget: TargetKey is not set");
            }
            return;
        }
        const TargetLocation = this.GetTargetLocation(BlackboardComp);
        if (!TargetLocation) {
            if (this.bShowDebug) {
                console.warn("BTT_FlyToTarget: Target location is invalid");
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
        // 获取移动速度（优先从黑板读取）
        let Speed = 600.0;
        if (this.MoveSpeed && this.MoveSpeed.SelectedKeyName) {
            const BlackboardSpeed = BlackboardComp.GetValueAsFloat(this.MoveSpeed.SelectedKeyName);
            if (BlackboardSpeed > 0) {
                Speed = BlackboardSpeed;
            }
        }
        // 计算本帧的移动距离
        const MoveVelocity = UE.KismetMathLibrary.Multiply_VectorFloat(NormalizedDirection, Speed * DeltaSeconds);
        // 获取当前位置
        const CurrentLocation = ControlledPawn.K2_GetActorLocation();
        // 【检测是否到达目标】
        const DistanceToTarget = UE.KismetMathLibrary.Vector_Distance(CurrentLocation, TargetLocation);
        const AcceptRadius = this.AcceptanceRadius > 0 ? this.AcceptanceRadius : 100.0;
        if (DistanceToTarget <= AcceptRadius) {
            if (this.bShowDebug) {
                console.log(`BTT_FlyToTarget: 到达目标！距离=${DistanceToTarget.toFixed(0)}, 接受半径=${AcceptRadius.toFixed(0)}`);
            }
            this.FinishExecute(true);
            return;
        }
        // 【检测超时】
        if (this.TaskTimeout > 0) {
            const CurrentTime = UE.KismetSystemLibrary.GetGameTimeInSeconds(World);
            const ElapsedTime = CurrentTime - BlackboardComp.GetValueAsFloat(this.StartTime.SelectedKeyName);
            if (ElapsedTime >= this.TaskTimeout) {
                console.warn(`BTT_FlyToTarget: 任务超时！耗时=${ElapsedTime.toFixed(1)}s, 超时限制=${this.TaskTimeout.toFixed(1)}s`);
                this.FinishExecute(false);
                return;
            }
        }
        // 计算新位置
        let NewLocation = UE.KismetMathLibrary.Add_VectorVector(CurrentLocation, MoveVelocity);
        // 【最低高度限制 - 不能低于初始位置】
        if (NewLocation.Z < BlackboardComp.GetValueAsFloat(this.BaseHeight.SelectedKeyName)) {
            NewLocation.Z = BlackboardComp.GetValueAsFloat(this.BaseHeight.SelectedKeyName);
        }
        ControlledPawn.K2_SetActorLocation(NewLocation, false, undefined, false);
        if (this.bShowDebug) {
            const DrawDuration = 0.0;
            const DrawThickness = 3.0;
            // 绘制到目标的连线
            UE.KismetSystemLibrary.DrawDebugLine(World, CurrentLocation, TargetLocation, new UE.LinearColor(1, 1, 0, 0.5), DrawDuration, 1.5);
            // 绘制移动方向箭头
            const ArrowEnd = UE.KismetMathLibrary.Add_VectorVector(CurrentLocation, UE.KismetMathLibrary.Multiply_VectorFloat(NormalizedDirection, 150.0));
            UE.KismetSystemLibrary.DrawDebugArrow(World, CurrentLocation, ArrowEnd, 50.0, new UE.LinearColor(0, 1, 0, 1), DrawDuration, DrawThickness);
            // 绘制当前位置球体
            UE.KismetSystemLibrary.DrawDebugSphere(World, CurrentLocation, 30.0, 12, new UE.LinearColor(0, 0, 1, 1), DrawDuration, 2.0);
            // 绘制目标位置球体
            UE.KismetSystemLibrary.DrawDebugSphere(World, TargetLocation, 40.0, 12, new UE.LinearColor(1, 0, 0, 1), DrawDuration, 2.0);
            // 绘制接受半径圆圈
            UE.KismetSystemLibrary.DrawDebugCircle(World, TargetLocation, AcceptRadius, 32, new UE.LinearColor(0, 1, 0, 0.3), DrawDuration, 2.0, new UE.Vector(0, 1, 0), new UE.Vector(1, 0, 0), false);
            // 每秒输出一次调试信息
            const CurrentTime = UE.KismetSystemLibrary.GetGameTimeInSeconds(World);
            if (Math.floor(CurrentTime) !== Math.floor(CurrentTime - DeltaSeconds)) {
                const heightDiff = NewLocation.Z - BlackboardComp.GetValueAsFloat(this.BaseHeight.SelectedKeyName);
                console.log(`BTT_FlyToTarget: 距离=${DistanceToTarget.toFixed(0)}, 速度=${Speed.toFixed(0)}, 当前高度=${NewLocation.Z.toFixed(0)}, 高度差=${heightDiff.toFixed(0)}`);
            }
        }
    }
}
puerts_1.blueprint.mixin(jsCls, BTT_FlyToTarget);
//# sourceMappingURL=BTT_FlyToTarget.js.map