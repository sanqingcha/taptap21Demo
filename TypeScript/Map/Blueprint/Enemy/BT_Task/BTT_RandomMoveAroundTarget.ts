import * as UE from 'ue';
import { $Nullable, $ref, $unref, blueprint } from 'puerts';

let ucls = UE.Class.Load("/Game/Game/GameMap/Blueprint/Enemy/BT_Task/BTT_RandomMoveAroundTarget.BTT_RandomMoveAroundTarget_C")
let jsCls = blueprint.tojs<typeof UE.Game.Game.GameMap.Blueprint.Enemy.BT_Task.BTT_RandomMoveAroundTarget.BTT_RandomMoveAroundTarget_C>(ucls);
interface BTT_RandomMoveAroundTarget extends UE.Game.Game.GameMap.Blueprint.Enemy.BT_Task.BTT_RandomMoveAroundTarget.BTT_RandomMoveAroundTarget_C { }

/**
 * 围绕目标随机移动任务
 * 在目标周围保持一定距离，计算随机位置并保存到黑板
 * 用于远程攻击敌人的"风筝"行为或保持攻击距离
 *
 */
class BTT_RandomMoveAroundTarget implements BTT_RandomMoveAroundTarget {
    // 蓝图变量
    TargetActor: UE.BlackboardKeySelector;         // 目标Actor（黑板键）
    Distance: number;                              // 期望保持的距离
    DistanceOffset: number;                        // 距离偏移量（随机范围）
    Radius: number;                                // 选点的角度范围（度）180=半圆，360=全圆
    KeepDistanceNextLocation: UE.BlackboardKeySelector;  // 保存计算结果的黑板键
    bShowDebug: boolean;                           // 是否显示调试可视化

