import * as UE from 'ue';
import { $Nullable, blueprint } from 'puerts';

let ucls = UE.Class.Load("/Game/Game/GameMap/Blueprint/Enemy/BT_Decorator/BTD_IsAtTarget.BTD_IsAtTarget_C")
let jsCls = blueprint.tojs<typeof UE.Game.Game.GameMap.Blueprint.Enemy.BT_Decorator.BTD_IsAtTarget.BTD_IsAtTarget_C>(ucls);
interface BTD_IsAtTarget extends UE.Game.Game.GameMap.Blueprint.Enemy.BT_Decorator.BTD_IsAtTarget.BTD_IsAtTarget_C { }

/**
 *
 *
 * @class 检测自己与目标的距离
 * @implements {BTD_IsAtTarget}
 */
class BTD_IsAtTarget implements BTD_IsAtTarget {
    TargetActor: UE.BlackboardKeySelector;
    AcceptableRadius: UE.BlackboardKeySelector;
    bInvertCondition: boolean;
    bUse2DDistance: boolean;

    PerformConditionCheckAI(OwnerController: $Nullable<UE.AIController>, ControlledPawn: $Nullable<UE.Pawn>): boolean {
        if (!OwnerController || !ControlledPawn) {
            console.warn("BTD_IsAtTarget: OwnerController 或 ControlledPawn 为空");
            return false;
        }

        const BlackboardComp = OwnerController.Blackboard;
        if (!BlackboardComp) {
            console.error("BTD_IsAtTarget: Blackboard 组件未找到");
            return false;
        }

        if (!this.TargetActor) {
            console.error("BTD_IsAtTarget: TargetActor 未设置");
            return false;
        }

        const PawnLocation = ControlledPawn.K2_GetActorLocation();
        let TargetLocation: UE.Vector | null = null;
        const TargetActor = BlackboardComp.GetValueAsObject(this.TargetActor.SelectedKeyName) as UE.Actor;
        if (TargetActor) {
            TargetLocation = TargetActor.K2_GetActorLocation();
        } else {
            const TargetVector = BlackboardComp.GetValueAsVector(this.TargetActor.SelectedKeyName);
            if (TargetVector) {
                TargetLocation = TargetVector;
            }
        }

        if (!TargetLocation) {
            console.warn(`BTD_IsAtTarget: 目标 "${this.TargetActor.SelectedKeyName}" 未设置或无效`);
            return false;
        }

        let CurrentDistance: number;
        const bUse2D = this.bUse2DDistance || false;

        if (bUse2D) {
            CurrentDistance = UE.KismetMathLibrary.Vector_Distance2D(PawnLocation, TargetLocation);
        } else {
            CurrentDistance = UE.KismetMathLibrary.Vector_Distance(PawnLocation, TargetLocation);
        }
        const AcceptableRadius = BlackboardComp.GetValueAsFloat(this.AcceptableRadius.SelectedKeyName);
        const DistanceThreshold = AcceptableRadius || 200.0;
        const bWithinDistance = CurrentDistance <= DistanceThreshold;
        const bInvert = this.bInvertCondition || false;
        const result = bInvert ? !bWithinDistance : bWithinDistance;
        return result;
    }

}

blueprint.mixin(jsCls, BTD_IsAtTarget);
