uint32_t pm_logical_console_flash_message(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x2826: { // push r16
        s->stack[s->depth++] = s->r[16];
        return 10280;
    }
    case 0x2828: { // lpm r16, Z+
        uint16_t address = pm_pointer(s, 30) + 0;
        pm_setpointer(s, 30, address + 1);
        s->r[16] = pm_golden_flash[address];
        return 10282;
    }
    case 0x282a: { // and r16, r16
        s->r[16] &= s->r[16];
        pm_nzv(s, s->r[16], false);
        return 10284;
    }
    case 0x282c: { // breq .+4
        return (pm_getflag(s, 1) == 1) ? 10290 : 10286;
    }
    case 0x282e: { // rcall .+124
        s->calls[s->call_depth++] = 10288;
        return 10412;
    }
    case 0x2830: { // rjmp .-10
        return 10280;
    }
    case 0x2832: { // pop r16
        s->r[16] = s->stack[--s->depth];
        return 10292;
    }
    case 0x2834: { // ret
        return s->calls[--s->call_depth];
    }
    default: return UINT32_MAX;
    }
}
