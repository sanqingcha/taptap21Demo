import * as UE from 'ue';
import { blueprint, $ref, $unref } from 'puerts';

let ucls = UE.Class.Load("/Game/Game/GameMap/Blueprint/Actor/BP_EnemySpawnArea.BP_EnemySpawnArea_C")
let jsCls = blueprint.tojs<typeof UE.Game.Game.GameMap.Blueprint.Actor.BP_EnemySpawnArea.BP_EnemySpawnArea_C>(ucls);
interface BP_EnemySpawnArea extends UE.Game.Game.GameMap.Blueprint.Actor.BP_EnemySpawnArea.BP_EnemySpawnArea_C { }

class BP_EnemySpawnArea implements BP_EnemySpawnArea {
    // 可在蓝图中编辑的参数
    SpawnInterval: number; // 刷怪间隔(秒)
    MaxEnemyCount: number; // 最大怪物数量
    FlyingHeight: number; // 飞行怪物高度
    SpawnRadius: number; // 刷怪半径
    bEnableSpawn: boolean; // 是否启用刷怪 (true=刷怪, false=暂停)

    // 私有变量
    private SpawnTimerHandle: number = -1;
    private CurrentEnemyCount: number = 0;
    private SpawnedEnemies: UE.Actor[];
    private NavigationSystem: UE.NavigationSystemV1;

    // 怪物类缓存（硬编码路径）
    private GroundEnemyConfigs: Array<{ enemyClass: UE.Class, weight: number }> = [];
    private FlyingEnemyConfigs: Array<{ enemyClass: UE.Class, weight: number }> = [];
    
    Constructor(): void {
        this.SpawnedEnemies = [];
        this.GroundEnemyConfigs = [];
        this.FlyingEnemyConfigs = [];
    }
    
    ReceiveBeginPlay(): void {
        if (!this.SpawnedEnemies) {
            this.SpawnedEnemies = [];
        }
        if (!this.GroundEnemyConfigs) {
            this.GroundEnemyConfigs = [];
        }
        if (!this.FlyingEnemyConfigs) {
            this.FlyingEnemyConfigs = [];
        }

        this.NavigationSystem = UE.NavigationSystemV1.GetNavigationSystem(this);

        // 硬编码加载怪物类
        this.InitializeEnemyClasses();

        // 启动刷怪定时器
        this.StartSpawnTimer();

        console.log(`刷怪区域已启动，刷怪间隔: ${this.SpawnInterval}秒`);
        console.log(`地面怪物种类数: ${this.GroundEnemyConfigs.length}`);
        console.log(`飞行怪物种类数: ${this.FlyingEnemyConfigs.length}`);
    }

    /**
     * 初始化怪物类（硬编码路径）
     */
    InitializeEnemyClasses(): void {
        // 地面怪物配置 - 修复路径到正确的 Enemy 目录
        try {
            let BugEgg = UE.Class.Load("/Game/Game/GameMap/Blueprint/Enemy/BP_BugEgg/BP_BugEgg.BP_BugEgg_C");
            if (BugEgg) {
                this.GroundEnemyConfigs.push({ enemyClass: BugEgg, weight: 1.0 });
                console.log("成功加载地面怪物: BP_BugEgg");
            } else {
                console.warn("加载 BP_BugEgg 失败");
            }

            let BugEggShooter = UE.Class.Load("/Game/Game/GameMap/Blueprint/Enemy/BP_BugEggShooter/BP_BugEggShooter.BP_BugEggShooter_C");
            if (BugEggShooter) {
                this.GroundEnemyConfigs.push({ enemyClass: BugEggShooter, weight: 1.0 });
                console.log("成功加载地面怪物: BP_BugEggShooter");
            } else {
                console.warn("加载 BP_BugEggShooter 失败");
            }

            let BugEggJumper = UE.Class.Load("/Game/Game/GameMap/Blueprint/Enemy/BP_BugEggJumper/BP_BugEggJumper.BP_BugEggJumper_C");
            if (BugEggJumper) {
                this.GroundEnemyConfigs.push({ enemyClass: BugEggJumper, weight: 1.0 });
                console.log("成功加载地面怪物: BP_BugEggJumper");
            } else {
                console.warn("加载 BP_BugEggJumper 失败");
            }
        } catch (e) {
            console.error("加载地面怪物类失败:", e);
        }

        // 飞行怪物配置
        try {
            let BugEggCopilot = UE.Class.Load("/Game/Game/GameMap/Blueprint/Enemy/BP_BugEggCopilot/BP_BugEggCopilot.BP_BugEggCopilot_C");
            if (BugEggCopilot) {
                this.FlyingEnemyConfigs.push({ enemyClass: BugEggCopilot, weight: 1.0 });
                console.log("成功加载飞行怪物: BP_BugEggCopilot");
            } else {
                console.warn("加载 BP_BugEggCopilot 失败");
            }
        } catch (e) {
            console.error("加载飞行怪物类失败:", e);
        }

        console.log(`成功加载 ${this.GroundEnemyConfigs.length} 个地面怪物, ${this.FlyingEnemyConfigs.length} 个飞行怪物`);
    }
    
