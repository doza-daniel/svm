#include "cpu.h"

int main()
{
    CPU *cpu = new_cpu();

    make_instruction(cpu, PUSH, 0, EBP);
    make_instruction(cpu, MOV,  EBP, ESP);

    make_instruction(cpu, PUSHI, 0, 10);
    // call fib(10)
    make_instruction(cpu, CALLI, 0, 20);

    make_instruction(cpu, HALT, 0, 0);

    // fib(n)
    make_instruction(cpu, PUSH, 0, EBP);
    make_instruction(cpu, MOV, EBP, ESP);

    // read first argument into EAX
    make_instruction(cpu, MOV, EDI, EBP);
    make_instruction(cpu, ADDI, EDI, 8);
    make_instruction(cpu, MOVA, EAX, EDI);

    // save argument as tmp value in EBX
    make_instruction(cpu, MOV, EBX, EAX);

    // if argument was equal to 0, jump to return
    make_instruction(cpu, SUBI, EBX, 0);
    make_instruction(cpu, JEQI, 0, 116);

    // save argument as tmp value in EBX
    make_instruction(cpu, MOV, EBX, EAX);

    // if argument was equal to 1, jump to return
    make_instruction(cpu, SUBI, EBX, 1);
    make_instruction(cpu, JEQI, 0, 116);

    // store (n-1) -> EDX
    make_instruction(cpu, MOV, EDX, EBX);
    // save local (n-2)
    make_instruction(cpu, SUBI, EBX, 1);
    make_instruction(cpu, PUSH, 0, EBX);

    // fib(n-1)
    make_instruction(cpu, PUSH, 0, EDX);
    make_instruction(cpu, CALLI, 0, 20);


    // pop last argument
    make_instruction(cpu, POP, 0, EDX);
    // pop n-2
    make_instruction(cpu, POP, 0, EDX);
    // push result of fib(n-1) as local var
    make_instruction(cpu, PUSH, 0, EAX);

    // fib(n-2)
    make_instruction(cpu, PUSH, 0, EDX);
    make_instruction(cpu, CALLI, 0, 20);

    // pop last argument
    make_instruction(cpu, POP, 0, EDX);
    // pop fib(n-1) local var
    make_instruction(cpu, POP, 0, EDX);
    // fib(n-1) + fib(n-2)
    make_instruction(cpu, ADD, EAX, EDX);


    make_instruction(cpu, MOV, ESP, EBP);
    make_instruction(cpu, POP, EBP, 0);
    make_instruction(cpu, RET, 0, 0);

    run(cpu);

    printf("%d\n", cpu->registers[EAX]);

    return 0;
}
