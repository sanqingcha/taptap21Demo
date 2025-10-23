"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const UE = require("ue");
const puerts_1 = require("puerts");
let ucls = UE.Class.Load("/Game/Game/GameMap/Blueprint/Enemy/BT_Task/BTT_ShootAttack.BTT_ShootAttack_C");
let jsCls = puerts_1.blueprint.tojs(ucls);
class BTT_ShootAttack {
    ReceiveExecuteAI(OwnerController, ControlledPawn) {
        console.log("远程攻击！");
        this.FinishExecute(true);
    }
}
puerts_1.blueprint.mixin(jsCls, BTT_ShootAttack);
//# sourceMappingURL=BTT_ShootAttack.js.map