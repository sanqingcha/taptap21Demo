import * as UE from 'ue';
import { $Nullable, blueprint } from 'puerts';

let ucls = UE.Class.Load("/Game/Game/GameMap/Blueprint/Enemy/BT_Service/BTS_DiscreteJump.BTS_DiscreteJump_C")
let jsCls = blueprint.tojs<typeof UE.Game.Game.GameMap.Blueprint.Enemy.BT_Service.BTS_DiscreteJump.BTS_DiscreteJump_C>(ucls);
interface BTS_DiscreteJump extends UE.Game.Game.GameMap.Blueprint.Enemy.BT_Service.BTS_DiscreteJump.BTS_DiscreteJump_C { }

class BTS_DiscreteJump implements BTS_DiscreteJump {
    // ========== 蓝图变量 ==========

    /** 跳跃最大高度 */
    JumpMaxHeight: UE.BlackboardKeySelector;

    /** 跳跃间隔（秒）- 从落地后开始计时 */
    JumpInterval: number;

    /** 间隔的随机偏差（秒） */
    IntervalRandomOffset: number;

    /** 跳跃特效 Niagara 系统 */
    JumpEffect: UE.NiagaraSystem;

    /** 特效持续时间（秒）*/
    EffectDuration: number;

    bShowDebug: boolean;

    // ========== 内部状态 ==========

    /** 是否正在等待落地 */
    bWaitingForLanding: UE.BlackboardKeySelector;

    /** 落地时间戳（用于计算下次跳跃） */
    LandingTime: UE.BlackboardKeySelector;

    /** 下次跳跃的时间戳 */
    NextJumpTime: UE.BlackboardKeySelector;

    /** 上一帧是否在空中 */
    bWasFallingLastFrame: UE.BlackboardKeySelector;

    /** 缓存的跳跃特效组件（复用，避免频繁创建销毁） */
    private CachedJumpEffectComp: UE.NiagaraComponent | null = null;

    /** 活跃的定时器 ID 列表（用于清理） */
    private ActiveTimers: number[] = [];

    private static ProcessedPawnIDs: Set<UE.Pawn> = new Set<UE.Pawn>();
    // ========== 生命周期方法 ==========

    /**
     * 服务激活时调用（节点开始执行）
     */
    ReceiveActivationAI(OwnerController: $Nullable<UE.AIController>, ControlledPawn: $Nullable<UE.Pawn>): void {
        if (!OwnerController || !ControlledPawn) {
            console.error("BTS_DiscreteJump: OwnerController 或 ControlledPawn 为空");
            return;
        }

        // 检查是否是 Character
        const Character = ControlledPawn as UE.Character;
        if (!Character) {
            console.error("BTS_DiscreteJump: ControlledPawn 不是 Character 类型");
            return;
        }

        const BlackboardComp = OwnerController.Blackboard;
        if (!BlackboardComp) {
            console.error("BTD_IsAtTarget: Blackboard 组件未找到");
            return;
        }


        BlackboardComp.SetValueAsBool("bWaitingForLanding", false);
        BlackboardComp.SetValueAsFloat("NextJumpTime", 0);
        BlackboardComp.SetValueAsFloat("LandingTime", 0);
        BlackboardComp.SetValueAsBool("bWasFallingLastFrame", false);

        // 显式初始化数组（PuerTS mixin 可能不会执行字段初始化器）
        if (!this.ActiveTimers) {
            this.ActiveTimers = [];
        }

        if (this.bShowDebug) {
            console.log("BTS_DiscreteJump: Service activated - 准备立即跳跃");
        }

        if(!BTS_DiscreteJump.ProcessedPawnIDs.has(ControlledPawn)) {
            BTS_DiscreteJump.ProcessedPawnIDs.add(ControlledPawn);
            if (!ControlledPawn)
                return;

            console.log(`add on land has controlledPawn`)
            let componentPath = "/Game/Game/GameMap/Blueprint/Enemy/BP_BugEggJumper/BT_OnJumperLandedComponent.BT_OnJumperLandedComponent_C"
            let componentClass = UE.Class.Load(componentPath);
            let comp = ControlledPawn.GetComponentByClass(componentClass);
            if (!comp)
                return;
            console.log(`add on land get comp successful`);
            this.OnLandedDispatcher.Add(() => {
                (comp as any).OnLanded?.();
            });
        }
    }

