import * as UE from 'ue';
import { $Nullable, blueprint } from 'puerts';

let ucls = UE.Class.Load("/Game/Game/GameMap/Blueprint/Enemy/BT_Task/BTT_ClearLastKnownLocation.BTT_ClearLastKnownLocation_C")
let jsCls = blueprint.tojs<typeof UE.Game.Game.GameMap.Blueprint.Enemy.BT_Task.BTT_ClearLastKnownLocation.BTT_ClearLastKnownLocation_C>(ucls);
interface BTT_ClearLastKnownLocation extends UE.Game.Game.GameMap.Blueprint.Enemy.BT_Task.BTT_ClearLastKnownLocation.BTT_ClearLastKnownLocation_C { }


/**
 *
 *
 * @class 清除最后已知玩家位置
 * @implements {BTT_ClearLastKnownLocation}
 */
class BTT_ClearLastKnownLocation implements BTT_ClearLastKnownLocation {
    ReceiveExecuteAI(OwnerController: $Nullable<UE.AIController>, ControlledPawn: $Nullable<UE.Pawn>): void {
        if (!OwnerController) {
            console.error("BTT_ClearLastKnownLocation: OwnerController 为空");
            this.FinishExecute(false);
            return;
        }
        const BlackboardComp = OwnerController.Blackboard;
        if (!BlackboardComp) {
            console.error("BTT_ClearLastKnownLocation: Blackboard 组件未找到");
            this.FinishExecute(false);
            return;
        }
        BlackboardComp.ClearValue("LastKnownLocation");
        this.FinishExecute(true);
    }
}

blueprint.mixin(jsCls, BTT_ClearLastKnownLocation);
