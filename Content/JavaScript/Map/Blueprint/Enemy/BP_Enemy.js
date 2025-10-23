"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const UE = require("ue");
const puerts_1 = require("puerts");
let ucls = UE.Class.Load("/Game/Game/GameMap/Blueprint/Enemy/BP_Enemy.BP_Enemy_C");
let jsCls = puerts_1.blueprint.tojs(ucls);
/**
 * 敌人基类
 * 从数据表读取配置并初始化黑板值
 *
 */
class BP_Enemy {
    /**
     * 从数据表结构体批量初始化黑板值
     * 自动遍历结构体的所有字段，将字段名作为黑板键名，字段值作为黑板值
     *
     * @param BlackBoard 黑板组件
     * @param DataTabel S_Enemy 结构体数据
     */
    InitBlackBoardValue(BlackBoard, DataTabel) {
        console.log("BP_Enemy: Initializing blackboard values from data table");
        if (!BlackBoard) {
            console.error("BP_Enemy: BlackBoard is null, cannot initialize values");
            return;
        }
        if (!DataTabel) {
            console.error("BP_Enemy: DataTabel is null, cannot initialize values");
            return;
        }
        console.log("BP_Enemy: Starting to initialize blackboard values from data table");
        // 遍历 DataTabel 的所有属性
        for (const key in DataTabel) {
            // 跳过原型链上的属性和非数据属性
            if (!DataTabel.hasOwnProperty(key)) {
                continue;
            }
            const value = DataTabel[key];
            if (typeof value === 'number') {
                BlackBoard.SetValueAsFloat(key, value);
                console.log(`BP_Enemy: Setting blackboard value "${key}" to ${value}`);
            }
            else if (typeof value === 'string') {
                BlackBoard.SetValueAsString(key, value);
            }
            else if (typeof value === 'boolean') {
                BlackBoard.SetValueAsBool(key, value);
            }
            else if (value instanceof UE.Vector) {
                BlackBoard.SetValueAsVector(key, value);
            }
            else if (value instanceof UE.Rotator) {
                BlackBoard.SetValueAsRotator(key, value);
            }
            else if (value && typeof value === 'object' && value.StaticClass) {
                BlackBoard.SetValueAsObject(key, value);
            }
            else {
                console.warn(`BP_Enemy: Skipping key "${key}" - unsupported type: ${typeof value}`);
            }
        }
    }
}
puerts_1.blueprint.mixin(jsCls, BP_Enemy);
//# sourceMappingURL=BP_Enemy.js.map