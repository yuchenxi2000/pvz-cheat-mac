#include "pvz.h"
PvZ::PvZ() : code(memory) {
    memory.Attach(ProcessName);
}
PvZ::~PvZ() {
    memory.Detach();
}
// 自由种植物
void PvZ::PlantFreely(bool on) {
    if (on)
        WriteMemory<byte>(0x83, 0x2a38b+1); // 植物任意种植
    else
        WriteMemory<byte>(0x84, 0x2a38b+1); // 解除植物任意种植
}
// 自由放僵尸（我是僵尸模式）
void PvZ::PutZombieFreely(bool on) {
    if (on)
        WriteMemory<byte>(0x83, 0xac868+1); // 僵尸任意放置
    else
        WriteMemory<byte>(0x84, 0xac868+1); // 解除僵尸任意放置
}
// 全屏窝瓜
void PvZ::FullScreenWoGua(bool on) {
    if (on) {
        // 窝瓜启动范围全屏
        WriteMemory<byte, 6>({0x0f, 0x81, 0x26, 0x01, 0x00, 0x00}, 0x3fff0);
        WriteMemory<byte>(0x70, 0x401b7);
        // 窝瓜攻击范围全屏
        WriteMemory<byte, 2>({0x90, 0x90}, 0x4029c);
        WriteMemory<byte, 2>({0x90, 0x90}, 0x402b5);
        WriteMemory<byte, 2>({0x90, 0x90}, 0x40300);
    } else {
        WriteMemory<byte, 6>({0x0f, 0x85, 0x0e, 0x02, 0x00, 0x00}, 0x3fff0);
        WriteMemory<byte>(0x75, 0x401b7);
        WriteMemory<byte, 2>({0x75, 0x7f}, 0x4029c);
        WriteMemory<byte, 2>({0x75, 0x66}, 0x402b5);
        WriteMemory<byte, 2>({0x7e, 0x1b}, 0x40300);
    }
}
// 植物疯狂射子弹
void PvZ::FastShoot(bool on) {
    if (on) {
        WriteMemory<byte>(0x80, 0x41970+1); // 植物狂暴
        WriteMemory<byte, 3>({0xeb, 0x36, 0x90}, 0x41895); // 机枪强化
    } else {
        WriteMemory<byte>(0x85, 0x41970+1); // 解除植物狂暴
        WriteMemory<byte, 3>({0x8b, 0x45, 0x08}, 0x41895); // 解除机枪强化
    }
}
void PvZ::ButterPult() {
    WriteMemory<byte>(0x70, 0x40d3a); // 黄油投手
}
void PvZ::KernelPult() {
    WriteMemory<byte>(0x71, 0x40d3a); // 玉米粒投手
}
void PvZ::KernelButterPult() {
    WriteMemory<byte>(0x75, 0x40d3a); // 正常玉米投手
}
// 所有植物无视僵尸位置攻击
void PvZ::AlwaysShoot() {
    WriteMemory<byte, 5>({0xe9, 0xb7, 0x03, 0x00, 0x00}, 0x3e6d2);
}
// 全屏曾哥
void PvZ::FullScreenZengGe(bool on) {
    if (on) {
        // 曾哥攻击范围全屏
        // 0x40aa4: jno 0x40b33
        WriteMemory<byte, 6>({0x0f, 0x81, 0x89, 0x00, 0x00, 0x00}, 0x40aa4);
        // 无视3x3限制攻击
        // 3e79a: mov eax, dword [ebp-60] 8b 45 c4
        // 3e79d: jmp 3ea8e               e9 cc 02 00 00
        WriteMemory<byte, 10>({0x8b, 0x45, 0xc4, 0xe9, 0xec, 0x02, 0x00, 0x00, 0x90, 0x90}, 0x3e79a);
    } else {
        WriteMemory<byte, 6>({0x0f, 0x8c, 0x14, 0x01, 0x00, 0x00}, 0x40aa4);
        WriteMemory<byte, 10>({0x83, 0x7d, 0xe4, 0xff, 0x0f, 0x8c, 0xca, 0x02, 0x00, 0x00}, 0x3e79a);
    }
}
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
void PvZ::AddBullet(const BulletBuilder & builder, int cnt) {
    code.asm_init_newThread();
    code.asm_mov_dword_ptr_esp_add(0x18, cnt);
    // loop:
    code.asm_mov_dword_ptr_esp_add(0x14, builder.bullet); // 11
    code.asm_mov_dword_ptr_esp_add(0x10, builder.shadow_row);
    code.asm_mov_dword_ptr_esp_add(0xC, builder.unknown);
    code.asm_mov_dword_ptr_esp_add(0x8, builder.y);
    code.asm_mov_dword_ptr_esp_add(0x4, builder.x);
    code.asm_mov_exx_dword_ptr(Reg::EAX, 0x35EE64); // 5
    code.asm_mov_exx_dword_ptr_exx_add(Reg::EAX, 0x780); // 6
    code.asm_mov_ptr_esp_add_exx(0x0, Reg::EAX); // 4
    code.asm_call(0x2CA3A); // 13
    // mov dword [eax+0x58], 0x07   c7 40 58 07 00 00 00
    code.asm_add_byte(0xc7); code.asm_add_byte(0x40);
    code.asm_add_byte(0x58); code.asm_add_dword(builder.move_type); // 7
    // mov dword [eax+0x74], 0x01   c7 40 74 01 00 00 00
    code.asm_add_byte(0xc7); code.asm_add_byte(0x40);
    code.asm_add_byte(0x74); code.asm_add_dword(builder.hurt_type);
    // mov dword [eax+0x3c], 0xc0551eb8
    code.asm_add_byte(0xc7); code.asm_add_byte(0x40);
    code.asm_add_byte(0x3c); code.asm_add_dword(*(int*)&builder.vx);
    // mov dword [eax+0x40], 0xc0551eb8
    code.asm_add_byte(0xc7); code.asm_add_byte(0x40);
    code.asm_add_byte(0x40); code.asm_add_dword(*(int*)&builder.vy);
    // mov eax, dword [esp+0x18] 8b 44 24 18
    code.asm_add_byte(0x8b); code.asm_add_byte(0x44); // 4
    code.asm_add_byte(0x24); code.asm_add_byte(0x18);
    // sub eax, 0x1
    code.asm_add_byte(0x83); code.asm_add_byte(0xe8); // 3
    code.asm_add_byte(0x1);
    // mov dword [esp+0x18], eax
    code.asm_add_byte(0x89); code.asm_add_byte(0x44); // 4
    code.asm_add_byte(0x24); code.asm_add_byte(0x18);
    // test eax, eax
    code.asm_add_byte(0x85); code.asm_add_byte(0xc0); // 2
    // jne loop [-133]
    code.asm_add_byte(0x0f); code.asm_add_byte(0x85);
    code.asm_add_dword(-130);
    
    code.asm_ret();
    code.asm_create_thread();
}
// 植物射玉米炮
void PvZ::CannonShooter(bool on) {
    code.asm_init_codeInject();
    // mov dword [ebp+28], 0xb
    code.asm_add_byte(0xc7);
    code.asm_add_byte(0x45);
    code.asm_add_byte(0x1c);
    code.asm_add_dword(0xb);
    code.asm_code_inject(on, 0x2ca43, 5);
}
// 无视阳光种植
void PvZ::PlantWithoutSun(bool on) {
    if (on) {
        // 阳光不够时卡片依然可用
        WriteMemory<byte, 2>({0x90, 0x90}, 0x1c6da);
        // 阳光不够时仍能种在场地上，且阳光不减
        // 0x1c71c: jmp 0x1c735
        WriteMemory<byte, 2>({0xeb, 0x17}, 0x1c71c);
    } else {
        WriteMemory<byte, 2>({0x74, 0x09}, 0x1c6da);
        WriteMemory<byte, 2>({0x74, 0x20}, 0x1c71c);
    }
}
// @origin
void PvZ::NoCoolDown(bool on) {
    if (on) {
        WriteMemory<byte>(0x70, 0xF8334);
        //WriteMemory<byte>(0xEB,0xF848C);
        WriteMemory({0x31, 0xC0}, 0xF84A8);
    } else {
        WriteMemory<byte>(0x7E, 0xF8334);
        //WriteMemory<byte>(0x75,0xF848C);
        WriteMemory({0x89, 0xC8}, 0xF84A8);
    }
}
// @origin
void PvZ::PurplePlantAvailable(bool on) {
    if (on) {
        WriteMemory({0xB8, 0x01, 0x00, 0x00, 0x00, 0xC3}, 0x1F3E0);
        WriteMemory<byte>(0xEB, 0x1F12B);
        WriteMemory<byte>(0xEB, 0x1F232);
    } else {
        WriteMemory({0x55, 0x89, 0xE5, 0x83, 0xEC, 0x28}, 0x1F3E0);
        WriteMemory<byte>(0x74, 0x1F12B);
        WriteMemory<byte>(0x74, 0x1F232);
    }
}
// 食人花吞巨人、僵王
void PvZ::ChomperSwallowEverything(bool on) {
    if (on) {
        WriteMemory<byte>(0x0, 0x3f363);
    } else {
        WriteMemory<byte>(0x1, 0x3f363);
    }
}
// 海草拉陆地僵尸
void PvZ::KelpPullEverything(bool on) {
    if (on) {
        // 3e958: jo 3ea6e
        WriteMemory<byte>(0x80, 0x3e959);
    } else {
        WriteMemory<byte>(0x85, 0x3e959);
    }
}
// 大喷菇一行（改攻击hitbox）
void PvZ::FumeshroomOneLine(bool on) {
    if (on) {
        WriteMemory<int>(0x640, 0x3e3eb);
    } else {
        WriteMemory<int>(0x154, 0x3e3eb);
    }
}
// 割草机不动
void PvZ::InfiniteLawnMower(bool on) {
    if (on) {
        WriteMemory({0xe9, 0xb0, 0x00, 0x00, 0x00}, 0x8c03);
    } else {
        WriteMemory({0x83, 0xec, 0x28, 0x8b, 0x45}, 0x8c03);
    }
}
// 我是僵尸模式阳光不足、没僵尸时不死
void PvZ::IZombieNoDie(bool on) {
    if (on) {
        WriteMemory<byte>(0xeb, 0xad050);
    } else {
        WriteMemory<byte>(0x7f, 0xad050);
    }
}
