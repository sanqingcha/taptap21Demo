"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const UE = require("ue");
const puerts_1 = require("puerts");
let ucls = UE.Class.Load("/Game/Game/GameMap/Blueprint/Enemy/BT_Task/BTT_FindPatrolLocation.BTT_FindPatrolLocation_C");
let jsCls = puerts_1.blueprint.tojs(ucls);
/**
 *
 *
 * @class 寻找巡逻点
 * @implements {BTT_FindPatrolLocation}
 */
class BTT_FindPatrolLocation {
    SearchRadius; // 搜索半径
    ReceiveExecuteAI(OwnerController, ControlledPawn) {
        if (!OwnerController || !ControlledPawn) {
            console.error("BTT_FindPatrolLocation: Invalid OwnerController or ControlledPawn");
            this.FinishExecute(false);
            return;
        }
        const BlackboardComp = OwnerController.Blackboard;
        if (!BlackboardComp) {
            console.error("BTT_FindPatrolLocation: Blackboard component not found");
            this.FinishExecute(false);
            return;
        }
        const World = ControlledPawn.GetWorld();
        if (!World) {
            console.error("BTT_FindPatrolLocation: World not found");
            this.FinishExecute(false);
            return;
        }
        const NavSys = UE.NavigationSystemV1.GetNavigationSystem(World);
        if (!NavSys) {
            console.error("BTT_FindPatrolLocation: Navigation system not found");
            this.FinishExecute(false);
            return;
        }
        const SearchRadius = this.SearchRadius || 1500.0;
        let CurrentLocation = ControlledPawn.K2_GetActorLocation();
        //将当前位置投影到NavMesh
        const ProjectedLocation = (0, puerts_1.$ref)(undefined);
        const QueryExtent = new UE.Vector(500, 500, 500); // 投影搜索范围
        const bProjected = UE.NavigationSystemV1.K2_ProjectPointToNavigation(World, CurrentLocation, ProjectedLocation, undefined, undefined, QueryExtent);
        if (bProjected && ProjectedLocation) {
            CurrentLocation = (0, puerts_1.$unref)(ProjectedLocation);
            console.log(`位置已投影到NavMesh: X=${CurrentLocation.X.toFixed(1)}, Y=${CurrentLocation.Y.toFixed(1)}, Z=${CurrentLocation.Z.toFixed(1)}`);
        }
        else {
            console.warn("BTT_FindPatrolLocation: 当前位置无法投影到NavMesh，尝试使用上次有效位置");
            // 尝试使用上次有效的巡逻位置
            const LastValidLocation = BlackboardComp.GetValueAsVector("LastValidLocation");
            if (LastValidLocation) {
                CurrentLocation = LastValidLocation;
                console.log(`使用上次有效位置: X=${CurrentLocation.X.toFixed(1)}, Y=${CurrentLocation.Y.toFixed(1)}, Z=${CurrentLocation.Z.toFixed(1)}`);
            }
            else {
                console.error("BTT_FindPatrolLocation: 没有可用的NavMesh位置，任务失败");
                this.FinishExecute(false);
                return;
            }
        }
        const SearchRadii = [SearchRadius, SearchRadius * 2, SearchRadius * 3];
        let bFoundLocation = false;
        let PatrolLocation;
        for (const Radius of SearchRadii) {
            const RandomPoint = (0, puerts_1.$ref)(undefined);
            const bSuccess = UE.NavigationSystemV1.K2_GetRandomReachablePointInRadius(World, CurrentLocation, RandomPoint, Radius);
            if (bSuccess && RandomPoint) {
                PatrolLocation = (0, puerts_1.$unref)(RandomPoint);
                bFoundLocation = true;
                console.log(`找到巡逻点 (半径=${Radius.toFixed(0)}): X=${PatrolLocation.X.toFixed(1)}, Y=${PatrolLocation.Y.toFixed(1)}, Z=${PatrolLocation.Z.toFixed(1)}`);
                break;
            }
            else if (Radius < SearchRadii[SearchRadii.length - 1]) {
                console.warn(`半径 ${Radius.toFixed(0)} 内未找到巡逻点，尝试更大半径`);
            }
        }
        if (bFoundLocation && PatrolLocation) {
            // 设置巡逻位置
            BlackboardComp.SetValueAsVector("PatrolLocation", PatrolLocation);
            // 保存为上次有效位置，供下次使用
            BlackboardComp.SetValueAsVector("LastValidLocation", PatrolLocation);
            this.FinishExecute(true);
        }
        else {
            console.error("BTT_FindPatrolLocation: 所有半径尝试均失败，无法找到巡逻点");
            this.FinishExecute(false);
        }
    }
}
puerts_1.blueprint.mixin(jsCls, BTT_FindPatrolLocation);
//# sourceMappingURL=BTT_FindPatrolLocation.js.map