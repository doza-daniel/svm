#include "cpu.h"
#include "debug.h"

int main()
{
    CPU *cpu = new_cpu();

    make_instruction(cpu, PUSH, 0, EBP);
    make_instruction(cpu, MOV,  EBP, ESP);

    make_instruction(cpu, PUSHI, 0, 5);

    make_instruction(cpu, CALLI, 0, 20);
    make_instruction(cpu, HALT, 0, 0);


    // factorial
    make_instruction(cpu, PUSH, 0, EBP);
    make_instruction(cpu, MOV, EBP, ESP);

    make_instruction(cpu, MOV, EDI, EBP);
    make_instruction(cpu, ADDI, EDI, 8);
    make_instruction(cpu, MOVA, EBX, EDI);

    make_instruction(cpu, MOV, EDX, EBX);

    // jump to ret if arg == 1
    make_instruction(cpu, SUBI, EDX, 1);
    make_instruction(cpu, JEQI, 0, 64);

    // else fact(arg - 1)
    make_instruction(cpu, PUSH, 0, EDX);
    make_instruction(cpu, CALLI, 0, 20);

    make_instruction(cpu, MUL, EAX, EBX);

    make_instruction(cpu, MOV, ESP, EBP);
    make_instruction(cpu, POP, EBP, 0);
    make_instruction(cpu, RET, 0, 0);






    run(cpu);

    //for(uint32_t i = 0; i < 40; i++) {
    //    print_bits(fetch(cpu));
    //}

    printf("%d", cpu->registers[EAX]);

    return 0;
}
