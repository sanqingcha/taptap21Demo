"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const UE = require("ue");
const puerts_1 = require("puerts");
let ucls = UE.Class.Load("/Game/Game/GameMap/Blueprint/Enemy/BT_Task/BTT_DiscreteJump.BTT_DiscreteJump_C");
let jsCls = puerts_1.blueprint.tojs(ucls);
/**
 * 离散跳跃任务
 * 让角色周期性地执行跳跃动作，通过施加力实现
 *
 * 使用场景：跳跃类敌人（如史莱姆、兔子等）的移动方式
 */
class BTT_DiscreteJump {
    // ========== 蓝图变量 ==========
    /** 跳跃最大高度（从黑板读取） */
    JumpMaxHeight;
    /** 跳跃滞空时间（秒） */
    JumpTime;
    /** 跳跃间隔（秒） */
    Interval;
    /** 间隔的随机偏差（秒） */
    IntervalRandomOffset;
    /** 是否显示调试信息 */
    bShowDebug;
    // ========== 内部状态 ==========
    /** 下次跳跃的时间戳 */
    NextJumpTime = 0;
    /** 当前控制的 Pawn */
    CachedPawn = undefined;
    /** AI 控制器 */
    CachedController = undefined;
    // ========== 生命周期方法 ==========
    /**
     * 任务开始执行
     */
    ReceiveExecuteAI(OwnerController, ControlledPawn) {
        if (!OwnerController || !ControlledPawn) {
            console.error("BTT_DiscreteJump: OwnerController 或 ControlledPawn 为空");
            this.FinishExecute(false);
            return;
        }
        // 检查是否是 Character（需要 LaunchCharacter 方法）
        const Character = ControlledPawn;
        if (!Character) {
            console.error("BTT_DiscreteJump: ControlledPawn 不是 Character 类型");
            this.FinishExecute(false);
            return;
        }
        // 缓存引用
        this.CachedPawn = Character;
        this.CachedController = OwnerController;
        // 初始化下次跳跃时间为立即跳跃
        this.NextJumpTime = 0;
        console.log("BTT_DiscreteJump: Task started");
    }
    /**
     * 任务持续执行（每帧调用）
     */
    ReceiveTickAI(OwnerController, ControlledPawn, DeltaSeconds) {
        if (!this.CachedPawn || !this.CachedController) {
            return;
        }
        const World = this.CachedPawn.GetWorld();
        if (!World) {
            return;
        }
        const CurrentTime = UE.KismetSystemLibrary.GetGameTimeInSeconds(World);
        // 检查是否到达下次跳跃时间
        if (CurrentTime >= this.NextJumpTime) {
            this.PerformJump();
            this.ScheduleNextJump(CurrentTime);
        }
    }
    /**
     * 任务中止
     */
    ReceiveAbortAI(OwnerController, ControlledPawn) {
        console.log("BTT_DiscreteJump: Task aborted");
        this.Cleanup();
    }
    // ========== 跳跃逻辑 ==========
    /**
     * 执行一次跳跃
     */
    PerformJump() {
        if (!this.CachedPawn || !this.CachedController) {
            return;
        }
        // 检查是否在地面上（只有在地面上才能跳跃）
        const CharacterMovement = this.CachedPawn.CharacterMovement;
        if (!CharacterMovement) {
            console.warn("BTT_DiscreteJump: CharacterMovement 组件未找到");
            return;
        }
        // 如果已经在空中，跳过本次跳跃
        if (CharacterMovement.IsFalling()) {
            if (this.bShowDebug) {
                console.log("BTT_DiscreteJump: 角色在空中，跳过本次跳跃");
            }
            return;
        }
        // 从黑板获取跳跃高度
        const BlackboardComp = this.CachedController.Blackboard;
        let JumpHeight = 500.0; // 默认高度
        if (BlackboardComp && this.JumpMaxHeight) {
            JumpHeight = BlackboardComp.GetValueAsFloat(this.JumpMaxHeight.SelectedKeyName);
            if (JumpHeight <= 0) {
                JumpHeight = 500.0;
            }
        }
        // 计算跳跃所需的初速度
        // 使用公式: v₀ = sqrt(2 * g * h)
        // UE 的重力默认是 -980 cm/s²
        const Gravity = Math.abs(CharacterMovement.GetGravityZ());
        const JumpVelocity = Math.sqrt(2 * Gravity * JumpHeight);
        // 施加向上的速度
        const LaunchVelocity = new UE.Vector(0, 0, JumpVelocity);
        this.CachedPawn.LaunchCharacter(LaunchVelocity, false, true); // 不覆盖 XY，覆盖 Z
        if (this.bShowDebug) {
            console.log(`BTT_DiscreteJump: 执行跳跃 - 高度=${JumpHeight.toFixed(0)}, 初速度=${JumpVelocity.toFixed(0)}`);
            // 绘制调试信息
            const World = this.CachedPawn.GetWorld();
            if (World) {
                const StartLocation = this.CachedPawn.K2_GetActorLocation();
                const EndLocation = new UE.Vector(StartLocation.X, StartLocation.Y, StartLocation.Z + JumpHeight);
                // 绘制跳跃轨迹（绿色箭头）
                UE.KismetSystemLibrary.DrawDebugArrow(World, StartLocation, EndLocation, 50.0, new UE.LinearColor(0, 1, 0, 1), 1.0, 5.0);
            }
        }
    }
    /**
     * 计划下次跳跃时间
     */
    ScheduleNextJump(CurrentTime) {
        const BaseInterval = this.Interval || 2.0;
        const RandomOffset = this.IntervalRandomOffset || 0.5;
        // 计算随机间隔
        const RandomInterval = BaseInterval + UE.KismetMathLibrary.RandomFloatInRange(-RandomOffset, RandomOffset);
        // 确保间隔不会小于 0.1 秒
        const ClampedInterval = Math.max(0.1, RandomInterval);
        this.NextJumpTime = CurrentTime + ClampedInterval;
        if (this.bShowDebug) {
            console.log(`BTT_DiscreteJump: 下次跳跃时间: ${ClampedInterval.toFixed(2)}秒后`);
        }
    }
    /**
     * 清理资源
     */
    Cleanup() {
        this.CachedPawn = undefined;
        this.CachedController = undefined;
        this.NextJumpTime = 0;
    }
}
puerts_1.blueprint.mixin(jsCls, BTT_DiscreteJump);
//# sourceMappingURL=BTT_DiscreteJump.js.map