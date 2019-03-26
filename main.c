#include "cpu.h"

int main()
{
    CPU *cpu = new_cpu();

    make_instruction(cpu, PUSH, 0, EBP);
    make_instruction(cpu, MOV,  EBP, ESP);

    make_instruction(cpu, PUSHI, 0, 10);
    // call factorial(10)
    make_instruction(cpu, CALLI, 0, 20);

    make_instruction(cpu, HALT, 0, 0);

    // function: factorial(n)
    // prepare stack frame
    make_instruction(cpu, PUSH, 0, EBP);
    make_instruction(cpu, MOV, EBP, ESP);

    // read first argument into EAX
    make_instruction(cpu, MOV, EDI, EBP);
    make_instruction(cpu, ADDI, EDI, 8);
    make_instruction(cpu, MOVA, EAX, EDI);

    // save argument as tmp value in EBX
    make_instruction(cpu, MOV, EBX, EAX);

    // if argument was equal to 1, jump to return
    make_instruction(cpu, SUBI, EBX, 1);
    make_instruction(cpu, JEQI, 0, 76);
    // else
    // save current argument on stack
    make_instruction(cpu, PUSH, 0, EAX);
    // call factorial(n-1)
    make_instruction(cpu, PUSH, 0, EBX);
    make_instruction(cpu, CALLI, 0, 20);

    // pop the argument for previous call
    make_instruction(cpu, POP, EBX, 0);
    // pop the local variable we saved
    make_instruction(cpu, POP, EBX, 0);
    // n * factorial(n-1) where EAX = factorial(n-1) and EBX = n
    make_instruction(cpu, MUL, EAX, EBX);

    // reset stack frame and return
    make_instruction(cpu, MOV, ESP, EBP);
    make_instruction(cpu, POP, EBP, 0);
    make_instruction(cpu, RET, 0, 0);


    run(cpu);

    printf("%d\n", cpu->registers[EAX]);

    return 0;
}
