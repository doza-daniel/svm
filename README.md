# Simple Virtual Machine

## Registers
There is 8 registers in total, indexed 0 to 7. Registers [0..5] are general purpose registers.
Register 6 is represents stack pointer (ESP). Register 7 represent base pointer (EBP).

## Flags
There is 32 bits provided for specific flags. At the moment, only equality bit is used, which is
the rightmost bit.

## Instruction format
Each instruction is 32 bits long and is divided in three sections:

  8 bits   8 bits       16 bits
|   op   |   dst  |       src      |

First byte is operation code. Second byte is the number of the destination register.
The last 16 bits represent either an immediate value or source register number.

## Operations
| opname | code | description|
|--------|------|------------|
| MOV | 0x0 | copy the value from SRC register to DST. |
| MOVI | 0x1 | insert the immediate SRC value to register DST. |
| MOVA | 0x2 | copy the value from SRC memory address to register DST. |
| PUSH | 0x3 | push the value in SRC register to the top of the stack. |
| PUSHI | 0x4 | push the immediate SRC value to the top of the stack. |
| POP | 0x5 | pop the value from the top of the stack to register DST. |
| ADD | 0x6 | add values in registers SRC and DST and store in register DST. |
| ADDI | 0x7 | add the immediate SRC value to register DST and store in register DST. |
| SUB | 0x8 | subtract register SRC from register DST and store in register DST. |
| SUBI | 0x9 | subtract immediate value SRC from register DST and store in register DST. |
| MUL | 0xA | multiply registers SRC and DST and store in DST. |
| JEQ | 0xB | if equal flag is set, jump to the location in SRC register. |
| JEQI | 0xC | if equal flag is set, jump to the location given as immediate value SRC. |
| JMP | 0xD | unconditional jump to the location given in SRC register. |
| CALL | 0xE | push the current instruction pointer value to stack and jump to the location given in SRC regiser. |
| CALLI | 0xF | push the current instruction pointer value to stack and jump to the location given as immediate SRC value. |
| RET | 0x10 | pop the top of the stack and jump to that location. |
| HALT | 0x11 | terminate the processor. |


