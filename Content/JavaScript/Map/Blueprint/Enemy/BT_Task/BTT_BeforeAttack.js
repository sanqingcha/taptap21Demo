"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const UE = require("ue");
const puerts_1 = require("puerts");
let ucls = UE.Class.Load("/Game/Game/GameMap/Blueprint/Enemy/BT_Task/BTT_BeforeAttack.BTT_BeforeAttack_C");
let jsCls = puerts_1.blueprint.tojs(ucls);
class BTT_BeforeAttack {
    // 私有字段：保存定时器和初始值
    ActiveTimers = [];
    InitialEmissionValue = 1.0;
    DynamicMaterial = null;
    ReceiveExecuteAI(OwnerController, ControlledPawn) {
        if (!OwnerController || !ControlledPawn) {
            console.error("BTT_BeforeAttack: Invalid OwnerController or ControlledPawn");
            this.FinishExecute(false);
            return;
        }
        // 确保字段已初始化（PuerTS mixin 问题）
        if (!this.ActiveTimers) {
            this.ActiveTimers = [];
        }
        const Character = ControlledPawn;
        if (!Character) {
            console.error("BTT_BeforeAttack: ControlledPawn is not a Character");
            this.FinishExecute(false);
            return;
        }
        const MeshComp = Character.Mesh;
        if (!MeshComp) {
            console.error("BTT_BeforeAttack: Mesh component not found");
            this.FinishExecute(false);
            return;
        }
        // 获取或创建动态材质实例
        this.DynamicMaterial = MeshComp.CreateDynamicMaterialInstance(0, null);
        if (!this.DynamicMaterial) {
            console.error("BTT_BeforeAttack: Failed to create dynamic material instance");
            this.FinishExecute(false);
            return;
        }
        // 关键：将动态材质实例设置回 Mesh（否则 Mesh 仍使用原始材质）
        MeshComp.SetMaterial(0, this.DynamicMaterial);
        // 获取初始 EmissionStrength 值
        this.InitialEmissionValue = this.DynamicMaterial.K2_GetScalarParameterValue("EmissionStrength");
        // 开始渐变效果
        this.StartEmissionFade(this.DynamicMaterial, this.InitialEmissionValue);
    }
    StartEmissionFade(Material, InitialValue) {
        const TargetValue = 300.0;
        const Duration = 1000; // 1秒
        const UpdateInterval = 50; // 每50ms更新一次
        const TotalSteps = Duration / UpdateInterval; // 20步
        const DeltaValue = (TargetValue - InitialValue) / TotalSteps; // 每步增量
        let CurrentStep = 0;
        // 创建定时器进行渐变
        const FadeInterval = setInterval(() => {
            CurrentStep++;
            const NewValue = InitialValue + (DeltaValue * CurrentStep);
            // 更新材质参数
            Material.SetScalarParameterValue("EmissionStrength", NewValue);
            // 到达目标值
            if (CurrentStep >= TotalSteps) {
                clearInterval(FadeInterval);
                console.log("BTT_BeforeAttack: Fade complete, holding at target value");
                // 立即重置并完成任务
                this.ResetEmissionAndFinish(Material, InitialValue);
            }
        }, UpdateInterval);
        // 保存定时器ID以便清理
        this.ActiveTimers.push(FadeInterval);
    }
    ResetEmissionAndFinish(Material, InitialValue) {
        // 重置材质参数
        Material.SetScalarParameterValue("EmissionStrength", InitialValue);
        // 清理所有定时器
        if (this.ActiveTimers && this.ActiveTimers.length > 0) {
            for (const timerId of this.ActiveTimers) {
                clearInterval(timerId);
            }
            this.ActiveTimers = [];
        }
        // 完成任务
        this.FinishExecute(true);
    }
    // 任务被中断时的清理
    ReceiveAbortAI(OwnerController, ControlledPawn) {
        // 清理定时器
        if (this.ActiveTimers && this.ActiveTimers.length > 0) {
            for (const timerId of this.ActiveTimers) {
                clearInterval(timerId);
            }
            this.ActiveTimers = [];
        }
        // 如果有动态材质，恢复初始值
        if (this.DynamicMaterial) {
            this.DynamicMaterial.SetScalarParameterValue("EmissionStrength", this.InitialEmissionValue);
            console.log("BTT_BeforeAttack: Emission reset on abort");
        }
    }
}
puerts_1.blueprint.mixin(jsCls, BTT_BeforeAttack);
//# sourceMappingURL=BTT_BeforeAttack.js.map