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
    PUSH,
    POP,
    ADD,
    SUB,
    JEQ,
    CALL,
    RET,
    HALT
};

typedef struct _CPU {
    int32_t registers[8];
    uint8_t *memory, is_running;
    uint32_t program_counter, flags;
} CPU;

void push(CPU *cpu, int32_t content);
int32_t pop(CPU *cpu);
uint32_t fetch(CPU *cpu);
uint8_t extract_op(uint32_t instruction);
void set_equal(CPU *cpu);
void set_not_equal(CPU *cpu);
void jump(CPU *cpu, int32_t location);
void run(CPU *cpu);
void make_instruction(CPU *cpu, uint8_t op, uint8_t dst, uint8_t src);
CPU *new_cpu();
