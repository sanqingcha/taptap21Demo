# TapTap21_GameDemo
TapTap21天GameJam

##　使用插件 ：Puerts

### 文件夹框架注意：
## Content文件夹结构说明： 
# 结构预览：
    ├─Asset                                             ├─项目内部资产文件夹，用于存放所有资产    
    │  ├─Mesh                                           │  ├─存放网格体资产，包括静态网格资产，骨骼网格物理资产以及角色动画资产
    │  │  └─Player                                      │  │  └─网格资产子集，文件夹命名为存放的资产名字
    │  │      └─Anim                                    │  │      └─如果上一级是骨骼网格体资产，有动画的话就扩展一级作为动画资产文件夹
    │  ├─Niagara                                        │  ├─存放粒子资产的位置，包括Niagara以及p粒子等，(暂时不用创建子集，如果有需要归类的可以创建一个子集来存放)
    │  ├─Sound                                          │  ├─存放声效资产的位置，包括所有Sound类型以及音效合成器等暂时不用创建子集，(如果有需要归类的可以创建一个子集来存放)
    │  ├─Tex                                            │  ├─存放纹理资产的位置，包括2D纹理3D纹理等所有纹理类型，纹理导入时必须要创建一个分类子集，子集命名为资产名字
    │  │  └─Player                                      │  │  └─子集：这里存放是项目示例的Player的纹理
    │  └─Vidio                                          │  └─存放视频资产的位置
    ├─Debug                                             ├─用于Debug的文件夹，主要用于测试，该文件夹会被Label资产管理单独打包，这个里面创建的东西只能引用其他的资产，不能被其他资产引用；
    │  ├─Test                                           │  ├─存放测试用到东西：例如，你不会使用某个蓝图，想要试一下就复制过来直接在这个里面测试一下，试完记得删除；
    │  └─Tool                                           │  └─用于存放编辑器工具：如Noise纹理生成等实用工具，（当前项目不一定会用到）
    ├─Game                                              ├─真正的游戏核心数据了，分成多份，会被单独打包成Pak文件，使用label资产管理打包，便于直接跟换Pak文件（热更）不需要打包和下载整个文件
    │  ├─GameMap                                        │  ├─主要玩法部分：
    │  │  ├─Blueprint                                   │  │  ├─蓝图部分：不包含UMG，UMG有单独的UI动画等东西；
    │  │  │  ├─Characters                               │  │  │  ├─角色文件夹：包含所有角色每个角色蓝图文件都在这里，每个角色都要创建一个子集，命名为角色的英文名；
    │  │  │  │  └─Protagonist                           │  │  │  │  └─Protagonist（英文名：主角）
    │  │  │  │      ├─AnimBP                            │  │  │  │      ├─AnimBP 角色动画蓝图存放；
    │  │  │  │      └─IK                                │  │  │  │      └─IK：角色IK存放，如有需要直接在虚幻K动画的，直接在该文件夹下面创建一个关卡A，在这个关卡A里面K动画；
    │  │  │  └─Player                                   │  │  │  └─Player，玩家控制层，包含GameMode，PlayerController，gameState
    │  │  │      └─Input                                │  │  │      └─Input，输入映射资产
    │  │  │          └─Action                           │  │  │          └─Action，输入映射Action
    │  │  ├─DataAsset                                   │  │  ├─DataAsset,所有游戏数据供给到策划等人员调整数据，包含表格，数据资产，曲线平滑数据等；
    │  │  │  ├─Curve                                    │  │  │  ├─Curve 曲线
    │  │  │  ├─DA                                       │  │  │  ├─DA   数据资产
    │  │  │  └─DT                                       │  │  │  └─DT  数据表格
    │  │  └─Maps                                        │  │  └─Maps，关卡文件夹
    │  └─MenuMap                                        │  └─MenuMap ；主菜单文件夹，和GameMap的内容隔开用于分Pak来热更；
    │      ├─Map                                        │      ├─Map
    │      └─UMG                                        │      └─UMG
    ├─GameRender                                        ├─GameRender ：游戏渲染的数据，由于Shader数据无法分包统一规划到一起
    │  ├─Mat                                            │  ├─Mat， 材质
    │  │  └─Player                                      │  │  └─Player ，当前Player材质
    │  └─MF                                             │  └─MF ，材质函数
    
# 补充：
     ├─Game                                             
     │  ├─GameMap                                        
     │  │  ├─Blueprint                                  
     │  │  │  ├─UMG  ,添加UMG文件夹到Blueprint子集里面，用于存放GameMap的游戏内控件                                   


