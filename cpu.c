#include "cpu.h"
#include "debug.h"

void die(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

CPU *new_cpu() {
    CPU *c = (CPU*)malloc(sizeof(CPU));
    if (c == NULL) {
        die("could not allocate cpu");
    }
    c->memory = (uint8_t *)malloc(sizeof(uint8_t) * UINT32_MAX);
    if (c->memory == NULL) {
        die("could not allocate cpu memory");
    }
    c->registers[ESP] = UINT32_MAX;
    c->registers[EBP] = UINT32_MAX;
    c->program_counter = 0;
    c->is_running = 0;

    return c;
}

void push(CPU *cpu, uint32_t content)
{
    cpu->registers[ESP] -= 4;
    write_to_mem(cpu->memory, cpu->registers[ESP], content);
}

uint32_t pop(CPU *cpu)
{
    uint32_t ret = read_from_mem(cpu->memory, cpu->registers[ESP]);
    cpu->registers[ESP] += 4;
    return ret;
}

uint32_t fetch(CPU *cpu)
{
    uint32_t instruction = read_from_mem(cpu->memory, cpu->program_counter);
    cpu->program_counter += 4;
    return instruction;
}

uint8_t extract_op(uint32_t instruction)
{
    return instruction >> 24;
}

uint8_t extract_dst(uint32_t instruction)
{
    return (instruction & 0x00FF0000) >> 16;
}

uint16_t extract_src(uint32_t instruction)
{
    return instruction & 0xFF;
}

void set_equal(CPU *cpu)
{
    cpu->flags |= 0x1;
}

void set_not_equal(CPU *cpu)
{
    cpu->flags &= 0xFFFFFFFE;
}

void jump(CPU *cpu, uint32_t location)
{
    cpu->program_counter = location;
}


void run(CPU *cpu)
{
    cpu->is_running = 1;

    while (cpu->is_running) {
        uint32_t instruction = fetch(cpu);
        uint8_t op = extract_op(instruction);
        uint8_t dst = extract_dst(instruction);
        uint16_t src = extract_src(instruction);

        switch (op) {
            case HALT:
                debug("halt\n");
                cpu->is_running = 0;
                break;
            case MOV:
                debug("mov\t[%s: %d <- %s: %d]\n",
                        reg_name(dst), cpu->registers[dst], reg_name(src), cpu->registers[src]);
                cpu->registers[dst] = cpu->registers[src];
                break;
            case MOVI:
                debug("movi\t[%s: %d <- %d]\n",
                        reg_name(dst), cpu->registers[dst], src);
                cpu->registers[dst] = src;
                break;
            case MOVA:
                debug("mova\t[%s: %d <- %u]\n",
                        reg_name(dst), cpu->registers[dst], read_from_mem(cpu->memory, cpu->registers[src]));
                cpu->registers[dst] = read_from_mem(cpu->memory, cpu->registers[src]);
                break;
            case PUSH:
                debug("push\t[stack <- %s: %d]\n", reg_name(src), cpu->registers[src]);
                push(cpu, cpu->registers[src]);
                break;
            case PUSHI:
                debug("pushi\t[stack <- %d]\n", src);
                push(cpu, src);
                break;
            case POP:
                cpu->registers[dst] = pop(cpu);
                debug("pop\t[%s: %d <- stack]\n", reg_name(dst), cpu->registers[dst]);
                break;
            case ADD:
                debug("add\t[%s: %d + %s: %d",
                        reg_name(dst), cpu->registers[dst], reg_name(src), cpu->registers[src]);
                cpu->registers[dst] = cpu->registers[dst] + cpu->registers[src];
                if (cpu->registers[dst] == 0) {
                    set_equal(cpu);
                    debug(" (set flag 0)");
                } else {
                    set_not_equal(cpu);
                }
                debug("]\n");
                break;
            case ADDI:
                debug("addi\t[%s: %d + %d",
                        reg_name(dst), cpu->registers[dst], src);
                cpu->registers[dst] = cpu->registers[dst] + src;
                if (cpu->registers[dst] == 0) {
                    set_equal(cpu);
                    debug(" (set flag 0)");
                } else {
                    set_not_equal(cpu);
                }
                debug("]\n");
                break;
            case SUB:
                debug("sub\t[%s: %d - %s: %d",
                        reg_name(dst), cpu->registers[dst], reg_name(src), cpu->registers[src]);
                cpu->registers[dst] = cpu->registers[dst] - cpu->registers[src];
                if (cpu->registers[dst] == 0) {
                    set_equal(cpu);
                    debug(" (set flag 0)");
                } else {
                    set_not_equal(cpu);
                }
                debug("]\n");
                break;
            case SUBI:
                debug("subi\t[%s: %d - %d",
                        reg_name(dst), cpu->registers[dst], src);
                cpu->registers[dst] = cpu->registers[dst] - src;
                if (cpu->registers[dst] == 0) {
                    set_equal(cpu);
                    debug(" (set flag 0)");
                } else {
                    set_not_equal(cpu);
                }
                debug("]\n");
                break;
            case MUL:
                debug("mul\t[%s: %d * %s: %d]\n",
                        reg_name(dst), cpu->registers[dst], reg_name(src), cpu->registers[src]);
                cpu->registers[dst] = cpu->registers[dst] * cpu->registers[src];
                break;
            case JEQ:
                debug("jeq\t");
                if (cpu->flags == 1) {
                    debug("[jumping -> %s: %d]\n", reg_name(src), cpu->registers[src]);
                    jump(cpu, cpu->registers[src]);
                } else {
                    debug("[no jump]\n");
                }
                break;
            case JEQI:
                debug("jeqi\t");
                if (cpu->flags == 1) {
                    debug("[jumping -> %d]\n", src);
                    jump(cpu, src);
                } else {
                    debug("[no jump]\n");
                }
                break;
            case JMP:
                debug("jmp\t");
                debug("[jumping -> %s: %d\n", reg_name(src), cpu->registers[src]);
                jump(cpu, cpu->registers[src]);
                break;
            case CALL:
                debug("call\t[%s: %d]\n", reg_name(src), cpu->registers[src]);
                push(cpu, cpu->program_counter);
                jump(cpu, cpu->registers[src]);
                break;
            case CALLI:
                debug("calli\t[%d]\n", src);
                push(cpu, cpu->program_counter);
                jump(cpu, src);
                break;
            case RET:
                debug("ret\n");
                jump(cpu, pop(cpu));
                break;
        }
    }
}



void make_instruction(CPU *cpu, uint8_t op, uint8_t dst, uint16_t src)
{
    static int instruction_cnt = 0;
    cpu->memory[cpu->program_counter + instruction_cnt] = op;
    cpu->memory[cpu->program_counter + instruction_cnt + 1] = dst;
    cpu->memory[cpu->program_counter + instruction_cnt + 2] = (src & 0xFF00) >> 8;
    cpu->memory[cpu->program_counter + instruction_cnt + 3] = src & 0x00FF;
    instruction_cnt += 4;
}


void write_to_mem(uint8_t memory[], uint32_t location, uint32_t content)
{
    memory[location] |= (content & 0xFF000000) >> 24;
    memory[location + 1] |= (content & 0x00FF0000) >> 16;
    memory[location + 2] |= (content & 0x0000FF00) >> 8;
    memory[location + 3] |= (content & 0x000000FF);
}

uint32_t read_from_mem(uint8_t memory[], uint32_t location)
{
    return (uint32_t)memory[location] << 24 |
      (uint32_t)memory[location + 1] << 16 |
      (uint32_t)memory[location + 2] << 8  |
      (uint32_t)memory[location + 3];
}