    /**
     * 每帧更新
     */
    ReceiveTickAI(OwnerController: $Nullable<UE.AIController>, ControlledPawn: $Nullable<UE.Pawn>, DeltaSeconds: number): void {
        if (!OwnerController || !ControlledPawn) {
            return;
        }

        const Character = ControlledPawn as UE.Character;
        if (!Character) {
            return;
        }

        const CharacterMovement = Character.CharacterMovement;
        if (!CharacterMovement) {
            return;
        }

        const World = Character.GetWorld();
        if (!World) {
            return;
        }

        const BlackboardComp = OwnerController.Blackboard;
        if (!BlackboardComp) {
            console.error("BTD_IsAtTarget: Blackboard 组件未找到");
            return;
        }

        const CurrentTime = UE.KismetSystemLibrary.GetGameTimeInSeconds(World);
        const bIsFalling = CharacterMovement.IsFalling();

        // 检测落地事件：上一帧在空中，这一帧在地面
        if (BlackboardComp.GetValueAsBool("bWasFallingLastFrame") && !bIsFalling) {
            this.OnLanded(CurrentTime,BlackboardComp);
        }

        // 更新上一帧状态
        BlackboardComp.SetValueAsBool("bWasFallingLastFrame", bIsFalling);

        // 如果正在等待落地，不执行跳跃
        if (this.bWaitingForLanding) {
            if (this.bShowDebug) {
                console.log(`BTS_DiscreteJump: 等待落地... (在空中: ${bIsFalling})`);
            }
            return;
        }

        // 检查是否到达下次跳跃时间且在地面上
        if (CurrentTime >= BlackboardComp.GetValueAsFloat("NextJumpTime") && !bIsFalling) {
            this.PerformJump(Character, OwnerController, CurrentTime);
        } else if (this.bShowDebug) {
            const TimeLeft = BlackboardComp.GetValueAsFloat("NextJumpTime") - CurrentTime;
            if (TimeLeft > 0) {
                console.log(`BTS_DiscreteJump: 等待跳跃 (还需 ${TimeLeft.toFixed(2)}s, 在地面: ${!bIsFalling})`);
            }
        }
    }

    /**
     * 服务停用时调用
     */
    ReceiveDeactivationAI(OwnerController: $Nullable<UE.AIController>, ControlledPawn: $Nullable<UE.Pawn>): void {
        const BlackboardComp = OwnerController.Blackboard;
        if (!BlackboardComp) {
            console.error("BTD_IsAtTarget: Blackboard 组件未找到");
            return;
        }
        BlackboardComp.SetValueAsBool("bWaitingForLanding", false);
        BlackboardComp.SetValueAsFloat("NextJumpTime", 0);
        BlackboardComp.SetValueAsFloat("LandingTime", 0);
        BlackboardComp.SetValueAsBool("bWasFallingLastFrame", false);

        // 清除所有活跃的定时器（安全检查）
        if (this.ActiveTimers && this.ActiveTimers.length > 0) {
            for (const timerId of this.ActiveTimers) {
                clearTimeout(timerId);
            }
            this.ActiveTimers = [];

            if (this.bShowDebug) {
                console.log("BTS_DiscreteJump: 已清除所有活跃定时器");
            }
        }

        // 清理缓存的特效组件
        if (this.CachedJumpEffectComp) {
            try {
                // 检查组件是否有效再销毁
                if (UE.KismetSystemLibrary.IsValid(this.CachedJumpEffectComp)) {
                    this.CachedJumpEffectComp.K2_DestroyComponent(this.CachedJumpEffectComp);
                }
            } catch (e) {
                // 组件已经被销毁，忽略错误
                if (this.bShowDebug) {
                    console.log("BTS_DiscreteJump: 特效组件已被销毁");
                }
            } finally {
                this.CachedJumpEffectComp = null;
            }

            if (this.bShowDebug) {
                console.log("BTS_DiscreteJump: Service deactivated - 已清理跳跃特效组件");
            }
        } else if (this.bShowDebug) {
            console.log("BTS_DiscreteJump: Service deactivated");
        }
    }


