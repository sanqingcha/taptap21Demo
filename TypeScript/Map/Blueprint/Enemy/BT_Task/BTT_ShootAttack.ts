import * as UE from 'ue';
import { $Nullable, blueprint } from 'puerts';

let ucls = UE.Class.Load("/Game/Game/GameMap/Blueprint/Enemy/BT_Task/BTT_ShootAttack.BTT_ShootAttack_C")
let jsCls = blueprint.tojs<typeof UE.Game.Game.GameMap.Blueprint.Enemy.BT_Task.BTT_ShootAttack.BTT_ShootAttack_C>(ucls);
interface BTT_ShootAttack extends UE.Game.Game.GameMap.Blueprint.Enemy.BT_Task.BTT_ShootAttack.BTT_ShootAttack_C { }

class BTT_ShootAttack implements BTT_ShootAttack {
    ReceiveExecuteAI(OwnerController: $Nullable<UE.AIController>, ControlledPawn: $Nullable<UE.Pawn>): void {
        console.log("远程攻击！");
        this.FinishExecute(true);
    }
}
blueprint.mixin(jsCls, BTT_ShootAttack);
