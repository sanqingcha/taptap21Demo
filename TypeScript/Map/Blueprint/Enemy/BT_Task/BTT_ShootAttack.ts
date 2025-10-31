import * as UE from 'ue';
import { $Nullable, blueprint } from 'puerts';

let ucls = UE.Class.Load("/Game/Game/GameMap/Blueprint/Enemy/BT_Task/BTT_ShootAttack.BTT_ShootAttack_C")
let jsCls = blueprint.tojs<typeof UE.Game.Game.GameMap.Blueprint.Enemy.BT_Task.BTT_ShootAttack.BTT_ShootAttack_C>(ucls);
interface BTT_ShootAttack extends UE.Game.Game.GameMap.Blueprint.Enemy.BT_Task.BTT_ShootAttack.BTT_ShootAttack_C { }

class BTT_ShootAttack implements BTT_ShootAttack {
    MuzzleSocketName: string;   // 枪口插槽名称

    ReceiveExecuteAI(OwnerController: $Nullable<UE.AIController>, ControlledPawn: $Nullable<UE.Pawn>): void {
        if (!OwnerController || !ControlledPawn) {
            console.error("BTT_ShootAttack: Invalid OwnerController or ControlledPawn");
            this.FinishExecute(false);
            return;
        }

        const World = ControlledPawn.GetWorld();
        if (!World) {
            console.error("BTT_ShootAttack: World not found");
            this.FinishExecute(false);
            return;
        }

        const PlayerCharacter = UE.GameplayStatics.GetPlayerCharacter(World, 0);
        if (!PlayerCharacter) {
            console.error("BTT_ShootAttack: Player character not found");
            this.FinishExecute(false);
            return;
        }

        const Character = ControlledPawn as UE.Character;
        if (!Character) {
            console.error("BTT_ShootAttack: ControlledPawn is not a Character");
            this.FinishExecute(false);
            return;
        }

        const MeshComp = Character.Mesh;
        if (!MeshComp) {
            console.error("BTT_ShootAttack: Mesh component not found");
            this.FinishExecute(false);
            return;
        }

        const SocketName = this.MuzzleSocketName || "Shoot";
        let SpawnLocation: UE.Vector;
        let SpawnRotation: UE.Rotator;

        if (MeshComp.DoesSocketExist(SocketName)) {
            // 从插槽获取位置（使用世界空间）
            SpawnLocation = MeshComp.GetSocketLocation(SocketName);

            // 计算朝向玩家的方向
            const PlayerLocation = PlayerCharacter.K2_GetActorLocation();
            const DirectionToPlayer = UE.KismetMathLibrary.Subtract_VectorVector(PlayerLocation, SpawnLocation);
            SpawnRotation = UE.KismetMathLibrary.MakeRotFromX(DirectionToPlayer);

            console.log(`BTT_ShootAttack: Spawning bullet at socket '${SocketName}'`);
        } else {
            // 插槽不存在，使用角色前方位置
            console.warn(`BTT_ShootAttack: Socket '${SocketName}' not found, using actor forward direction`);

            SpawnLocation = ControlledPawn.K2_GetActorLocation();
            // 在角色前方 100 单位处生成
            const ForwardVector = ControlledPawn.GetActorForwardVector();
            SpawnLocation = SpawnLocation.op_Addition(
                UE.KismetMathLibrary.Multiply_VectorFloat(ForwardVector, 100.0)
            );

            // 计算朝向玩家的方向
            const PlayerLocation = PlayerCharacter.K2_GetActorLocation();
            const DirectionToPlayer = UE.KismetMathLibrary.Subtract_VectorVector(PlayerLocation, SpawnLocation);
            SpawnRotation = UE.KismetMathLibrary.MakeRotFromX(DirectionToPlayer);
        }

        // 加载子弹蓝图类
        const BulletClass = UE.Class.Load("/Game/Game/GameMap/Blueprint/Enemy/BP_EnemyBullet.BP_EnemyBullet_C");
        if (!BulletClass) {
            console.error("BTT_ShootAttack: Failed to load BP_EnemyBullet class");
            this.FinishExecute(false);
            return;
        }

        // 生成子弹
        const Bullet = UE.GameplayStatics.BeginDeferredActorSpawnFromClass(
            World,
            BulletClass,
            new UE.Transform(
                SpawnRotation.Quaternion(),
                SpawnLocation,
                new UE.Vector(1, 1, 1)
            ),
            UE.ESpawnActorCollisionHandlingMethod.AlwaysSpawn,
            ControlledPawn
        );

        if (!Bullet) {
            console.error("BTT_ShootAttack: Failed to spawn bullet actor");
            this.FinishExecute(false);
            return;
        }

        console.log("BTT_ShootAttack: Bullet spawned successfully");

        const BulletObj = Bullet as any;
        if (BulletObj.Pawn !== undefined) {
            BulletObj.Pawn = ControlledPawn;
            console.log("BTT_ShootAttack: Assigned Pawn to bullet");
        }

        UE.GameplayStatics.FinishSpawningActor(Bullet, new UE.Transform(
            SpawnRotation.Quaternion(),
            SpawnLocation,
            new UE.Vector(1, 1, 1)
        ));

        this.FinishExecute(true);
    }
}
blueprint.mixin(jsCls, BTT_ShootAttack);