    private PerformJump(Character: UE.Character, Controller: UE.AIController, CurrentTime: number): void {
        const CharacterMovement = Character.CharacterMovement;
        if (!CharacterMovement) {
            return;
        }

        // 从黑板获取跳跃高度
        const BlackboardComp = Controller.Blackboard;
        let JumpHeight = 500.0; // 默认高度

        if (BlackboardComp && this.JumpMaxHeight) {
            JumpHeight = BlackboardComp.GetValueAsFloat(this.JumpMaxHeight.SelectedKeyName);
            if (JumpHeight <= 0) {
                JumpHeight = 500.0;
            }
        }

        // 计算跳跃所需的初速度
        // 使用公式: v₀ = sqrt(2 * g * h)
        const Gravity = Math.abs(CharacterMovement.GetGravityZ());
        const JumpVelocity = Math.sqrt(2 * Gravity * JumpHeight);

        // 施加向上的速度
        const LaunchVelocity = new UE.Vector(0, 0, JumpVelocity);
        Character.LaunchCharacter(LaunchVelocity, false, true);

        // 设置为等待落地状态
        BlackboardComp.SetValueAsBool("bWaitingForLanding", true);
        BlackboardComp.SetValueAsBool("bWasFallingLastFrame", true);
        

        // ========== 生成/复用跳跃特效 ==========
        if (this.JumpEffect) {
            const World = Character.GetWorld();
            if (World) {
                // 获取角色的 Mesh 组件
                const MeshComp = Character.Mesh;
                if (!MeshComp) {
                    console.warn("BTS_DiscreteJump: Character has no Mesh component");
                    return;
                }

                // 检查是否已有缓存的特效组件
                if (!this.CachedJumpEffectComp) {
                    // 第一次跳跃：创建并附加 Niagara 组件
                    this.CachedJumpEffectComp = UE.NiagaraFunctionLibrary.SpawnSystemAttached(
                        this.JumpEffect,
                        MeshComp,                         // 附加到角色 Mesh
                        "None",                           // Socket 名称（None 表示附加到根）
                        new UE.Vector(0, 0, 0),          // 相对位置
                        new UE.Rotator(0, 0, 0),         // 相对旋转
                        UE.EAttachLocation.KeepRelativeOffset,  // 保持相对偏移
                        false                             // Auto Destroy = false（手动控制）
                    );

                    if (this.bShowDebug) {
                        console.log("BTS_DiscreteJump: 创建跳跃特效组件（将被复用）");
                    }
                }

                // 激活特效（无论是新创建还是复用）
                if (this.CachedJumpEffectComp) {
                    // 激活 Niagara 组件
                    this.CachedJumpEffectComp.Activate(true); // true = 重置

                    // 设置特效持续时间（默认 1 秒）
                    const Duration = this.EffectDuration > 0 ? this.EffectDuration : 0.7;

                    // 使用 setTimeout 在 1 秒后停用特效（不销毁，下次复用）
                    const EffectComp = this.CachedJumpEffectComp;
                    const Self = this;
                    const timerId = setTimeout(() => {
                        try {
                            // 检查组件是否仍然有效
                            if (EffectComp && UE.KismetSystemLibrary.IsValid(EffectComp)) {
                                EffectComp.Deactivate(); // 停用但不销毁

                                if (Self.bShowDebug) {
                                    console.log("BTS_DiscreteJump: 跳跃特效已停用（组件保留用于复用）");
                                }
                            }
                        } catch (e) {
                            // 组件已被销毁，忽略错误
                            if (Self.bShowDebug) {
                                console.log("BTS_DiscreteJump: 特效组件已失效，跳过停用操作");
                            }
                        } finally {
                            // 从活跃定时器列表中移除（安全检查）
                            if (Self.ActiveTimers) {
                                const index = Self.ActiveTimers.indexOf(timerId);
                                if (index > -1) {
                                    Self.ActiveTimers.splice(index, 1);
                                }
                            }
                        }
                    }, Duration * 1000); // setTimeout 使用毫秒

                    // 保存定时器 ID（安全检查）
                    if (!this.ActiveTimers) {
                        this.ActiveTimers = [];
                    }
                    this.ActiveTimers.push(timerId);

                    if (this.bShowDebug) {
                        console.log(`BTS_DiscreteJump: 激活跳跃特效（复用模式），持续时间 ${Duration.toFixed(2)}s`);
                    }
                } else {
                    console.warn("BTS_DiscreteJump: Failed to create jump effect component");
                }
            }
        }

        if (this.bShowDebug) {
            console.log(`BTS_DiscreteJump: [${CurrentTime.toFixed(2)}s] 执行跳跃 - 高度=${JumpHeight.toFixed(0)}, 初速度=${JumpVelocity.toFixed(0)}`);

            // 绘制调试信息
            const World = Character.GetWorld();
            if (World) {
                const StartLocation = Character.K2_GetActorLocation();
                const EndLocation = new UE.Vector(StartLocation.X, StartLocation.Y, StartLocation.Z + JumpHeight);

                // 绘制跳跃轨迹（绿色箭头）
                UE.KismetSystemLibrary.DrawDebugArrow(
                    World,
                    StartLocation,
                    EndLocation,
                    50.0,
                    new UE.LinearColor(0, 1, 0, 1),
                    2.0,
                    5.0
                );

                // 绘制目标高度标记（黄色球体）
                UE.KismetSystemLibrary.DrawDebugSphere(
                    World,
                    EndLocation,
                    30.0,
                    12,
                    new UE.LinearColor(1, 1, 0, 1),
                    2.0,
                    2.0
                );
            }
        }
    }

    /**
     * 落地事件处理
     */
    private OnLanded(CurrentTime: number, BlackboardComp: UE.BlackboardComponent): void {
        BlackboardComp.SetValueAsBool("bWaitingForLanding", false);
        BlackboardComp.SetValueAsFloat("LandingTime", CurrentTime);

        // 计算下次跳跃时间 = 落地时间 + 间隔
        const BaseInterval = this.JumpInterval;
        const RandomOffset = this.IntervalRandomOffset;
        const RandomInterval = BaseInterval + UE.KismetMathLibrary.RandomFloatInRange(-RandomOffset, RandomOffset);
        const ClampedInterval = Math.max(0.0, RandomInterval); // 允许 0 秒间隔（连续跳跃）

        BlackboardComp.SetValueAsFloat("NextJumpTime", CurrentTime + ClampedInterval);

        this.OnLandedDispatcher.Broadcast();
    }
}

blueprint.mixin(jsCls, BTS_DiscreteJump);
