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
// 无限割草机、循环割草机
void PvZ::InfiniteLawnMower(LawnMowerState state) {
    switch (state) {
        case NORMAL:
            WriteMemory<byte, 2>({0x74, 0x48}, 0x92b8);
            WriteMemory<int>(0x1, 0x863e);
            WriteMemory<int>(0x0, 0x92eb);
            break;
            
        case INFIN:
            WriteMemory<byte, 2>({0x90, 0x90}, 0x92b8);
            WriteMemory<int>(0x1, 0x863e);
            WriteMemory<int>(0x0, 0x92eb);
            break;
            
        case CIRCULATION:
            WriteMemory<byte, 2>({0x90, 0x90}, 0x92b8);
            WriteMemory<int>(0x2, 0x863e);
            WriteMemory<int>(0x2, 0x92eb);
            break;
            
        default:
            break;
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
// 任何游戏模式均放小推车
void PvZ::SetLawnMowerForAllLevel(bool on) {
    if (on) {
        WriteMemory<byte, 2>({0x90, 0x90}, 0x11086);
    } else {
        WriteMemory<byte, 2>({0x75, 0x2c}, 0x11086);
    }
}
void PvZ::FullScreenJalapeno(bool on) {
    if (on) {
        WriteMemory<byte, 2>({0xeb, 0x3cd11-0x3cce9-2}, 0x3cce9);
    }else {
        WriteMemory<byte, 2>({0x75, 0x07}, 0x3cce9);
    }
}
void PvZ::AllPlantsExplode() {
    uint32_t plant_array = ReadMemory<uint32_t>(base, 0x780, 0xa0);
    int plant_max_cnt = ReadMemory<uint32_t>(base, 0x780, 0xa4);
    for (int i = 0; i < plant_max_cnt; ++i) {
        uint32_t plant_ptr = plant_array + i * 0x14c;
        WriteMemory<int>(0x1, plant_ptr + 0x50);
        // wake up mushroom
        WriteMemory<byte>(0x0, plant_ptr+0x143);
        WriteMemory<int>(0x14, plant_ptr + 0x24);
    }
}
void PvZ::AllPlantsExplode2() {
    uint32_t plant_array = ReadMemory<uint32_t>(base, 0x780, 0xa0);
    int plant_max_cnt = ReadMemory<uint32_t>(base, 0x780, 0xa4);
    for (int i = 0; i < plant_max_cnt; ++i) {
        uint32_t plant_ptr = plant_array + i * 0x14c;
        WriteMemory<int>(0x1, plant_ptr + 0x50);
        // wake up mushroom
        WriteMemory<byte>(0x0, plant_ptr+0x143);
        WriteMemory<int>(0x0f, plant_ptr + 0x24);
    }
}
void PvZ::RedNutExplode() {
//    code.asm_init_codeInject();
//    code.asm_add_byte(0x83); code.asm_add_byte(0xfa); code.asm_add_byte(0x40);
//    code.asm_add_byte(0x7f); code.asm_add_byte(0x22);
//    code.asm_add_byte(0x8b); code.asm_add_byte(0x45); code.asm_add_byte(0xc);
//    code.asm_add_byte(0x8b); code.asm_add_byte(0x40); code.asm_add_byte(0x24);
//    code.asm_add_byte(0x83); code.asm_add_byte(0xf8); code.asm_add_byte(0x31);
//    code.asm_add_byte(0x75); code.asm_add_byte(0x17);
//    code.asm_add_byte(0x8b); code.asm_add_byte(0x45); code.asm_add_byte(0xc);
//    code.asm_add_byte(0xc7); code.asm_add_byte(0x80); code.asm_add_dword(0x24); code.asm_add_dword(0x2);
//    code.asm_add_byte(0xc7); code.asm_add_byte(0x80); code.asm_add_dword(0x50); code.asm_add_dword(0x1);
//    code.asm_code_inject(false, 0xd82cd, 13);
    
    code.asm_init_codeInject();
    // cmp edx, 0x40  (edx: plant+0x40)  3
    // jg end                            2
    // mov eax, dword [ebp+0xc]          3
    // mov eax, dword [eax+0x24]         3
    // cmp eax, 0x31                     3
    // jne tall_nut                      2
    // mov eax, dword [ebp+0xc]          3
    // mov dword [eax+0x24], 0x2         7
    // mov dword [eax+0x50], 0x1         7
    // tall-nut:
    // mov eax, dword [ebp+0xc]          3
    // mov eax, dword [eax+0x24]         3
    // cmp eax, 0x17                     3
    // jne end                           2
    // mov eax, dword [ebp+0xc]          3
    // mov dword [eax+0x24], 0x11        7
    // end:
    code.asm_cmp_exx(Reg::EDX, 0x40);
    code.asm_jg_short(49); // end
    code.asm_mov_eax_dword_ebp_add(0xc);
    code.asm_mov_eax_dword_eax_add(0x24);
    code.asm_cmp_exx(Reg::EAX, 0x31);
    code.asm_jne_short(17); // tall_nut
    code.asm_mov_eax_dword_ebp_add(0xc);
    code.asm_mov_dword_ptr_exx_add(Reg::EAX, 0x24, 0x2);
    code.asm_mov_dword_ptr_exx_add(Reg::EAX, 0x50, 0x1);
    // tall_nut
    code.asm_mov_eax_dword_ebp_add(0xc);
    code.asm_mov_eax_dword_eax_add(0x24);
    code.asm_cmp_exx(Reg::EAX, 0x17);
    code.asm_jne_short(10); // end
    code.asm_mov_eax_dword_ebp_add(0xc);
    code.asm_mov_dword_ptr_exx_add(Reg::EAX, 0x24, 0x11);
    // end
    
    code.asm_code_inject(true, 0xd82cd, 13);
}

void PvZ::EasyCheat(bool on) {
    PlantWithoutSun(on);
    PlantFreely(on);
    PurplePlantAvailable(on);
    NoCoolDown(on);
    FullScreenZengGe(on);
    FullScreenWoGua(on);
    FullScreenJalapeno(on);
    ChomperSwallowEverything(on);
    FumeshroomOneLine(on);
    KelpPullEverything(on);
    PutZombieFreely(on);
    if (on) {
        ButterPult();
        InfiniteLawnMower(INFIN);
    }else {
        KernelButterPult();
        InfiniteLawnMower(NORMAL);
    }
}

void PvZ::PlantInIZombieMode() {
    using namespace asmjit::x86;
    uint32_t patch_addr = (uint32_t)memory.Allocate(1024, VM_PROT_ALL);
    /*
     2a264:
     jns patch
     */
    {
        Patch patch(memory, 0x2a264);
        auto& as = patch.as;
        
        as.jns(patch_addr);
        
        patch.patch();
    }
    /*
     patch:
     mov eax, dword [ebp+0x8]
     mov eax, dword [eax+0x80]
     mov dword [esp], eax
     call is_game_mode_i_zombie_bb7a6
     test al, al
     je 2a2cf
     mov eax, dword [ebp+0x8]
     mov dword [esp], eax
     call get_hold_card_id_117ec
     cmp eax, 0x3b
     jle 2a2cf // plant card
     jmp 2a2a4 // zombie card
     */
    {
        Patch patch(memory, patch_addr);
        auto& as = patch.as;
        
        as.mov(eax, dword_ptr(ebp, 0x8));
        as.mov(eax, dword_ptr(eax, 0x80));
        as.mov(dword_ptr(esp), eax);
        as.call(0xbb7a6);
        as.test(al, al);
        as.je(0x2a2cf);
        as.mov(eax, dword_ptr(ebp, 0x8));
        as.mov(dword_ptr(esp), eax);
        as.call(0x117ec);
        as.cmp(eax, 0x3b);
        as.jle(0x2a2cf);
        as.jmp(0x2a2a4);
        
        patch.patch();
    }
}

// test
void PvZ::WinterMelon() {
    // 示例：每行放50个冰瓜
    BulletBuilder builder;
    builder.setSpeed(3, -0);
    builder.bullet = 5;
    builder.move_type = 7;
    builder.hurt_type = 1;
    builder.unknown = 0x50000;
    for (int i = 1; i <= 6; ++i) {
        builder.setPosition(Coord(i, 1));
        AddBullet(builder, 50);
    }
}
// test
// 曾哥攻击碰撞箱全屏
// 修改碰撞箱并不能实现全屏。曾哥只攻击3行内所有僵尸。
// 因为get target zombie，循环获取瞄准僵尸时曾哥有一个是否在3行以内的判断。
void PvZ::testZenggeFullScreenAttackHitbox() {
    // 3e42c~3e47b+5
    // 3e4e8~3e52d
    std::array<byte, 69> fullscreen = ReadMemory<byte, 69>(0x3e4e8);
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
    WriteMemory(gloomshroom, 0x3e42c);
}
// 舞王不转身
//    pvz.WriteMemory({0x90, 0x90, 0x90}, 0xea4fc);
