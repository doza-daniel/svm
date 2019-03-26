#ifndef CPU_H
#define CPU_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

enum REGS {
    EAX,
    EBX,
    ECX,
    EDX,
    ESI,
    EDI,
    ESP,
    EBP
};

enum OPS {
    MOV,
    MOVI,
    MOVA,
    PUSH,
    PUSHI,
    POP,
    ADD,
    ADDI,
    SUB,
    SUBI,
    MUL,
    JEQ,
    JEQI,
    JMP,
    CALL,
    CALLI,
    RET,
    HALT
};

typedef struct _CPU {
    uint32_t registers[8];
    uint8_t *memory, is_running;
    uint32_t program_counter, flags;
} CPU;

// write four bytes to stack and adjust ESP
void push(CPU *cpu, uint32_t content);
// read four bytes from stack and adjust ESP
uint32_t pop(CPU *cpu);
// read next instruction from memory and increment program counter
uint32_t fetch(CPU *cpu);

// read 8 most significant bits as operation code
uint8_t extract_op(uint32_t instruction);
// read bytes [9-16] as destination register number
uint8_t extract_dst(uint32_t instruction);
// read 16 least significant bits as register number or immediate value
uint16_t extract_src(uint32_t instruction);

// set the least significant bit of the flags variable to 1
void set_equal(CPU *cpu);
// set the least significant bit of the flags variable to 0
void set_not_equal(CPU *cpu);
// set program counter to `location`
void jump(CPU *cpu, uint32_t location);
// start cpu loop
void run(CPU *cpu);
// encode an instruction to memory
void make_instruction(CPU *cpu, uint8_t op, uint8_t dst, uint16_t src);

// initialize CPU struct
CPU *new_cpu();

// Write four bytes to memory
void write_to_mem(uint8_t memory[], uint32_t location, uint32_t content);

// Read four bytes from memory
uint32_t read_from_mem(uint8_t memory[], uint32_t location);

#endif
