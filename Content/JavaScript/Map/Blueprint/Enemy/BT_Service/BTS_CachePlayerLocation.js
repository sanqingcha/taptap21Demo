"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const UE = require("ue");
const puerts_1 = require("puerts");
let ucls = UE.Class.Load("/Game/Game/GameMap/Blueprint/Enemy/BT_Service/BTS_CachePlayerLocation.BTS_CachePlayerLocation_C");
let jsCls = puerts_1.blueprint.tojs(ucls);
class BTS_CachePlayerLocation {
    ReceiveTickAI(OwnerController, ControlledPawn, DeltaSeconds) {
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
        if (!PlayerCharacter) {
            BlackboardComp.ClearValue("TargetActor");
            return;
        }
        const PlayerLocation = PlayerCharacter.K2_GetActorLocation();
        BlackboardComp.SetValueAsVector("LastKnownLocation", PlayerLocation);
    }
}
puerts_1.blueprint.mixin(jsCls, BTS_CachePlayerLocation);
//# sourceMappingURL=BTS_CachePlayerLocation.js.map