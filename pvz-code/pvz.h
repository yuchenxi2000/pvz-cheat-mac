#ifndef PVZ_H
#define PVZ_H

#include <initializer_list>
#include <string>
#include <cmath>
#include <array>
#include "memory.h"
#include "code.h"
#include "patch.hpp"

typedef uint8_t byte;

struct Coord {
    union {
        struct {
            float row, col;
        };
        struct {
            float y, x;
        };
    };
    Coord() {}
    Coord(float y, float x) : x(x), y(y) {}
};
struct BulletBuilder {
    int x, y; // 子弹位置
    int unknown;
    int shadow_row; // 攻击判定的行数
    int bullet; // 子弹类型
    float vx, vy; // 速度
    int hurt_type;
    int move_type;
    void setPosition(const Coord & grid) {
        shadow_row = floor(grid.row-1);
        y = 85*grid.row;
        x = 80*grid.col-40;
    }
    void setSpeed(float vx, float vy) {
        this->vx = vx;
        this->vy = vy;
    }
};

class PvZ {

public:
    PvZ();
    ~PvZ();
    
public:
    // cheat
    // 自由种植物
    void PlantFreely(bool on);
    // 自由放僵尸（我是僵尸模式）
    void PutZombieFreely(bool on);
    // 全屏窝瓜
    void FullScreenWoGua(bool on);
    // 植物疯狂射子弹
    void FastShoot(bool on);
    // 欧皇投手
    void ButterPult();
    // 非酋投手
    void KernelPult();
    // 正常玉米投手
    void KernelButterPult();
    // 所有植物无视僵尸位置攻击
    void AlwaysShoot();
    // 全屏曾哥
    void FullScreenZengGe(bool on);
    // 直接在场地上放子弹
    // Bullet* put_bullet_2ca3a(PvZ*, int x, int y, int, int row, int bullet_type);
    /*
     bullet 类型取值
     0: 豌豆
     1: 冰豌豆
     2: 卷心菜
     3: 西瓜
     4: 孢子
     5: 冰西瓜
     6: 看不见
     7: 星星
     8: 尖刺
     9: 篮球
     a: 玉米粒
     b: 玉米炮
     c: 黄油
     d: 豌豆（植物僵尸）
     */
    /*
     hurt_type
     仙人掌：1 2
     樱桃、辣椒、玉米炮、毁灭菇：0x7f
     卷心菜、玉米、西瓜、冰西瓜：0xd
     土豆：0x4d
     窝瓜：0xd
     小喷、大喷、水兵、忧郁菇、大嘴花：9
     香蒲：0xb
     水草：5
     32：11
     其他：1
     */
    /*
     move_type
     3：三线射手朝上一行飞
     4：倒飞，隐形
     5：飞一段时间后消失
     6：倒飞不隐形
     7：杨桃飞行方式
     8：飞得巨慢，打不到僵尸
     9：打不到僵尸
     */
    void AddBullet(const BulletBuilder & builder, int cnt);
    // 植物射玉米炮
    void CannonShooter(bool on);
    // 无视阳光种植
    void PlantWithoutSun(bool on);
    // @origin
    void NoCoolDown(bool on);
    // @origin
    void PurplePlantAvailable(bool on);
    // 食人花吞巨人、僵王
    void ChomperSwallowEverything(bool on);
    // 海草拉陆地僵尸
    void KelpPullEverything(bool on);
    // 大喷菇一行（改攻击hitbox）
    void FumeshroomOneLine(bool on);
    enum LawnMowerState {
        NORMAL, INFIN, CIRCULATION
    };
    // 无限割草机、循环割草机
    void InfiniteLawnMower(LawnMowerState state);
    // 我是僵尸模式阳光不足、没僵尸时不死
    void IZombieNoDie(bool on);
    // 任何游戏模式均放小推车
    void SetLawnMowerForAllLevel(bool on);
    // 全屏辣椒
    void FullScreenJalapeno(bool on);
    // 所有植物变成辣椒并瞬间爆炸
    void AllPlantsExplode();
    // 所有植物变成核菇并瞬间爆炸
    void AllPlantsExplode2();
    // 生成奖杯/钱袋
    void GenGoldenCup();
    
    // patch
    // 原版补丁：爆炸坚果死亡时爆炸，高坚果死亡时压死僵尸
    void PatchRedNutExplode();
    // 原版补丁：正确地放置植物、僵尸
    void PatchUseCard();
    // 原版补丁：点击卡片时不会崩溃（如奖杯、钻石等修改器修改出的卡片）
    // 点击奖杯获得钱袋
    // 点击阳光获得1000阳光
    // 点击钻石获得钻石
    // 点击潜水僵尸僵尸进家
    void PatchClickCard();
    
    // 标准开挂
    void EasyCheat(bool on);
    
    // test
    void WinterMelon();
    void testZenggeFullScreenAttackHitbox();

//private:
    template<typename T, typename... Args>
    T ReadMemory(Args... address);
    
    template<typename T, size_t size, typename... Args>
    std::array<T, size> ReadMemory(Args... address);
    
    template<typename T, typename... Args>
    void WriteMemory(T value, Args... address);
    
    template<typename T, size_t size, typename... Args>
    void WriteMemory(std::array<T, size> value, Args... address);
    
    void WriteMemory(std::initializer_list<byte> il, uintptr_t address);
    
//    bool isGameOn(bool alert = true);
    
    Memory memory;
    Code code;
    pid_t pid = 0;
    const uint32_t base = 0x35EE64;
    const std::string ProcessName = "Plants vs. Zombi";
};

template<typename T, typename... Args>
inline T PvZ::ReadMemory(Args... address) {
    return memory.ReadMemory<T>({static_cast<uintptr_t>(address)...});
}

template<typename T, size_t size, typename... Args>
inline std::array<T, size> PvZ::ReadMemory(Args... address) {
    return memory.ReadMemory<T, size>({static_cast<uintptr_t>(address)...});
}

template<typename T, typename... Args>
inline void PvZ::WriteMemory(T value, Args... address) {
    memory.WriteMemory<T>(value, {static_cast<uintptr_t>(address)...});
}

template<typename T, size_t size, typename... Args>
void PvZ::WriteMemory(std::array<T, size> value, Args... address) {
    memory.WriteMemory<T, size>(value, {static_cast<uintptr_t>(address)...});
}

inline void PvZ::WriteMemory(std::initializer_list<byte> il, uintptr_t address) {
    memory.Write(address, il.size(), (void *) il.begin());
}

#endif //PVZ_H
