"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const UE = require("ue");
const puerts_1 = require("puerts");
let ucls = UE.Class.Load("/Game/Game/GameMap/Blueprint/Enemy/BP_AttackFX.BP_AttackFX_C");
let jsCls = puerts_1.blueprint.tojs(ucls);
class BP_AttackFX {
}
puerts_1.blueprint.mixin(jsCls, BP_AttackFX);
//# sourceMappingURL=BP_AttackFX.js.map