#include <iostream>
#include "pvz.h"
#include <unistd.h>
PvZ pvz;
// 示例修改程序
void example() {
    
    // 示例：每行放50个冰瓜
    BulletBuilder builder;
    builder.setSpeed(3, -0);
    builder.bullet = 5;
    builder.move_type = 7;
    builder.hurt_type = 1;
    builder.unknown = 0x50000;
    for (int i = 1; i <= 6; ++i) {
        builder.setPosition(Coord(i, 1));
        pvz.AddBullet(builder, 50);
    }
    
    // 开启上帝模式
    pvz.FullScreenWoGua(true);
    pvz.FullScreenZengGe(true);
    pvz.FumeshroomOneLine(true);
    pvz.ChomperSwallowEverything(true);
    pvz.KelpPullEverything(true);
    //    pvz.fastShoot(true); // 场上子弹过多可能会崩游戏，请适度使用
    pvz.ButterPult();
    pvz.PlantWithoutSun(true);
    pvz.NoCoolDown(true);
    pvz.PurplePlantAvailable(true);
    pvz.PlantFreely(true);
    pvz.PutZombieFreely(true);
    
    // 开启柏学模式
    //    cannonShooter(true);
}
// test
// 曾哥攻击碰撞箱全屏
// 修改碰撞箱并不能实现全屏。曾哥只攻击3行内所有僵尸。
// 因为get target zombie，循环获取瞄准僵尸时曾哥有一个是否在3行以内的判断。
void ZenggeFullScreenAttackHitbox() {
    // 3e42c~3e47b+5
    // 3e4e8~3e52d
    std::array<byte, 69> fullscreen = pvz.ReadMemory<byte, 69>(0x3e4e8);
    std::array<byte, 84> gloomshroom;
    gloomshroom.fill(0x90);
    memcpy(gloomshroom.data(), fullscreen.data(), fullscreen.size());
    // 修改jmp
    char jmp[5] = {static_cast<char>(0xe9), 0x00, 0x00, 0x00, 0x00};
    int jmp_offset = 0x3e57d-0x3e42c-fullscreen.size()-5;
    int* offset = (int*)&jmp[1];
    *offset = jmp_offset;
    memcpy(gloomshroom.data()+fullscreen.size(), jmp, sizeof(jmp));
    // 修正call的offset
    int* call_offset = (int*)(gloomshroom.data()+39);
    *call_offset += 0x3e4e8-0x3e42c;
    pvz.WriteMemory(gloomshroom, 0x3e42c);
}
int main(int argc, const char * argv[]) {
    pvz.PlantWithoutSun(true);
    pvz.PlantFreely(true);
    pvz.PurplePlantAvailable(true);
    pvz.NoCoolDown(true);
    pvz.FullScreenZengGe(true);
    pvz.FullScreenWoGua(true);
    pvz.ChomperSwallowEverything(true);
    pvz.FumeshroomOneLine(true);
    pvz.KelpPullEverything(true);
    pvz.ButterPult();
    pvz.PutZombieFreely(true);
    pvz.InfiniteLawnMower(true);
    return 0;
    // 以下是测试
    // 舞王不转身
    pvz.WriteMemory({0x90, 0x90, 0x90}, 0xea4fc);
    
    // 手动放僵尸
    sleep(50);
    pvz.code.asm_init_newThread();
    pvz.code.asm_set_zombie(3, 10, 0x8);
//    pvz.code.asm_set_zombie(0, -2, 0xc);
    pvz.code.asm_ret();
    pvz.code.asm_create_thread();
    sleep(25);
    pvz.code.asm_init_newThread();
    pvz.code.asm_set_zombie(3, 10, 0x8);
    //    pvz.code.asm_set_zombie(0, -2, 0xc);
    pvz.code.asm_ret();
    pvz.code.asm_create_thread();
    return 0;
}
