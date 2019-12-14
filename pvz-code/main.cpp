#include <iostream>
#include "pvz.h"
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
        pvz.addBullet(builder, 50);
    }
    
    // 开启上帝模式
    pvz.fullScreenWoGua(true);
    pvz.fullScreenZengGe(true);
    pvz.ChomperSwallowEverything(true);
    pvz.KelpPullEverything(true);
    //    pvz.fastShoot(true); // 场上子弹过多可能会崩游戏，请适度使用
    pvz.butterPult();
    pvz.plantWithoutSun(true);
    pvz.NoCoolDown(true);
    pvz.PurplePlantAvailable(true);
    pvz.plantFreely(true);
    pvz.putZombieFreely(true);
    
    // 开启柏学模式
    //    cannonShooter(true);
}
int main(int argc, const char * argv[]) {
    
    return 0;
}
