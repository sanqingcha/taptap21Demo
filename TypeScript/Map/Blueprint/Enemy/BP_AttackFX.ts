import * as UE from 'ue';
import { blueprint } from 'puerts';

let ucls = UE.Class.Load("/Game/Game/GameMap/Blueprint/Enemy/BP_AttackFX.BP_AttackFX_C")
let jsCls = blueprint.tojs<typeof UE.Game.Game.GameMap.Blueprint.Enemy.BP_AttackFX.BP_AttackFX_C>(ucls);
interface BP_AttackFX extends UE.Game.Game.GameMap.Blueprint.Enemy.BP_AttackFX.BP_AttackFX_C { }

class BP_AttackFX implements BP_AttackFX {
	
}
blueprint.mixin(jsCls, BP_AttackFX);
