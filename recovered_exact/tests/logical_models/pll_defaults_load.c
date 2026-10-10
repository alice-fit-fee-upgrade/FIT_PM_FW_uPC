uint32_t pm_logical_pll_defaults_load(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x0ddc: { // push r16
        s->stack[s->depth++] = s->r[16];
        return 3550;
    }
    case 0x0dde: { // push r17
        s->stack[s->depth++] = s->r[17];
        return 3552;
    }
    case 0x0de0: { // push r18
        s->stack[s->depth++] = s->r[18];
        return 3554;
    }
    case 0x0de2: { // push r19
        s->stack[s->depth++] = s->r[19];
        return 3556;
    }
    case 0x0de4: { // push r20
        s->stack[s->depth++] = s->r[20];
        return 3558;
    }
    case 0x0de6: { // push r30
        s->stack[s->depth++] = s->r[30];
        return 3560;
    }
    case 0x0de8: { // push r31
        s->stack[s->depth++] = s->r[31];
        return 3562;
    }
    case 0x0dea: { // ldi r30, 0x16
        s->r[30] = 22;
        return 3564;
    }
    case 0x0dec: { // ldi r31, 0x29
        s->r[31] = 41;
        return 3566;
    }
    case 0x0dee: { // ldi r20, 0x0A
        s->r[20] = 10;
        return 3568;
    }
    case 0x0df0: { // lpm r16, Z+
        uint16_t address = pm_pointer(s, 30) + 0;
        pm_setpointer(s, 30, address + 1);
        s->r[16] = pm_golden_flash[address];
        return 3570;
    }
    case 0x0df2: { // lpm r17, Z+
        uint16_t address = pm_pointer(s, 30) + 0;
        pm_setpointer(s, 30, address + 1);
        s->r[17] = pm_golden_flash[address];
        return 3572;
    }
    case 0x0df4: { // lpm r18, Z+
        uint16_t address = pm_pointer(s, 30) + 0;
        pm_setpointer(s, 30, address + 1);
        s->r[18] = pm_golden_flash[address];
        return 3574;
    }
    case 0x0df6: { // lpm r19, Z+
        uint16_t address = pm_pointer(s, 30) + 0;
        pm_setpointer(s, 30, address + 1);
        s->r[19] = pm_golden_flash[address];
        return 3576;
    }
    case 0x0df8: { // cli
        pm_irq(s, false);
        return 3578;
    }
    case 0x0dfa: { // call 0x2486
        s->calls[s->call_depth++] = 3582;
        return 9350;
    }
    case 0x0dfe: { // sei
        pm_irq(s, true);
        return 3584;
    }
    case 0x0e00: { // dec r20
        s->r[20]--;
        pm_nzv(s, s->r[20], s->r[20] == 127);
        return 3586;
    }
    case 0x0e02: { // brne .-20
        return (pm_getflag(s, 1) == 0) ? 3568 : 3588;
    }
    case 0x0e04: { // ldi r16, 0x0A
        s->r[16] = 10;
        return 3590;
    }
    case 0x0e06: { // sts 0x06A9, r16
        uint16_t address = 1705;
        pm_write(s, address, s->r[16]);
        return 3594;
    }
    case 0x0e0a: { // ldi r16, 0x20
        s->r[16] = 32;
        return 3596;
    }
    case 0x0e0c: { // sts 0x0626, r16
        uint16_t address = 1574;
        pm_write(s, address, s->r[16]);
        return 3600;
    }
    case 0x0e10: { // pop r31
        s->r[31] = s->stack[--s->depth];
        return 3602;
    }
    case 0x0e12: { // pop r30
        s->r[30] = s->stack[--s->depth];
        return 3604;
    }
    case 0x0e14: { // pop r20
        s->r[20] = s->stack[--s->depth];
        return 3606;
    }
    case 0x0e16: { // pop r19
        s->r[19] = s->stack[--s->depth];
        return 3608;
    }
    case 0x0e18: { // pop r18
        s->r[18] = s->stack[--s->depth];
        return 3610;
    }
    case 0x0e1a: { // pop r17
        s->r[17] = s->stack[--s->depth];
        return 3612;
    }
    case 0x0e1c: { // pop r16
        s->r[16] = s->stack[--s->depth];
        return 3614;
    }
    case 0x0e1e: { // ret
        return s->calls[--s->call_depth];
    }
    default: return UINT32_MAX;
    }
}
