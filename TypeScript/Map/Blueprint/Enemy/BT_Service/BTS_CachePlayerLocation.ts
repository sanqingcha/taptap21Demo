import * as UE from 'ue';
import { $Nullable, blueprint } from 'puerts';

let ucls = UE.Class.Load("/Game/Game/GameMap/Blueprint/Enemy/BT_Service/BTS_CachePlayerLocation.BTS_CachePlayerLocation_C")
let jsCls = blueprint.tojs<typeof UE.Game.Game.GameMap.Blueprint.Enemy.BT_Service.BTS_CachePlayerLocation.BTS_CachePlayerLocation_C>(ucls);
interface BTS_CachePlayerLocation extends UE.Game.Game.GameMap.Blueprint.Enemy.BT_Service.BTS_CachePlayerLocation.BTS_CachePlayerLocation_C { }

class BTS_CachePlayerLocation implements BTS_CachePlayerLocation {
    ReceiveTickAI(OwnerController: $Nullable<UE.AIController>, ControlledPawn: $Nullable<UE.Pawn>, DeltaSeconds: number): void {
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
blueprint.mixin(jsCls, BTS_CachePlayerLocation);
