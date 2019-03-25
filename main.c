#include "cpu.h"
#include "debug.h"

int main()
{
    CPU *cpu = new_cpu();

    make_instruction(cpu, MOVI, EAX, 16);
    make_instruction(cpu, PUSH, 0, EAX);
    make_instruction(cpu, CALL, 0, EAX);
    make_instruction(cpu, HALT, 0, 0);
    make_instruction(cpu, MOVI, EAX, 10);
    make_instruction(cpu, RET, 0, 0);

    run(cpu);

    return 0;
}
