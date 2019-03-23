#include "cpu.h"

int main()
{
    CPU *cpu = new_cpu();
    cpu->registers[EAX] = 4;
    cpu->registers[EBX] = -4;
    make_instruction(cpu, HALT, 0, 0);
    make_instruction(cpu, ADD, EAX, EBX);
    make_instruction(cpu, JEQ, 0, 0);
    cpu->program_counter += 4;
    run(cpu);

    return 0;
}
