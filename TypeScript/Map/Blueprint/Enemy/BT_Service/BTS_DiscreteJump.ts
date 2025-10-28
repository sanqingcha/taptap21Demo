import * as UE from 'ue';
import { $Nullable, blueprint } from 'puerts';

let ucls = UE.Class.Load("/Game/Game/GameMap/Blueprint/Enemy/BT_Service/BTS_DiscreteJump.BTS_DiscreteJump_C")
let jsCls = blueprint.tojs<typeof UE.Game.Game.GameMap.Blueprint.Enemy.BT_Service.BTS_DiscreteJump.BTS_DiscreteJump_C>(ucls);
interface BTS_DiscreteJump extends UE.Game.Game.GameMap.Blueprint.Enemy.BT_Service.BTS_DiscreteJump.BTS_DiscreteJump_C { }

/**
 * 离散跳跃服务
 * 让角色在执行行为树节点时周期性地跳跃
 *
 * 工作流程：
 * 1. 立即跳跃
 * 2. 等待落地
 * 3. 落地后等待 JumpInterval 时间
 * 4. 重复步骤 1
 */
class BTS_DiscreteJump implements BTS_DiscreteJump {
    // ========== 蓝图变量 ==========

    /** 跳跃最大高度（从黑板读取） */
    JumpMaxHeight: UE.BlackboardKeySelector;

    /** 跳跃间隔（秒）- 从落地后开始计时 */
    JumpInterval: number;

    /** 间隔的随机偏差（秒） */
    IntervalRandomOffset: number;

    /** 是否显示调试信息 */
    bShowDebug: boolean;

    // ========== 内部状态 ==========

    /** 是否正在等待落地 */
    private bWaitingForLanding: boolean = false;

    /** 落地时间戳（用于计算下次跳跃） */
    private LandingTime: number = 0;

    /** 下次跳跃的时间戳 */
    private NextJumpTime: number = 0;

    /** 上一帧是否在空中 */
    private bWasFallingLastFrame: boolean = false;

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

        // 初始化状态：立即跳跃
        this.bWaitingForLanding = false;
        this.NextJumpTime = 0; // 0 表示立即跳跃
        this.LandingTime = 0;
        this.bWasFallingLastFrame = false;

        if (this.bShowDebug) {
            console.log("BTS_DiscreteJump: Service activated - 准备立即跳跃");
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

        const CurrentTime = UE.KismetSystemLibrary.GetGameTimeInSeconds(World);
        const bIsFalling = CharacterMovement.IsFalling();

        // 检测落地事件：上一帧在空中，这一帧在地面
        if (this.bWasFallingLastFrame && !bIsFalling) {
            this.OnLanded(CurrentTime);
        }

        // 更新上一帧状态
        this.bWasFallingLastFrame = bIsFalling;

        // 如果正在等待落地，不执行跳跃
        if (this.bWaitingForLanding) {
            if (this.bShowDebug) {
                console.log(`BTS_DiscreteJump: 等待落地... (在空中: ${bIsFalling})`);
            }
            return;
        }

        // 检查是否到达下次跳跃时间且在地面上
        if (CurrentTime >= this.NextJumpTime && !bIsFalling) {
            this.PerformJump(Character, OwnerController, CurrentTime);
        } else if (this.bShowDebug) {
            const TimeLeft = this.NextJumpTime - CurrentTime;
            if (TimeLeft > 0) {
                console.log(`BTS_DiscreteJump: 等待跳跃 (还需 ${TimeLeft.toFixed(2)}s, 在地面: ${!bIsFalling})`);
            }
        }
    }

    /**
     * 服务停用时调用
     */
    ReceiveDeactivationAI(OwnerController: $Nullable<UE.AIController>, ControlledPawn: $Nullable<UE.Pawn>): void {
        this.bWaitingForLanding = false;
        this.NextJumpTime = 0;
        this.LandingTime = 0;
        this.bWasFallingLastFrame = false;

        if (this.bShowDebug) {
            console.log("BTS_DiscreteJump: Service deactivated");
        }
    }

    // ========== 跳跃逻辑 ==========

    /**
     * 执行一次跳跃
     */
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
        this.bWaitingForLanding = true;
        this.bWasFallingLastFrame = true; // 跳跃后立即在空中

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
    private OnLanded(CurrentTime: number): void {
        this.bWaitingForLanding = false;
        this.LandingTime = CurrentTime;

        // 计算下次跳跃时间 = 落地时间 + 间隔
        const BaseInterval = this.JumpInterval;
        const RandomOffset = this.IntervalRandomOffset;
        const RandomInterval = BaseInterval + UE.KismetMathLibrary.RandomFloatInRange(-RandomOffset, RandomOffset);
        const ClampedInterval = Math.max(0.0, RandomInterval); // 允许 0 秒间隔（连续跳跃）

        this.NextJumpTime = CurrentTime + ClampedInterval;

        if (this.bShowDebug) {
            console.log(`BTS_DiscreteJump: [${CurrentTime.toFixed(2)}s] 落地！下次跳跃时间: ${ClampedInterval.toFixed(2)}s 后 (${this.NextJumpTime.toFixed(2)}s)`);
        }
    }
}

blueprint.mixin(jsCls, BTS_DiscreteJump);
