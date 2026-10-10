uint32_t pm_logical_console_fixed2_entry(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x272e: { // push r20
        s->stack[s->depth++] = s->r[20];
        return 10032;
    }
    case 0x2730: { // ldi r20, 0x02
        s->r[20] = 2;
        return 10034;
    }
    case 0x2732: { // sec
        pm_flag(s, CARRY, true);
        return 10036;
    }
    case 0x2734: { // rjmp .+30
        return 10068;
    }
    default: return UINT32_MAX;
    }
}
