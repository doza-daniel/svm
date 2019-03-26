#include <stdarg.h>

#include "debug.h"
#include "cpu.h"


int debug(const char *fmt, ...)
{
    int ret = 0;
#ifdef DEBUG
    va_list args;
    va_start(args, fmt);
    ret = vprintf(fmt, args);
    va_end(args);
#endif
    return ret;
}

char *reg_name(int r)
{
    switch(r) {
    case EAX:
        return "EAX";
    case EBX:
        return "EBX";
    case ECX:
        return "ECX";
    case EDX:
        return "EDX";
    case ESI:
        return "ESI";
    case EDI:
        return "EDI";
    case ESP:
        return "ESP";
    case EBP:
        return "EBP";
    }
    return "error";
}
