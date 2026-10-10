uint32_t pm_logical_pll_console_reset(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x1a3e: { // rcall .+3580
        s->calls[s->call_depth++] = 6720;
        return 10300;
    }
    case 0x1a40: { // cpi r16, 0x0D
        pm_sub(s, s->r[16], 13, 0, false);
        return 6722;
    }
    case 0x1a42: { // brne .-8
        return (pm_getflag(s, 1) == 0) ? 6716 : 6724;
    }
    case 0x1a44: { // ldi r30, 0x3E
        s->r[30] = 62;
        return 6726;
    }
    case 0x1a46: { // ldi r31, 0x29
        s->r[31] = 41;
        return 6728;
    }
    case 0x1a48: { // ldi r20, 0x09
        s->r[20] = 9;
        return 6730;
    }
    case 0x1a4a: { // cli
        pm_irq(s, false);
        return 6732;
    }
    case 0x1a4c: { // lpm r16, Z+
        uint16_t address = pm_pointer(s, 30) + 0;
        pm_setpointer(s, 30, address + 1);
        s->r[16] = pm_golden_flash[address];
        return 6734;
    }
    case 0x1a4e: { // lpm r17, Z+
        uint16_t address = pm_pointer(s, 30) + 0;
        pm_setpointer(s, 30, address + 1);
        s->r[17] = pm_golden_flash[address];
        return 6736;
    }
    case 0x1a50: { // lpm r18, Z+
        uint16_t address = pm_pointer(s, 30) + 0;
        pm_setpointer(s, 30, address + 1);
        s->r[18] = pm_golden_flash[address];
        return 6738;
    }
    case 0x1a52: { // lpm r19, Z+
        uint16_t address = pm_pointer(s, 30) + 0;
        pm_setpointer(s, 30, address + 1);
        s->r[19] = pm_golden_flash[address];
        return 6740;
    }
    case 0x1a54: { // rcall .+2608
        s->calls[s->call_depth++] = 6742;
        return 9350;
    }
    case 0x1a56: { // dec r20
        s->r[20]--;
        pm_nzv(s, s->r[20], s->r[20] == 127);
        return 6744;
    }
    case 0x1a58: { // brne .-14
        return (pm_getflag(s, 1) == 0) ? 6732 : 6746;
    }
    case 0x1a5a: { // sei
        pm_irq(s, true);
        return 6748;
    }
    case 0x1a5c: { // ldi r30, 0x98
        s->r[30] = 152;
        return 6750;
    }
    case 0x1a5e: { // ldi r31, 0x29
        s->r[31] = 41;
        return 6752;
    }
    case 0x1a60: { // rcall .+3524
        s->calls[s->call_depth++] = 6754;
        return 10278;
    }
    case 0x1a62: { // ret
        return s->calls[--s->call_depth];
    }
    default: return UINT32_MAX;
    }
}
