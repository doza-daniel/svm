#include "cpu.h"
#include "debug.h"

int main()
{
    CPU *cpu = new_cpu();
    //make_instruction(cpu, PUSH, 0, EBP);
    //make_instruction(cpu, MOVC, EBX, 16);
    //make_instruction(cpu, JMP, 0, EBX);

    //make_instruction(cpu, HALT, 0, 0);
    //make_instruction(cpu, ADD, EAX, EBX);

    //make_instruction(cpu, MOVC, EAX, 10);
    //make_instruction(cpu, MOVC, EBX, 10);

    //make_instruction(cpu, SUB, EAX, EBX);
    //make_instruction(cpu, MOVC, EAX, 12);
    //make_instruction(cpu, JEQ, 0, EAX);



    //memory_layout(cpu->memory);
    //make_instruction(cpu, PUSH, 0, EAX);
    //make_instruction(cpu, CALL, 0, 14);



    // 0
    make_instruction(cpu, MOVC, EBX, 5);
    // 4
    make_instruction(cpu, MOVC, EAX, 5);
    // 8
    make_instruction(cpu, SUB, EBX, EAX);
    // 12
    make_instruction(cpu, MOVC, EAX, 24);
    // 16
    make_instruction(cpu, JEQ, 0, EAX);
    // 20
    make_instruction(cpu, PUSH, 0, EAX);
    // 24
    make_instruction(cpu, PUSH, 0, EBX);
    // 28
    make_instruction(cpu, HALT, 0, 0);

    run(cpu);

    for (int i = UINT32_MAX - 1; i > UINT32_MAX - 5; i--) {
        printf("%d\n", ((int32_t*)cpu->memory)[i]);
    }
    return 0;
}
