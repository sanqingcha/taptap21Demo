import * as UE from 'ue';
import { $Nullable, blueprint } from 'puerts';

let ucls = UE.Class.Load("/Game/Game/GameMap/Blueprint/Enemy/BT_Task/BTT_MeleeAttack.BTT_MeleeAttack_C")
let jsCls = blueprint.tojs<typeof UE.Game.Game.GameMap.Blueprint.Enemy.BT_Task.BTT_MeleeAttack.BTT_MeleeAttack_C>(ucls);
interface BTT_MeleeAttack extends UE.Game.Game.GameMap.Blueprint.Enemy.BT_Task.BTT_MeleeAttack.BTT_MeleeAttack_C { }

class BTT_MeleeAttack implements BTT_MeleeAttack {
    EnemyHitEffectClass: UE.Class;

    ReceiveExecuteAI(OwnerController: $Nullable<UE.AIController>, ControlledPawn: $Nullable<UE.Pawn>): void {
        if (!OwnerController || !ControlledPawn) {
            console.error("BTT_MeleeAttack: Invalid OwnerController or ControlledPawn");
            this.FinishExecute(false);
            return;
        }

        const World = ControlledPawn.GetWorld();
        if (!World) {
            console.error("BTT_MeleeAttack: World not found");
            this.FinishExecute(false);
            return;
        }

        const PlayerCharacter = UE.GameplayStatics.GetPlayerCharacter(World, 0);
        if (!PlayerCharacter) {
            console.error("BTT_MeleeAttack: Player character not found");
            this.FinishExecute(false);
            return;
        }

        const GE_PATH = "/Game/Game/GameMap/Blueprint/Gas/GE/BP_EnemyDefaultAttack.BP_EnemyDefaultAttack_C"; 
        this.EnemyHitEffectClass = UE.Class.Load(GE_PATH);
        
        if (!this.EnemyHitEffectClass) {
            console.error(`BTT_MeleeAttack: Failed to load GE Class at: ${GE_PATH}`);
            this.FinishExecute(false);
            return;
        }

        const SourceASC = ControlledPawn.GetComponentByClass(UE.AbilitySystemComponent.StaticClass()) as UE.AbilitySystemComponent;
        if (!SourceASC) {
            console.error(`BTT_MeleeAttack: AI Pawn ${ControlledPawn.GetName()} has no AbilitySystemComponent`);
            this.FinishExecute(false);
            return;
        }

        const TargetASC = PlayerCharacter.GetComponentByClass(UE.AbilitySystemComponent.StaticClass()) as UE.AbilitySystemComponent;
        if (!TargetASC) {
            console.error(`BTT_MeleeAttack: Player ${PlayerCharacter.GetName()} has no AbilitySystemComponent`);
            this.FinishExecute(false);
            return;
        }
        
        const EffectLevel = 0.0;
        const ContextHandle = SourceASC.MakeEffectContext();
        
        const bSuccess = SourceASC.BP_ApplyGameplayEffectToTarget(
            this.EnemyHitEffectClass,
            TargetASC,
            EffectLevel,
            ContextHandle
        );

        if (bSuccess) {
            console.log("BTT_MeleeAttack: Attack applied successfully");
            this.FinishExecute(true);
        } else {
            console.error("BTT_MeleeAttack: Failed to apply attack effect");
            this.FinishExecute(false);
        }
    }
}
blueprint.mixin(jsCls, BTT_MeleeAttack);
