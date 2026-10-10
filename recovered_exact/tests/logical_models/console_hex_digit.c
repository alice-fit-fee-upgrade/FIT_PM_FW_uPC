uint32_t pm_logical_console_hex_digit(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x2720: { // andi r16, 0x0F
        s->r[16] &= 15;
        pm_nzv(s, s->r[16], false);
        return 10018;
    }
    case 0x2722: { // subi r16, 0xD0
        s->r[16] = pm_sub(s, s->r[16], 208, 0, false);
        return 10020;
    }
    case 0x2724: { // cpi r16, 0x3A
        pm_sub(s, s->r[16], 58, 0, false);
        return 10022;
    }
    case 0x2726: { // brcs .+2
        return (pm_getflag(s, 0) == 1) ? 10026 : 10024;
    }
    case 0x2728: { // subi r16, 0xF9
        s->r[16] = pm_sub(s, s->r[16], 249, 0, false);
        return 10026;
    }
    case 0x272a: { // rcall .+384
        s->calls[s->call_depth++] = 10028;
        return 10412;
    }
    case 0x272c: { // ret
        return s->calls[--s->call_depth];
    }
    default: return UINT32_MAX;
    }
}
