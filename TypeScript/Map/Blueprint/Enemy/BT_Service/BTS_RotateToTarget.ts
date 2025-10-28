import * as UE from 'ue';
import { $Nullable, blueprint } from 'puerts';

let ucls = UE.Class.Load("/Game/Game/GameMap/Blueprint/Enemy/BT_Service/BTS_RotateToTarget.BTS_RotateToTarget_C")
let jsCls = blueprint.tojs<typeof UE.Game.Game.GameMap.Blueprint.Enemy.BT_Service.BTS_RotateToTarget.BTS_RotateToTarget_C>(ucls);
interface BTS_RotateToTarget extends UE.Game.Game.GameMap.Blueprint.Enemy.BT_Service.BTS_RotateToTarget.BTS_RotateToTarget_C { }

class BTS_RotateToTarget implements BTS_RotateToTarget {

    Target: UE.BlackboardKeySelector;

    /** 旋转插值速度 */
    RotationInterpSpeed: number;

    /** 最小旋转距离 */
    MinRotationDistance: number;

    /** 角度容差（当角度差小于此值时停止旋转，避免抖动） */
    AngleTolerance: number;

    /** 是否只旋转 Yaw（偏航角），忽略 Pitch 和 Roll */
    bYawOnly: boolean;

    bShowDebug: boolean;


    private GetTargetLocation(BlackboardComp: UE.BlackboardComponent): $Nullable<UE.Vector> {
        if (!this.Target || !this.Target.SelectedKeyName) {
            return null;
        }

        const KeyName = this.Target.SelectedKeyName;

        const VectorLocation = BlackboardComp.GetValueAsVector(KeyName);
        if (VectorLocation && !VectorLocation.IsZero()) {
            return VectorLocation;
        }

        const TargetActor = BlackboardComp.GetValueAsObject(KeyName) as UE.Actor;
        if (TargetActor) {
            return TargetActor.K2_GetActorLocation();
        }

        return null;
    }

    ReceiveTickAI(OwnerController: $Nullable<UE.AIController>, ControlledPawn: $Nullable<UE.Pawn>, DeltaSeconds: number): void {
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
                console.error("BTS_RotateToTarget: Blackboard component not found");
            }
            return;
        }

        if (!this.Target || !this.Target.SelectedKeyName) {
            if (this.bShowDebug) {
                console.error("BTS_RotateToTarget: Target key is not set");
            }
            return;
        }

        const TargetLocation = this.GetTargetLocation(BlackboardComp);

        if (!TargetLocation) {
            if (this.bShowDebug) {
                console.warn("BTS_RotateToTarget: Target location is invalid");
            }
            return;
        }

        const CurrentLocation = ControlledPawn.K2_GetActorLocation();
        const CurrentRotation = ControlledPawn.K2_GetActorRotation();

        // 计算朝向目标的方向向量
        const DirectionToTarget = UE.KismetMathLibrary.Subtract_VectorVector(TargetLocation, CurrentLocation);

        // 计算距离目标的距离
        const DistanceToTarget = UE.KismetMathLibrary.Vector_Distance(CurrentLocation, TargetLocation);

        // 检查距离是否足够远（避免在目标附近抖动）
        const MinDistance = this.MinRotationDistance > 0 ? this.MinRotationDistance : 50.0;
        if (DistanceToTarget < MinDistance) {
            if (this.bShowDebug) {
                console.log(`BTS_RotateToTarget: 距离目标太近 (${DistanceToTarget.toFixed(0)} < ${MinDistance.toFixed(0)})，停止旋转`);
            }
            return;
        }

        // 计算目标旋转
        let TargetRotation: UE.Rotator;

        if (this.bYawOnly) {
            const YawRotation = UE.KismetMathLibrary.MakeRotFromX(DirectionToTarget);
            TargetRotation = new UE.Rotator(CurrentRotation.Pitch, YawRotation.Yaw, CurrentRotation.Roll);
        } else {
            TargetRotation = UE.KismetMathLibrary.MakeRotFromX(DirectionToTarget);
        }

        // 计算当前旋转和目标旋转之间的角度差
        const RotationDelta = UE.KismetMathLibrary.NormalizedDeltaRotator(TargetRotation, CurrentRotation);
        const AngleDifference = Math.sqrt(
            RotationDelta.Pitch * RotationDelta.Pitch +
            RotationDelta.Yaw * RotationDelta.Yaw +
            RotationDelta.Roll * RotationDelta.Roll
        );

        // 检查角度差是否足够大（避免已经朝向时还在插值）
        const AngleTolerance = this.AngleTolerance > 0 ? this.AngleTolerance : 2.0;
        if (AngleDifference < AngleTolerance) {
            if (this.bShowDebug) {
                console.log(`BTS_RotateToTarget: 已朝向目标 (角度差=${AngleDifference.toFixed(1)}° < ${AngleTolerance.toFixed(1)}°)`);
            }
            return;
        }

        // 平滑插值到目标旋转
        const InterpSpeed = this.RotationInterpSpeed > 0 ? this.RotationInterpSpeed : 8.0;
        const NewRotation = UE.KismetMathLibrary.RInterpTo(
            CurrentRotation,
            TargetRotation,
            DeltaSeconds,
            InterpSpeed
        );

        // 应用新旋转
        ControlledPawn.K2_SetActorRotation(NewRotation, false);

        if (this.bShowDebug) {
            const DrawDuration = 0.0;

            const ForwardVector = ControlledPawn.GetActorForwardVector();
            const ArrowEnd = UE.KismetMathLibrary.Add_VectorVector(
                CurrentLocation,
                UE.KismetMathLibrary.Multiply_VectorFloat(ForwardVector, 200.0)
            );

            UE.KismetSystemLibrary.DrawDebugArrow(
                World,
                CurrentLocation,
                ArrowEnd,
                60.0,
                new UE.LinearColor(0, 1, 1, 1),
                DrawDuration,
                3.0
            );

            UE.KismetSystemLibrary.DrawDebugLine(
                World,
                CurrentLocation,
                TargetLocation,
                new UE.LinearColor(1, 0, 1, 0.6),
                DrawDuration,
                2.0
            );

            const CurrentTime = UE.KismetSystemLibrary.GetGameTimeInSeconds(World);
            if (Math.floor(CurrentTime) !== Math.floor(CurrentTime - DeltaSeconds)) {
                console.log(`BTS_RotateToTarget: 旋转中 - 角度差=${AngleDifference.toFixed(1)}°, 距离=${DistanceToTarget.toFixed(0)}, 插值速度=${InterpSpeed.toFixed(1)}`);
            }
        }
    }
}

blueprint.mixin(jsCls, BTS_RotateToTarget);