## Source文件夹
      └─TapTapDemo_5_32   
            └─Game                                              └─Game 游戏总体文件夹，便于迁移放一个文件夹里里面  
               ├─Debug                                             ├─Debug 用于调试
               │  ├─EditorTool                                     │  ├─EditorTool
               │  └─TestClass                                      │  └─TestClass
               ├─GameplatTags                                      ├─GameplatTags  用于插件GameplayTags模块，存放Tag的声明和定义
               ├─Gameplay                                          ├─Gameplay  游戏核心玩法模块
               │  ├─Actor                                          │  ├─Actor，所有Actor的存放处
               │  ├─Character                                      │  ├─Character  所有角色存放处
               │  ├─Component                                      │  ├─Component  所有组件存放处
               │  ├─Player                                         │  ├─Player 存放GameMode，PlayerController，GameState，HUD ，没有单开UI文件夹，UI用MVVM模块架构了
               │  ├─Subsytem                                       │  ├─Subsytem  存放所有子系统文件夹
               │  └─DefaultWidget                                  │  └─DefaultWidget 存放非MVVM框架UI控件，虚幻默认的UMG                                               
               ├─Gas                                               ├─Gas 模块插件
               │  ├─Attribute                                      │  ├─Attribute  属性集：所有玩家的属性，例如血条，蓝条，体力条等
               │  ├─GA                                             │  ├─GA  ： GameplayAbility 玩家技能类，用于动态添加和移除
               │  ├─GC                                             │  ├─GC  ： GameplayCue音效类，大概不会用到
               │  └─GE                                             │  └─GE  ： GameplayEffect效果类 ，配合GA使用，用与技能的效果，如减少血量，减少蓝条，添加一个持续扣血Buff等效果
               └─MVVM                                              └─MVVM  ，MVVM模块插件
                   ├─UMG                                               ├─UMG  ， UMGc++类
                   └─ViewMode                                          └─ViewMode ，MVVM核心数据类型


# 补充：NONE

## 项目文件夹
      └─仓库   
         ├─Asset                                      用于存放美术资产，还没有导入的，例如PS项目，Blender项目，纹理png，模型fbx等等                                   
         ├─Shaders                                    ush和usf文件存放
         ├─README_Image                               存放Readme文件中图片的引用; 
         └─......                                     项目其他文件夹，例如Config ，Content，等文件夹    


### 对话系统使用 :
* 采用配资产表的方式来配置：
* ## 1 ： 进入到 ' /Content/Game/GameMap/DataAsset/DA/ ' 目录下的' DA_Dialoage.uasset ' 里面：
* ![Dialoage路径图片png](https://github.com/sanqingcha/TapTap21_GameDemo/blob/main/README_Image/DialoageAsset.png)
* ## 2 ： 在资产表中分配对话的内容和音频 ：
* # 资产中配置新对话，是使用Gameplaytag标记对话内容：
* # 首先在DialoageMapData的这一行的 + 按钮来添加一个新对话数据，如下图新加一行 Element0 空数据
* ![添加数据元素图片png](https://github.com/sanqingcha/TapTap21_GameDemo/blob/main/README_Image/Dialoage1.png)
* # 再点开键（Key）这一行后面的 标签 后并在界面点击 + 号添加标签名 ， 标签名的格式为 “Game.Dialoage.你的tag名” 这里我创建了一个Game.Dialoage.Gaylao ,  并且下方的源要选择DefaultGameplayTags.ini ,填好Tag数据后点击添加新标签
* ![比赛图片](https://github.com/sanqingcha/TapTap21_GameDemo/blob/main/README_Image/Dialoage2.png)
* # 创建好tag后把创建的tag勾上：
* ![比赛图片](https://github.com/sanqingcha/TapTap21_GameDemo/blob/main/README_Image/Dialoage3.png)
* # 然后到值（Value）位置点击 “ + ” 来添加对话内容和音频 ，没有音频就不填 ， 和播放延迟（相对于上一句的延迟，用于校准音频间隔）,这里我使用了一点成都超人的对话：
* ![比赛图片](https://github.com/sanqingcha/TapTap21_GameDemo/blob/main/README_Image/Dialoage4.png)
* # 接着可以在任意地方使用Tag触发，这里我使用是球体碰撞检测触发对话：蓝图中搜索DialoageSystem，拿到节点后搜索TrriggerDialoage节点 ，使用Make节点创建一个临时Tag（ 这个tag选择刚刚表里面自己创建的Tag，我这里创建的是Game.Dialoage.Gaylao ）变量传入
* ![比赛图片](https://github.com/sanqingcha/TapTap21_GameDemo/blob/main/README_Image/Dialoage5.png)




































