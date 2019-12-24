#include <iostream>
#include "pvz.h"
#include <unistd.h>
int main(int argc, const char * argv[]) {
//    pvz.EasyCheat(true);
    PvZ pvz;
    pvz.PlantInIZombieMode();
    pvz.RedNutExplode();
    /*
     2a264:
     jns patch
     
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
    /*
    PvZ pvz;
    uint32_t patch_addr = (uint32_t)pvz.memory.Allocate(1024, VM_PROT_ALL);
    // 2a264
    {
        CodeHolder code;
        CodeInfo codeinfo(ArchInfo::kIdX86);
        codeinfo.setBaseAddress(0x2a264);
        code.init(codeinfo);
        x86::Assembler as(&code);
        
        as.jns(patch_addr);
        
        CodeBuffer& buffer = code.textSection()->buffer();
        uint8_t* data = buffer.data();
        size_t length = buffer.size();
        assert(length == 6);
        pvz.memory.Write(0x2a264, length, data);
    }
    // patch
    {
        CodeHolder code;
        CodeInfo codeinfo(ArchInfo::kIdX86);
        codeinfo.setBaseAddress(patch_addr);
        code.init(codeinfo);
        x86::Assembler as(&code);
        
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
        
        CodeBuffer& buffer = code.textSection()->buffer();
        uint8_t* data = buffer.data();
        size_t length = buffer.size();
        pvz.memory.Write(patch_addr, length, data);
//        for (size_t i = 0; i < length; i++)
//            printf("%02X", data[i]);
//        printf("\n");
    }
     */
    
    return 0;
//    int plant_array = pvz.ReadMemory<int>(pvz.base, 0x780, 0xa0);
//    int plant_ptr = plant_array + 0 * 0x14c;
//    int plant_hp_addr = plant_ptr + 0x40;
//    std::cout << std::hex << plant_hp_addr << std::endl;
//    while (true) {
//        std::cout << pvz.ReadMemory<int>(plant_hp_addr) << std::endl;
//        usleep(10000);
//    }
}
