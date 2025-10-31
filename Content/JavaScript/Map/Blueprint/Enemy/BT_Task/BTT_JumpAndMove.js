"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const UE = require("ue");
const puerts_1 = require("puerts");
let ucls = UE.Class.Load("/Game/Game/GameMap/Blueprint/Enemy/BT_Task/BTT_JumpAndMove.BTT_JumpAndMove_C");
let jsCls = puerts_1.blueprint.tojs(ucls);
class BTT_JumpAndMove {
    // ... (所有蓝图变量和内部状态保持不变, 移除 bRotateToTargetInAir) ...
    // ▼▼▼ 蓝图变量 ▼▼▼
    TargetActor;
    JumpMaxHeight;
    DownwardForceMultiplier;
    AirMoveSpeed; // 【重要】这个值现在是“空中控制力”的倍数 (推荐 1.0 到 5.0)
    JumpEffect;
    EffectDuration;
    bShowDebug;
    // ▼▼▼ 内部状态 ▼▼▼
    CachedCharacter = null;
    CachedController = null;
    CachedTargetLocation = null;
    bReachedApex = false;
    PreviousVerticalVelocity = 0;
    bWasFallingLastFrame = false;
    JumpEffectComponent = null;
    EffectTimerId = -1;
    // -----------------------------------------------------------------
    // ▼▼▼【重要】: 我们需要缓存CMC的原始空中控制力 ▼▼▼
    // -----------------------------------------------------------------
    OriginalAirControl = 0.0;
    ReceiveExecuteAI(OwnerController, ControlledPawn) {
        if (!OwnerController || !ControlledPawn) {
            console.error("BTT_JumpAndMove: OwnerController 或 ControlledPawn 为空");
            this.FinishExecute(false);
            return;
        }
        const Character = ControlledPawn;
        if (!Character || !Character.CharacterMovement) {
            console.error("BTT_JumpAndMove: ControlledPawn 不是 Character 类型或缺少 CharacterMovement");
            this.FinishExecute(false);
            return;
        }
        this.CachedCharacter = Character;
        this.CachedController = OwnerController;
        // -----------------------------------------------------------------
        // ▼▼▼【重要】: 在任务开始时，增强空中控制力 ▼▼▼
        // -----------------------------------------------------------------
        const CharacterMovement = this.CachedCharacter.CharacterMovement;
        this.OriginalAirControl = CharacterMovement.AirControl;
        // AirMoveSpeed 现在作为“空中控制力”的倍数 (例如 1.0 或 2.0)，而不是一个速度值
        CharacterMovement.AirControl = this.AirMoveSpeed > 0 ? this.AirMoveSpeed : 1.0;
        // ... (省略 Blackboard 和 JumpHeight 的检查代码, 它们是正确的) ...
        const BlackboardComp = OwnerController.Blackboard;
        if (!BlackboardComp) { /* ... */
            this.FinishExecute(false);
            return;
        }
        this.CachedTargetLocation = BlackboardComp.GetValueAsVector(this.TargetActor.SelectedKeyName);
        if (!this.CachedTargetLocation || this.CachedTargetLocation.IsZero()) { /* ... */
            this.FinishExecute(false);
            return;
        }
        let JumpHeight = 500.0;
        if (this.JumpMaxHeight) { /* ... */ }
        // ... (省略状态初始化和 PerformJump 调用, 它们是正确的) ...
        this.bReachedApex = false;
        this.bWasFallingLastFrame = false;
        this.PerformJump(Character, JumpHeight);
        if (this.bShowDebug) { /* ... */ }
    }
    ReceiveTickAI(OwnerController, ControlledPawn, DeltaSeconds) {
        // ... (省略了开头的检查代码, 它们是正确的) ...
        if (!this.CachedCharacter || !this.CachedController || !this.CachedCharacter.CharacterMovement)
            return;
        const CharacterMovement = this.CachedCharacter.CharacterMovement;
        if (!CharacterMovement)
            return;
        const World = this.CachedCharacter.GetWorld();
        if (!World)
            return;
        const bIsFalling = CharacterMovement.IsFalling();
        if (bIsFalling) {
            // 1. 顶点检测和施加向下力
            // (现在速度不再抖动, 这个检测会变得非常可靠)
            this.HandleApexDownwardForce(CharacterMovement);
            // 2. 空中向目标移动 (使用 AddMovementInput)
            this.MoveTowardTargetInAir(CharacterMovement, DeltaSeconds);
        }
        // 检测落地
        if (this.bWasFallingLastFrame && !bIsFalling) {
            this.OnLanded();
        }
        this.bWasFallingLastFrame = bIsFalling;
    }
    /**
     * 在空中向目标移动 (已修复抖动问题)
     */
    MoveTowardTargetInAir(CharacterMovement, DeltaSeconds) {
        if (!this.CachedCharacter || !this.CachedTargetLocation) {
            return;
        }
        const CurrentLocation = this.CachedCharacter.K2_GetActorLocation();
        const TargetLocation = this.CachedTargetLocation;
        const DirectionToTarget = new UE.Vector(TargetLocation.X - CurrentLocation.X, TargetLocation.Y - CurrentLocation.Y, 0 // 只考虑水平移动
        );
        const Distance = Math.sqrt(DirectionToTarget.X * DirectionToTarget.X + DirectionToTarget.Y * DirectionToTarget.Y);
        if (Distance < 10.0) {
            return;
        }
        const NormalizedDirection = new UE.Vector(DirectionToTarget.X / Distance, DirectionToTarget.Y / Distance, 0);
        // -----------------------------------------------------------------
        // ▼▼▼【抖动修复】: 使用 AddMovementInput 代替 LaunchCharacter ▼▼▼
        // -----------------------------------------------------------------
        // 1.0 的 ScaleValue 意味着使用 CharacterMovement.AirControl 的全部力量
        this.CachedCharacter.AddMovementInput(NormalizedDirection, 1.0, false);
        if (this.bShowDebug) {
            // ... (调试绘制逻辑不变) ...
        }
    }
    /**
     * 清理资源
     */
    CleanUp() {
        // ... (清除特效和定时器) ...
        // -----------------------------------------------------------------
        // ▼▼▼【重要】: 恢复CMC的原始空中控制力 ▼▼▼
        // -----------------------------------------------------------------
        if (this.CachedCharacter && this.CachedCharacter.CharacterMovement) {
            this.CachedCharacter.CharacterMovement.AirControl = this.OriginalAirControl;
        }
        // ... (清空缓存) ...
        this.CachedCharacter = null;
        this.CachedController = null;
        this.CachedTargetLocation = null;
        this.bReachedApex = false;
        this.PreviousVerticalVelocity = 0;
        this.bWasFallingLastFrame = false;
    }
    // ... (PerformJump, HandleApexDownwardForce, SpawnJumpEffect, OnLanded 保持不变) ...
    /* (为保持完整性，以下函数保持不变) */
    PerformJump(Character, JumpHeight) {
        const CharacterMovement = Character.CharacterMovement;
        if (!CharacterMovement) {
            return;
        }
        const Gravity = Math.abs(CharacterMovement.GetGravityZ());
        const JumpVelocity = Math.sqrt(2 * Gravity * JumpHeight);
        const LaunchVelocity = new UE.Vector(0, 0, JumpVelocity);
        Character.LaunchCharacter(LaunchVelocity, false, true); // (这个是合法的, 因为它是一次性冲量)
        this.PreviousVerticalVelocity = JumpVelocity;
        this.bWasFallingLastFrame = true;
        this.SpawnJumpEffect(Character);
        // ... (调试绘制) ...
    }
    HandleApexDownwardForce(CharacterMovement) {
        if (this.bReachedApex || !this.CachedCharacter) {
            return;
        }
        const CurrentVerticalVelocity = this.CachedCharacter.GetVelocity().Z;
        if (this.PreviousVerticalVelocity > 50.0 && CurrentVerticalVelocity <= 50.0) {
            this.bReachedApex = true;
            const Gravity = Math.abs(CharacterMovement.GetGravityZ());
            const Multiplier = this.DownwardForceMultiplier > 0 ? this.DownwardForceMultiplier : 1.5;
            const DownwardForceMagnitude = Gravity * Multiplier;
            const DownwardVelocity = new UE.Vector(0, 0, -DownwardForceMagnitude);
            this.CachedCharacter.LaunchCharacter(DownwardVelocity, false, true); // (这个也是合法的, 一次性冲量)
            // ... (调试绘制) ...
        }
        this.PreviousVerticalVelocity = CurrentVerticalVelocity;
    }
    SpawnJumpEffect(Character) { }
    OnLanded() {
        if (this.bShowDebug) {
            console.log("BTT_JumpAndMove: 落地！任务完成");
        }
        this.CleanUp();
        this.FinishExecute(true);
    }
}
puerts_1.blueprint.mixin(jsCls, BTT_JumpAndMove);
//# sourceMappingURL=BTT_JumpAndMove.js.map