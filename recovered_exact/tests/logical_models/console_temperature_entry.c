uint32_t pm_logical_console_temperature_entry(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x2736: { // push r20
        s->stack[s->depth++] = s->r[20];
        return 10040;
    }
    case 0x2738: { // ldi r20, 0x01
        s->r[20] = 1;
        return 10042;
    }
    case 0x273a: { // sec
        pm_flag(s, CARRY, true);
        return 10044;
    }
    case 0x273c: { // rjmp .+22
        return 10068;
    }
    default: return UINT32_MAX;
    }
}