    ReceiveExecuteAI(OwnerController: $Nullable<UE.AIController>, ControlledPawn: $Nullable<UE.Pawn>): void {
        if (!OwnerController || !ControlledPawn) {
            console.error("BTT_RandomMoveAroundTarget: OwnerController 或 ControlledPawn 为空");
            this.FinishExecute(false);
            return;
        }

        const BlackboardComp = OwnerController.Blackboard;
        if (!BlackboardComp) {
            console.error("BTT_RandomMoveAroundTarget: Blackboard 组件未找到");
            this.FinishExecute(false);
            return;
        }

        const World = ControlledPawn.GetWorld();
        if (!World) {
            console.error("BTT_RandomMoveAroundTarget: World 未找到");
            this.FinishExecute(false);
            return;
        }

        const NavSys = UE.NavigationSystemV1.GetNavigationSystem(World);
        if (!NavSys) {
            console.error("BTT_RandomMoveAroundTarget: Navigation System 未找到");
            this.FinishExecute(false);
            return;
        }

        if (!this.TargetActor) {
            console.error("BTT_RandomMoveAroundTarget: TargetActor 未设置");
            this.FinishExecute(false);
            return;
        }

        if (!this.KeepDistanceNextLocation) {
            console.error("BTT_RandomMoveAroundTarget: KeepDistanceNextLocation 未设置");
            this.FinishExecute(false);
            return;
        }

        const TargetActorObj = BlackboardComp.GetValueAsObject(this.TargetActor.SelectedKeyName) as UE.Actor;
        if (!TargetActorObj) {
            console.warn("BTT_RandomMoveAroundTarget: 目标Actor未设置或无效");
            this.FinishExecute(false);
            return;
        }

        const TargetLocation = TargetActorObj.K2_GetActorLocation();
        const PawnLocation = ControlledPawn.K2_GetActorLocation();
        const BaseDistance = this.Distance || 500.0;
        const Offset = this.DistanceOffset || 100.0;
        const AngleRange = this.Radius !== undefined ? this.Radius : 360.0;
        const bShowDebug = this.bShowDebug !== undefined ? this.bShowDebug : false;

        const RandomDistance = BaseDistance + UE.KismetMathLibrary.RandomFloatInRange(-Offset, Offset);

        // 计算从目标到敌人的方向向量（决定"敌人所在方向"）
        const ToEnemyVector = new UE.Vector(
            PawnLocation.X - TargetLocation.X,
            PawnLocation.Y - TargetLocation.Y,
            0
        );

        // 获取这个方向的角度（度数），作为基准角度
        // DegAtan2 返回 -180 到 180 度的角度
        const BaseAngleDeg = UE.KismetMathLibrary.DegAtan2(ToEnemyVector.Y, ToEnemyVector.X);

        // 在基准角度 ± (AngleRange/2) 范围内随机选择角度
        // 例如：AngleRange=180时，在基准角度左右各90度范围内选择（只在敌人这一侧）
        const HalfRange = AngleRange / 2;
        const RandomAngle = BaseAngleDeg + UE.KismetMathLibrary.RandomFloatInRange(-HalfRange, HalfRange);

        const AngleRadians = UE.KismetMathLibrary.DegreesToRadians(RandomAngle);

        // 计算圆周上的偏移量（2D）
        const OffsetX = Math.cos(AngleRadians) * RandomDistance;
        const OffsetY = Math.sin(AngleRadians) * RandomDistance;

        // 计算目标位置（围绕TargetActor）
        const RandomLocation = new UE.Vector(
            TargetLocation.X + OffsetX,
            TargetLocation.Y + OffsetY,
            TargetLocation.Z  // 保持相同高度
        );

        // 将位置投影到NavMesh
        const ProjectedLocation = $ref<UE.Vector>(undefined);
        const QueryExtent = new UE.Vector(500, 500, 500);

        const bProjected = UE.NavigationSystemV1.K2_ProjectPointToNavigation(
            World,
            RandomLocation,
            ProjectedLocation,
            undefined,
            undefined,
            QueryExtent
        );

        let FinalLocation: UE.Vector;

        if (bProjected && ProjectedLocation) {
            FinalLocation = $unref(ProjectedLocation);
            console.log(`BTT_RandomMoveAroundTarget: 成功计算位置 (距离=${RandomDistance.toFixed(0)}, 角度=${RandomAngle.toFixed(0)}°, 范围=±${HalfRange.toFixed(0)}°)`);
        } else {
            console.warn("BTT_RandomMoveAroundTarget: 无法投影到NavMesh，使用原始位置");
            FinalLocation = RandomLocation;
        }

        // 保存到黑板
        BlackboardComp.SetValueAsVector(this.KeepDistanceNextLocation.SelectedKeyName, FinalLocation);

        // 调试可视化
        if (bShowDebug) {
            const DebugDuration = 5.0;
            const DebugThickness = 3.0;

            // 绘制目标周围的圆圈（红色）
            UE.KismetSystemLibrary.DrawDebugCircle(
                World,
                TargetLocation,
                BaseDistance,
                32,
                new UE.LinearColor(1, 0, 0, 0.5),  // 红色半透明
                DebugDuration,
                2.0,
                new UE.Vector(0, 1, 0),
                new UE.Vector(1, 0, 0),
                false
            );

            // 绘制从玩家到敌人的基准方向线（白色粗线）
            const BaseDirectionEnd = new UE.Vector(
                TargetLocation.X + Math.cos(UE.KismetMathLibrary.DegreesToRadians(BaseAngleDeg)) * BaseDistance,
                TargetLocation.Y + Math.sin(UE.KismetMathLibrary.DegreesToRadians(BaseAngleDeg)) * BaseDistance,
                TargetLocation.Z
            );
            UE.KismetSystemLibrary.DrawDebugLine(
                World,
                TargetLocation,
                BaseDirectionEnd,
                new UE.LinearColor(1, 1, 1, 0.8),  // 白色
                DebugDuration,
                5.0
            );

            // 绘制允许选点的扇形边界（青色虚线）
            if (AngleRange < 360) {
                const LeftBoundaryAngle = BaseAngleDeg - HalfRange;
                const RightBoundaryAngle = BaseAngleDeg + HalfRange;

                const LeftBoundaryEnd = new UE.Vector(
                    TargetLocation.X + Math.cos(UE.KismetMathLibrary.DegreesToRadians(LeftBoundaryAngle)) * BaseDistance,
                    TargetLocation.Y + Math.sin(UE.KismetMathLibrary.DegreesToRadians(LeftBoundaryAngle)) * BaseDistance,
                    TargetLocation.Z
                );

                const RightBoundaryEnd = new UE.Vector(
                    TargetLocation.X + Math.cos(UE.KismetMathLibrary.DegreesToRadians(RightBoundaryAngle)) * BaseDistance,
                    TargetLocation.Y + Math.sin(UE.KismetMathLibrary.DegreesToRadians(RightBoundaryAngle)) * BaseDistance,
                    TargetLocation.Z
                );

                UE.KismetSystemLibrary.DrawDebugLine(
                    World,
                    TargetLocation,
                    LeftBoundaryEnd,
                    new UE.LinearColor(0, 1, 1, 0.6),  // 青色
                    DebugDuration,
                    2.0
                );

                UE.KismetSystemLibrary.DrawDebugLine(
                    World,
                    TargetLocation,
                    RightBoundaryEnd,
                    new UE.LinearColor(0, 1, 1, 0.6),  // 青色
                    DebugDuration,
                    2.0
                );
            }

            // 绘制从目标到计算位置的线（黄色）
            UE.KismetSystemLibrary.DrawDebugLine(
                World,
                TargetLocation,
                FinalLocation,
                new UE.LinearColor(1, 1, 0, 1),  // 黄色
                DebugDuration,
                DebugThickness
            );

            // 绘制目标位置点（绿色球体）
            UE.KismetSystemLibrary.DrawDebugSphere(
                World,
                FinalLocation,
                50.0,
                12,
                new UE.LinearColor(0, 1, 0, 1),  // 绿色
                DebugDuration,
                DebugThickness
            );

            // 绘制从AI到目标位置的线（蓝色半透明）
            UE.KismetSystemLibrary.DrawDebugLine(
                World,
                PawnLocation,
                FinalLocation,
                new UE.LinearColor(0, 0, 1, 0.5),  // 蓝色半透明
                DebugDuration,
                1.0
            );
        }

        this.FinishExecute(true);
    }
}

blueprint.mixin(jsCls, BTT_RandomMoveAroundTarget);