    /**
     * 启动刷怪定时器
     */
    StartSpawnTimer(): void {
        let World = this.GetWorld();
        if (!World) return;

        // 使用 JavaScript 的 setInterval 代替 UE 定时器
        this.SpawnTimerHandle = setInterval(() => {
            this.OnSpawnTimer();
        }, this.SpawnInterval * 1000); // 转换为毫秒

        console.log(`刷怪定时器已启动，间隔: ${this.SpawnInterval}秒`);
    }

    /**
     * 定时器回调
     */
    OnSpawnTimer(): void {
        this.TrySpawnEnemy();
    }

    /**
     * 停止刷怪定时器
     */
    StopSpawnTimer(): void {
        if (this.SpawnTimerHandle !== -1) {
            clearInterval(this.SpawnTimerHandle);
            this.SpawnTimerHandle = -1;
            console.log("刷怪定时器已停止");
        }
    }
    
    /**
     * 尝试生成怪物
     */
    TrySpawnEnemy(): void {
        // 检查刷怪开关
        if (!this.bEnableSpawn) {
            console.log("刷怪已暂停 (bEnableSpawn = false)");
            return;
        }

        // 清理已销毁的怪物引用
        console.log("尝试生成怪物...");
        this.CleanupDestroyedEnemies();
        
        // 检查是否达到最大数量
        if (this.CurrentEnemyCount >= this.MaxEnemyCount) {
            return;
        }
        
        // 根据权重随机选择怪物类型
        let selectedEnemy = this.SelectEnemyByWeight();
        if (!selectedEnemy.enemyClass) {
            console.warn("未配置有效的怪物类!");
            return;
        }
        
        // 根据怪物类型获取生成位置
        let spawnLocation: UE.Vector;
        if (selectedEnemy.isFlying) {
            spawnLocation = this.GetRandomFlyingLocation();
        } else {
            spawnLocation = this.GetRandomNavigableLocation();
        }
        
        if (!spawnLocation) {
            console.warn("未找到合法的生成位置!");
            return;
        }
        
        // 生成怪物
        this.SpawnEnemyAtLocation(selectedEnemy.enemyClass, spawnLocation);
    }
    
    /**
     * 根据权重选择怪物类型
     */
    SelectEnemyByWeight(): { enemyClass: UE.Class, isFlying: boolean } {
        let enemies: Array<{ enemyClass: UE.Class, weight: number, isFlying: boolean }> = [];

        // 从地面怪物配置中获取
        for (let config of this.GroundEnemyConfigs) {
            enemies.push({
                enemyClass: config.enemyClass,
                weight: config.weight,
                isFlying: false
            });
        }

        // 从飞行怪物配置中获取
        for (let config of this.FlyingEnemyConfigs) {
            enemies.push({
                enemyClass: config.enemyClass,
                weight: config.weight,
                isFlying: true
            });
        }

        if (enemies.length === 0) {
            return { enemyClass: null, isFlying: false };
        }

        // 计算总权重
        let totalWeight = 0;
        for (let enemy of enemies) {
            totalWeight += enemy.weight;
        }

        // 随机选择
        let randomValue = Math.random() * totalWeight;
        let currentWeight = 0;

        for (let enemy of enemies) {
            currentWeight += enemy.weight;
            if (randomValue <= currentWeight) {
                return {
                    enemyClass: enemy.enemyClass,
                    isFlying: enemy.isFlying
                };
            }
        }

        // 默认返回第一个
        return {
            enemyClass: enemies[0].enemyClass,
            isFlying: enemies[0].isFlying
        };
    }
    
    /**
     * 获取导航网格体上的随机合法位置
     */
    GetRandomNavigableLocation(): UE.Vector {
        if (!this.NavigationSystem) {
            console.warn("导航系统未找到!");
            return null;
        }
        
        let actorLocation = this.K2_GetActorLocation();
        
        // 尝试多次查找合法位置
        for (let i = 0; i < 10; i++) {
            // 在半径内随机一个位置
            let randomOffset = new UE.Vector(
                (Math.random() - 0.5) * 2 * this.SpawnRadius,
                (Math.random() - 0.5) * 2 * this.SpawnRadius,
                0
            );
            
            let targetLocation = UE.KismetMathLibrary.Add_VectorVector(
                actorLocation,
                randomOffset
            );
            
            // 在导航网格体上查找最近的可到达点
            let outNavLocation = $ref<UE.Vector>();
            let result = UE.NavigationSystemV1.K2_ProjectPointToNavigation(
                this,
                targetLocation,
                outNavLocation,
                null, // NavData
                null, // FilterClass
                new UE.Vector(this.SpawnRadius, this.SpawnRadius, 500) // QueryExtent
            );
            
            if (result) {
                return $unref(outNavLocation);
            }
        }
        
        console.warn("未找到导航网格体上的合法位置!");
        return null;
    }
    
