#ifndef patch_hpp
#define patch_hpp

#include <iostream>
#include <asmjit/asmjit.h>
#include "memory.h"
class Patch {
    uint32_t addr;
    asmjit::CodeHolder code;
    Memory& memory;
public:
    Patch(Memory& memory_writer, uint32_t addr) : memory(memory_writer), as(&code) {
        this->addr = addr;
        asmjit::CodeInfo codeinfo(asmjit::ArchInfo::kIdX86);
        codeinfo.setBaseAddress(addr);
        code.init(codeinfo);
    }
    asmjit::x86::Assembler as;
    void patch() {
        asmjit::CodeBuffer& buffer = code.textSection()->buffer();
        uint8_t* data = buffer.data();
        size_t length = buffer.size();
        memory.Write(addr, length, data);
    }
    size_t code_length() {
        return code.textSection()->buffer().size();
    }
};

#endif /* patch_hpp */
