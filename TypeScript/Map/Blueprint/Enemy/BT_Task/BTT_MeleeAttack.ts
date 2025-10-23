import * as UE from 'ue';
import { $Nullable, blueprint } from 'puerts';

let ucls = UE.Class.Load("/Game/Game/GameMap/Blueprint/Enemy/BT_Task/BTT_MeleeAttack.BTT_MeleeAttack_C")
let jsCls = blueprint.tojs<typeof UE.Game.Game.GameMap.Blueprint.Enemy.BT_Task.BTT_MeleeAttack.BTT_MeleeAttack_C>(ucls);
interface BTT_MeleeAttack extends UE.Game.Game.GameMap.Blueprint.Enemy.BT_Task.BTT_MeleeAttack.BTT_MeleeAttack_C { }

class BTT_MeleeAttack implements BTT_MeleeAttack {
    ReceiveExecuteAI(OwnerController: $Nullable<UE.AIController>, ControlledPawn: $Nullable<UE.Pawn>): void {
        console.log("近战攻击！");
        this.FinishExecute(true);
    }
}
blueprint.mixin(jsCls, BTT_MeleeAttack);
