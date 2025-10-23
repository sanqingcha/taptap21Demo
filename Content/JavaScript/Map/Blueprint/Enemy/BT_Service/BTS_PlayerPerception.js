"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const UE = require("ue");
const puerts_1 = require("puerts");
let ucls = UE.Class.Load("/Game/Game/GameMap/Blueprint/Enemy/BT_Service/BTS_PlayerPerception.BTS_PlayerPerception_C");
let jsCls = puerts_1.blueprint.tojs(ucls);
/**
 *
 *
 * @class 检测玩家
 * @implements {BTS_PlayerPerception}
 */
class BTS_PlayerPerception {
    DetectionRadius; // 检测半径
    SightAngle; // 检测角度
    EyeHeight; // 眼睛高度
    bShowDebug;
    ReceiveTickAI(OwnerController, ControlledPawn, DeltaSeconds) {
        if (!OwnerController || !ControlledPawn) {
            return;
        }
        const BlackboardComp = OwnerController.Blackboard;
        if (!BlackboardComp) {
            console.error("BTS_PlayerPerception: Blackboard component not found");
            return;
        }
        let DetectionRange = 1500.0;
        if (this.DetectionRadius) {
            const RadiusFromBlackboard = BlackboardComp.GetValueAsFloat(this.DetectionRadius.SelectedKeyName);
            if (RadiusFromBlackboard > 0) {
                DetectionRange = RadiusFromBlackboard;
            }
        }
        const SightAngle = this.SightAngle || 90.0;
        const EyeHeight = this.EyeHeight || 80.0;
        const bShowDebug = this.bShowDebug !== undefined ? this.bShowDebug : false;
        const World = ControlledPawn.GetWorld();
        if (!World) {
            return;
        }
        const PlayerCharacter = UE.GameplayStatics.GetPlayerCharacter(World, 0);
        if (!PlayerCharacter) {
            BlackboardComp.ClearValue("TargetActor");
            return;
        }
        const AIPawnLocation = ControlledPawn.K2_GetActorLocation();
        const PlayerLocation = PlayerCharacter.K2_GetActorLocation();
        if (bShowDebug) {
            const DebugDrawDuration = 0.1;
            const CircleCenter = AIPawnLocation;
            const CircleRadius = DetectionRange;
            const CircleThickness = 2.0;
            const CircleColor = new UE.LinearColor(0, 1, 0, 0.5);
            UE.KismetSystemLibrary.DrawDebugCircle(World, CircleCenter, CircleRadius, 32, CircleColor, DebugDrawDuration, CircleThickness, new UE.Vector(0, 1, 0), new UE.Vector(1, 0, 0), false);
            // 如果不是360度全方位检测，绘制视野锥形
            if (SightAngle < 360.0) {
                const AIForwardVector = ControlledPawn.GetActorForwardVector();
                const HalfSightAngle = SightAngle / 2.0;
                const LeftBoundaryAngle = HalfSightAngle;
                const RightBoundaryAngle = -HalfSightAngle;
                const AIRotation = ControlledPawn.K2_GetActorRotation();
                const LeftRotation = UE.KismetMathLibrary.ComposeRotators(AIRotation, new UE.Rotator(0, LeftBoundaryAngle, 0));
                const LeftDirection = UE.KismetMathLibrary.GetForwardVector(LeftRotation);
                const LeftEnd = AIPawnLocation.op_Addition(UE.KismetMathLibrary.Multiply_VectorFloat(LeftDirection, DetectionRange));
                // 右边界
                const RightRotation = UE.KismetMathLibrary.ComposeRotators(AIRotation, new UE.Rotator(0, RightBoundaryAngle, 0));
                const RightDirection = UE.KismetMathLibrary.GetForwardVector(RightRotation);
                const RightEnd = AIPawnLocation.op_Addition(UE.KismetMathLibrary.Multiply_VectorFloat(RightDirection, DetectionRange));
                const LineColor = new UE.LinearColor(1, 1, 0, 1); // 黄色
                const LineThickness = 3.0;
                // 绘制左边界线
                UE.KismetSystemLibrary.DrawDebugLine(World, AIPawnLocation, LeftEnd, LineColor, DebugDrawDuration, LineThickness);
                // 绘制右边界线
                UE.KismetSystemLibrary.DrawDebugLine(World, AIPawnLocation, RightEnd, LineColor, DebugDrawDuration, LineThickness);
                // 绘制前向中心线
                const AIForwardVector2 = ControlledPawn.GetActorForwardVector();
                const CenterEnd = AIPawnLocation.op_Addition(UE.KismetMathLibrary.Multiply_VectorFloat(AIForwardVector2, DetectionRange));
                UE.KismetSystemLibrary.DrawDebugLine(World, AIPawnLocation, CenterEnd, new UE.LinearColor(1, 0, 0, 1), // 红色
                DebugDrawDuration, LineThickness);
                // 绘制视野扇形的弧线（用多条短线模拟）
                const ArcSegments = 16;
                for (let i = 0; i < ArcSegments; i++) {
                    const CurrentAngle = -HalfSightAngle + (SightAngle * i / ArcSegments);
                    const NextAngle = -HalfSightAngle + (SightAngle * (i + 1) / ArcSegments);
                    const CurrentRotation = UE.KismetMathLibrary.ComposeRotators(AIRotation, new UE.Rotator(0, CurrentAngle, 0));
                    const CurrentDirection = UE.KismetMathLibrary.GetForwardVector(CurrentRotation);
                    const CurrentPoint = AIPawnLocation.op_Addition(UE.KismetMathLibrary.Multiply_VectorFloat(CurrentDirection, DetectionRange));
                    const NextRotation = UE.KismetMathLibrary.ComposeRotators(AIRotation, new UE.Rotator(0, NextAngle, 0));
                    const NextDirection = UE.KismetMathLibrary.GetForwardVector(NextRotation);
                    const NextPoint = AIPawnLocation.op_Addition(UE.KismetMathLibrary.Multiply_VectorFloat(NextDirection, DetectionRange));
                    UE.KismetSystemLibrary.DrawDebugLine(World, CurrentPoint, NextPoint, LineColor, DebugDrawDuration, 2.0);
                }
            }
            else {
                // 360度全方位检测，绘制额外的圆形指示（红色，表示全方位感知）
                UE.KismetSystemLibrary.DrawDebugCircle(World, CircleCenter, CircleRadius * 0.9, // 稍小一点的内圈
                32, new UE.LinearColor(1, 0, 0, 0.3), // 红色半透明
                DebugDrawDuration, 3.0, new UE.Vector(0, 1, 0), new UE.Vector(1, 0, 0), false);
            }
        }
        // 计算距离
        const Distance = UE.KismetMathLibrary.Vector_Distance(AIPawnLocation, PlayerLocation);
        // 如果距离超过检测范围，清空目标并返回
        if (Distance > DetectionRange) {
            BlackboardComp.ClearValue("TargetActor");
            return;
        }
        // === 视野角度检测 ===
        // 如果SightAngle >= 360，表示360度全方位检测，跳过角度检查
        if (SightAngle < 360.0) {
            // 获取AI的前向向量
            const AIForwardVector = ControlledPawn.GetActorForwardVector();
            // 计算从AI指向玩家的方向向量
            const ToPlayerVector = UE.KismetMathLibrary.Subtract_VectorVector(PlayerLocation, AIPawnLocation);
            const ToPlayerDirection = UE.KismetMathLibrary.Normal(ToPlayerVector);
            // 计算两个向量之间的夹角（度）
            const DotProduct = UE.KismetMathLibrary.Dot_VectorVector(AIForwardVector, ToPlayerDirection);
            const AngleBetween = UE.KismetMathLibrary.DegAcos(DotProduct);
            // 检查玩家是否在视野角度范围内
            const HalfSightAngle = SightAngle / 2.0;
            if (AngleBetween > HalfSightAngle) {
                // 玩家不在视野范围内
                BlackboardComp.ClearValue("TargetActor");
                return;
            }
        }
        // 如果SightAngle >= 360，直接进入射线检测（全方位感知）
        // 距离和角度都在范围内，进行视线检测
        // 从AI眼睛位置发射射线到玩家位置
        const StartLocation = AIPawnLocation.op_Addition(new UE.Vector(0, 0, EyeHeight));
        const EndLocation = PlayerLocation.op_Addition(new UE.Vector(0, 0, EyeHeight));
        // 准备射线检测
        const HitResult = (0, puerts_1.$ref)(undefined);
        const TraceChannel = UE.ETraceTypeQuery.Visibility; // 使用可见性通道
        // 创建空的Actor数组用于忽略列表
        const ActorsToIgnore = [];
        // 执行射线检测
        const DebugType = bShowDebug ? UE.EDrawDebugTrace.ForDuration : UE.EDrawDebugTrace.None;
        const bHit = UE.KismetSystemLibrary.LineTraceSingle(World, StartLocation, EndLocation, TraceChannel, false, ActorsToIgnore, DebugType, HitResult, true);
        if (bHit && HitResult) {
            const Hit = (0, puerts_1.$unref)(HitResult);
            const HitActor = Hit.GetActor();
            // 检查击中的是不是玩家
            if (HitActor && HitActor.IsA(UE.Character.StaticClass())) {
                const HitCharacter = HitActor;
                if (HitCharacter === PlayerCharacter) {
                    // 看到玩家了，设置黑板值
                    BlackboardComp.SetValueAsObject("TargetActor", PlayerCharacter);
                    // 实时更新玩家最后已知位置
                    BlackboardComp.SetValueAsVector("LastKnownLocation", PlayerLocation);
                }
                else {
                    // 射线击中了其他角色（障碍物），丢失玩家
                    this.HandleLostPlayer(BlackboardComp);
                }
            }
            else {
                // 射线击中了非角色物体（墙壁等），丢失玩家
                this.HandleLostPlayer(BlackboardComp);
            }
        }
        else {
            // 射线没有击中任何东西，说明可以看到玩家
            BlackboardComp.SetValueAsObject("TargetActor", PlayerCharacter);
            // 实时更新玩家最后已知位置
            BlackboardComp.SetValueAsVector("LastKnownLocation", PlayerLocation);
        }
    }
    /**
     * 处理丢失玩家的逻辑
     * 保存最后已知位置，然后清除目标
     */
    HandleLostPlayer(BlackboardComp) {
        // 获取当前的TargetActor
        const CurrentTarget = BlackboardComp.GetValueAsObject("TargetActor");
        // 只有在之前有目标的情况下才保存位置（避免重复保存）
        if (CurrentTarget) {
            // 获取LastKnownLocation，如果已经设置过就不再覆盖
            const ExistingLocation = BlackboardComp.GetValueAsVector("LastKnownLocation");
            // 如果还没有LastKnownLocation，或者之前的位置是(0,0,0)，才保存新位置
            if (!ExistingLocation || (ExistingLocation.X === 0 && ExistingLocation.Y === 0 && ExistingLocation.Z === 0)) {
                const TargetActor = CurrentTarget;
                if (TargetActor) {
                    const LastLocation = TargetActor.K2_GetActorLocation();
                    BlackboardComp.SetValueAsVector("LastKnownLocation", LastLocation);
                    console.log(`BTS_PlayerPerception: 丢失玩家，保存最后位置 (${LastLocation.X.toFixed(0)}, ${LastLocation.Y.toFixed(0)}, ${LastLocation.Z.toFixed(0)})`);
                }
            }
        }
        // 清除目标
        BlackboardComp.ClearValue("TargetActor");
    }
}
puerts_1.blueprint.mixin(jsCls, BTS_PlayerPerception);
//# sourceMappingURL=BTS_PlayerPerception.js.map