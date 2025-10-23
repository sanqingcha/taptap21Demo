"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const UE = require("ue");
const puerts_1 = require("puerts");
let ucls = UE.Class.Load("/Game/Game/GameMap/Blueprint/Enemy/BT_Task/BTT_ClearLastKnownLocation.BTT_ClearLastKnownLocation_C");
let jsCls = puerts_1.blueprint.tojs(ucls);
/**
 *
 *
 * @class 清除最后已知玩家位置
 * @implements {BTT_ClearLastKnownLocation}
 */
class BTT_ClearLastKnownLocation {
    ReceiveExecuteAI(OwnerController, ControlledPawn) {
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
puerts_1.blueprint.mixin(jsCls, BTT_ClearLastKnownLocation);
//# sourceMappingURL=BTT_ClearLastKnownLocation.js.map