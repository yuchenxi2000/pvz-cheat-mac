#ifndef PVZ_H
#define PVZ_H

#include <initializer_list>
#include <string>
#include <cmath>
#include <array>
#include "memory.h"
#include "code.h"

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
    // 自由种植物
    void PlantFreely(bool on);
    // 自由放僵尸（我是僵尸模式）
    void PutZombieFreely(bool on);
    // 全屏窝瓜
    void FullScreenWoGua(bool on);
    // 植物疯狂射子弹
    void FastShoot(bool on);
    void ButterPult();
    void KernelPult();
    void KernelButterPult();
    // 所有植物无视僵尸位置攻击
    void AlwaysShoot();
    // 全屏曾哥
    void FullScreenZengGe(bool on);
    // 直接在场地上放子弹
    // Bullet* put_bullet_2ca3a(PvZ*, int x, int y, int, int shadow_row, int bullet_type);
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
     d: 豌豆
     以后就崩溃了
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
    void ChomperSwallowEverything(bool on);
    void KelpPullEverything(bool on);
    void FumeshroomOneLine(bool on);
    void InfiniteLawnMower(bool on);
    void IZombieNoDie(bool on);

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
    
    bool isGameOn(bool alert = true);
    
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
