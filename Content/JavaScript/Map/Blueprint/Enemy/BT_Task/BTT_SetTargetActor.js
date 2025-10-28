"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const UE = require("ue");
const puerts_1 = require("puerts");
let ucls = UE.Class.Load("/Game/Game/GameMap/Blueprint/Enemy/BT_Task/BTT_SetTargetActor.BTT_SetTargetActor_C");
let jsCls = puerts_1.blueprint.tojs(ucls);
class BTT_SetTargetActor {
    ReceiveExecuteAI(OwnerController, ControlledPawn) {
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
puerts_1.blueprint.mixin(jsCls, BTT_SetTargetActor);
//# sourceMappingURL=BTT_SetTargetActor.js.map