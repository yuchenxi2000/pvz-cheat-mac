#ifndef CODE_H
#define CODE_H

#include "memory.h"
#include <cstddef>
#include <cstdint>
#define NDEBUG

enum class Reg : unsigned int {
    EAX = 0,
    EBX,
    ECX,
    EDX,
    ESI,
    EDI,
    EBP,
    ESP,
};

class Code {
    friend class ProcessCore;
    
    friend class xnu_proc;

public:
    explicit Code(Memory &memory);
    
    ~Code();
    
    // modified
    void asm_mov_eax_dword_ebp_add(uint8_t offset);
    
    void asm_mov_eax_dword_eax_add(uint8_t offset);
    
    void asm_cmp_exx(Reg reg, uint8_t value);
    
    void asm_jg_short(uint8_t offset);
    
    void asm_jne_short(uint8_t offset);
    
    void asm_test_al_al();
    
    void asm_mov_dword_esp_add_eax(uint8_t offset);
    
    void patch_plant_in_i_zombie();
    // end modified
    
    void asm_init_codeInject();
    
    void asm_init_newThread();
    
    void asm_add_byte(unsigned char);
    
    void asm_add_word(unsigned short);
    
    void asm_add_dword(unsigned int);
    
    void asm_push(int);
    
    void asm_mov_exx(Reg, int);
    
    void asm_mov_dword_ptr_exx_add(Reg reg, int offset, int value);
    
    void asm_add_exx(Reg, int);
    
    void asm_mov_exx_dword_ptr(Reg, int);
    
    void asm_mov_dword_ptr_exx(int, Reg);
    
    void asm_mov_exx_dword_ptr_exx_add(Reg, int);
    
    void asm_mov_ptr_esp_add_exx(unsigned char offset, Reg reg);
    
    void asm_mov_dword_ptr_esp_add(int offset, int value);
    
    void asm_push_exx(Reg);
    
    void asm_pop_exx(Reg);
    
    void asm_call(int);
    
    void asm_ret();
    
    void asm_code_inject(bool on, uint32_t address, size_t original_size);
    
    void asm_create_thread();
    
    void asm_set_plant(int row, int column, int type, bool imitater, bool iz_style);
    
    void asm_set_zombie(int row, int column, int type);
    
    void asm_spawn_zombie(int type);
    
    void asm_put_ladder(int row, int column);
    
    void asm_put_grave(int row, int column);
    
    void asm_put_rake(int row, int column);
    
    void asm_put_portal(int row, int column, int type);
    
    void asm_put_coin(int row, int column, int type, int scene);

protected:
    unsigned char *code;
    unsigned int length;

private:
    Memory &memory;
};

#endif
