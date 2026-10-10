uint32_t pm_logical_console_raw_byte_entry(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x2836: { // push r19
        s->stack[s->depth++] = s->r[19];
        return 10296;
    }
    case 0x2838: { // ldi r19, 0x01
        s->r[19] = 1;
        return 10298;
    }
    case 0x283a: { // rjmp .+4
        return 10304;
    }
    default: return UINT32_MAX;
    }
}
