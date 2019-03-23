#include "cpu.h"


void die(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

CPU *new_cpu() {
    CPU *c = (CPU*)malloc(sizeof(CPU));
    if (c == NULL) {
        die("could not allocate cpu");
    }
    c->memory = (uint8_t *)malloc(sizeof(uint8_t) * UINT32_MAX); if (c->memory == NULL) {
        die("could not allocate cpu memory");
    }
    c->registers[ESP] = UINT32_MAX;
    c->program_counter = 0;
    c->is_running = 0;
}

void push(CPU *cpu, int32_t content)
{
    cpu->registers[ESP] -= 4;
    cpu->memory[ESP] = content;
}

int32_t pop(CPU *cpu)
{
    int32_t ret = cpu->memory[cpu->registers[ESP]];
    cpu->registers[ESP] += 4;
    return ret;
}

uint32_t fetch(CPU *cpu)
{
    uint32_t instruction = (uint32_t)cpu->memory[cpu->program_counter] << 24 |
      (uint32_t)cpu->memory[cpu->program_counter + 1] << 16 |
      (uint32_t)cpu->memory[cpu->program_counter + 2] << 8  |
      (uint32_t)cpu->memory[cpu->program_counter + 3];
    cpu->program_counter += 4;
    return instruction;
}

uint8_t extract_op(uint32_t instruction)
{
    return instruction >> 24;
}

void set_equal(CPU *cpu)
{
    cpu->flags |= 0x1;
}

void set_not_equal(CPU *cpu)
{
    cpu->flags |= 0x0;
}

void jump(CPU *cpu, int32_t location)
{
    cpu->program_counter = location;
}


void run(CPU *cpu)
{
    cpu->is_running = 1;

    while (cpu->is_running) {
        uint32_t instruction = fetch(cpu);
        uint8_t op = extract_op(instruction);
        uint8_t dst = (instruction & 0x0000FF00) >> 8;
        uint8_t src = instruction & 0x000000FF;

        switch (op) {
            case HALT:
                printf("halt\n");
                cpu->is_running = 0;
                break;
            case PUSH:
                printf("push\n");
                push(cpu, cpu->registers[src]);
                break;
            case POP:
                printf("pop\n");
                cpu->registers[src] = pop(cpu);
                break;
            case ADD:
                printf("add\n");
                cpu->registers[dst] = cpu->registers[dst] + cpu->registers[src];
                if (cpu->registers[dst] == 0) {
                    set_equal(cpu);
                } else {
                    set_not_equal(cpu);
                }
                break;
            case SUB:
                printf("sub\n");
                cpu->registers[dst] = cpu->registers[dst] - cpu->registers[src];
                if (cpu->registers[dst] == 0) {
                    set_equal(cpu);
                    printf("set flag 0\n");
                    printf("%d\n", cpu->flags);
                } else {
                    set_not_equal(cpu);
                }

                break;
            case JEQ:
                printf("jeq\n");
                if (cpu->flags == 1) {
                    printf("jumping to %d\n", cpu->registers[src]);
                    jump(cpu, cpu->registers[src]);
                }
                break;
            case CALL:
                printf("call\n");
                push(cpu, cpu->program_counter);
                jump(cpu, cpu->registers[src]);
                break;
            case RET:
                printf("ret\n");
                jump(cpu, pop(cpu));
                break;
        }
    }
}



void make_instruction(CPU *cpu, uint8_t op, uint8_t dst, uint8_t src)
{
    static int instruction_cnt = 0;
    cpu->memory[cpu->program_counter + instruction_cnt] = op;
    cpu->memory[cpu->program_counter + instruction_cnt + 2] = dst;
    cpu->memory[cpu->program_counter + instruction_cnt + 3] = src;
    instruction_cnt += 4;
}
