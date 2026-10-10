uint32_t pm_logical_console_fixed3_entry(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x273e: { // push r20
        s->stack[s->depth++] = s->r[20];
        return 10048;
    }
    case 0x2740: { // ldi r20, 0x03
        s->r[20] = 3;
        return 10050;
    }
    case 0x2742: { // sec
        pm_flag(s, CARRY, true);
        return 10052;
    }
    case 0x2744: { // rjmp .+14
        return 10068;
    }
    default: return UINT32_MAX;
    }
}
