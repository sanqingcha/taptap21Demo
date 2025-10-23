"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const UE = require("ue");
const puerts_1 = require("puerts");
let ucls = UE.Class.Load("/Game/Game/GameMap/Blueprint/Enemy/BT_Task/BTT_MeleeAttack.BTT_MeleeAttack_C");
let jsCls = puerts_1.blueprint.tojs(ucls);
class BTT_MeleeAttack {
    ReceiveExecuteAI(OwnerController, ControlledPawn) {
        console.log("近战攻击！");
        this.FinishExecute(true);
    }
}
puerts_1.blueprint.mixin(jsCls, BTT_MeleeAttack);
//# sourceMappingURL=BTT_MeleeAttack.js.map