    /**
     * 获取飞行怪物的随机位置
     */
    GetRandomFlyingLocation(): UE.Vector {
        let actorLocation = this.K2_GetActorLocation();
        
        // 在半径内随机一个位置,并设置高度
        let randomOffset = new UE.Vector(
            (Math.random() - 0.5) * 2 * this.SpawnRadius,
            (Math.random() - 0.5) * 2 * this.SpawnRadius,
            this.FlyingHeight
        );
        
        let spawnLocation = UE.KismetMathLibrary.Add_VectorVector(
            actorLocation,
            randomOffset
        );
        
        return spawnLocation;
    }
    
    /**
     * 在指定位置生成怪物
     */
    SpawnEnemyAtLocation(enemyClass: UE.Class, location: UE.Vector): void {
        let World = this.GetWorld();
        if (!World) return;

        // 设置生成参数
        let spawnTransform = new UE.Transform(
            new UE.Rotator(0, Math.random() * 360, 0), // 随机朝向
            location,
            new UE.Vector(1, 1, 1)
        );

        // 生成Actor（不使用 Deferred，直接生成）
        let spawnedEnemy = UE.GameplayStatics.BeginDeferredActorSpawnFromClass(
            this,
            enemyClass,
            spawnTransform,
            UE.ESpawnActorCollisionHandlingMethod.AdjustIfPossibleButAlwaysSpawn,
            this // Owner
        );

        if (spawnedEnemy) {
            // 完成生成
            UE.GameplayStatics.FinishSpawningActor(spawnedEnemy, spawnTransform);

            // 等待 AI Controller 和 Blackboard 初始化
            // 使用递归检查，最多等待 1 秒
            this.WaitForAIInitialized(spawnedEnemy, 0);
        } else {
            console.warn("生成怪物失败!");
        }
    }

    /**
     * 等待 AI 初始化完成
     */
    private WaitForAIInitialized(enemy: UE.Actor, attemptCount: number): void {
        const maxAttempts = 10; // 最多尝试 10 次 (1秒)

        if (attemptCount >= maxAttempts) {
            console.warn(`AI 初始化超时，尝试手动启动 AI (尝试 ${attemptCount} 次)`);
            // 超时后强制添加，让怪物自己处理初始化
            this.SpawnedEnemies.push(enemy);
            this.CurrentEnemyCount++;
            return;
        }

        // 尝试获取 AI Controller（假设 enemy 是 Pawn）
        const pawn = enemy as UE.Pawn;
        const aiController = pawn.Controller as UE.AIController;

        if (aiController) {
            // AI Controller 存在，尝试启动 Behavior Tree
            if (!aiController.Blackboard) {
                // Blackboard 不存在，可能是 BehaviorTree 没运行
                console.log(`尝试手动运行 Behavior Tree (尝试 ${attemptCount + 1}/${maxAttempts})`);

                // 尝试运行行为树（如果 BrainComponent 存在）
                if (aiController.BrainComponent) {
                    aiController.BrainComponent.StartLogic();
                }
            }

            if (aiController.Blackboard) {
                // Blackboard 已初始化
                this.SpawnedEnemies.push(enemy);
                this.CurrentEnemyCount++;
                console.log(`成功生成怪物并启动 AI，当前数量: ${this.CurrentEnemyCount}`);
                return;
            }
        }

        // 还未初始化，100ms 后重试
        setTimeout(() => {
            if (enemy && UE.KismetSystemLibrary.IsValid(enemy)) {
                this.WaitForAIInitialized(enemy, attemptCount + 1);
            }
        }, 100);
    }
    
    /**
     * 清理已销毁的怪物引用
     */
    CleanupDestroyedEnemies(): void {
        if (!this.SpawnedEnemies) return;
        
        let validEnemies: UE.Actor[] = [];
        
        for (let enemy of this.SpawnedEnemies) {
            if (enemy && UE.KismetSystemLibrary.IsValid(enemy)) {
                validEnemies.push(enemy);
            }
        }
        
        this.CurrentEnemyCount = validEnemies.length;
        this.SpawnedEnemies = validEnemies;
    }
    
    /**
     * Actor销毁时清理
     */
    ReceiveEndPlay(EndPlayReason: number): void {
        this.StopSpawnTimer();

        // 销毁所有生成的怪物
        if (this.SpawnedEnemies) {
            for (let enemy of this.SpawnedEnemies) {
                if (enemy && UE.KismetSystemLibrary.IsValid(enemy)) {
                    enemy.K2_DestroyActor();
                }
            }
        }
    }
}

blueprint.mixin(jsCls, BP_EnemySpawnArea);