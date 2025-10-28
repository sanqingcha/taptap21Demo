import * as UE from 'ue';
import { $Nullable, blueprint } from 'puerts';

let ucls = UE.Class.Load("/Game/Game/GameMap/Blueprint/Enemy/BT_Task/BTT_SetTargetActor.BTT_SetTargetActor_C")
let jsCls = blueprint.tojs<typeof UE.Game.Game.GameMap.Blueprint.Enemy.BT_Task.BTT_SetTargetActor.BTT_SetTargetActor_C>(ucls);
interface BTT_SetTargetActor extends UE.Game.Game.GameMap.Blueprint.Enemy.BT_Task.BTT_SetTargetActor.BTT_SetTargetActor_C { }

class BTT_SetTargetActor implements BTT_SetTargetActor {
    ReceiveExecuteAI(OwnerController: $Nullable<UE.AIController>, ControlledPawn: $Nullable<UE.Pawn>): void {
        const World = ControlledPawn.GetWorld();
        if (!World) {
            return;
        }

        const BlackboardComp = OwnerController.Blackboard;
        if (!BlackboardComp) {
            console.error("BTS_PlayerPerception: Blackboard component not found");
            return;
        }
        const PlayerCharacter = UE.GameplayStatics.GetPlayerCharacter(World, 0);
        BlackboardComp.SetValueAsObject("TargetActor", PlayerCharacter);
        this.FinishExecute(true);
    }
}
blueprint.mixin(jsCls, BTT_SetTargetActor);
