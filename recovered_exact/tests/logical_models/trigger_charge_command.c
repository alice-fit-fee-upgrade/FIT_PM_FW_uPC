uint32_t pm_logical_trigger_charge_command(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x1d62: { // rcall .+2776
        s->calls[s->call_depth++] = 7524;
        return 10300;
    }
    case 0x1d64: { // cpi r16, 0x20
        pm_sub(s, s->r[16], 32, 0, false);
        return 7526;
    }
    case 0x1d66: { // brne .-8
        return (pm_getflag(s, 1) == 0) ? 7520 : 7528;
    }
    case 0x1d68: { // rcall .+2250
        s->calls[s->call_depth++] = 7530;
        return 9780;
    }
    case 0x1d6a: { // brcs .-124
        return (pm_getflag(s, 0) == 1) ? 7408 : 7532;
    }
    case 0x1d6c: { // and r21, r21
        s->r[21] &= s->r[21];
        pm_nzv(s, s->r[21], false);
        return 7534;
    }
    case 0x1d6e: { // brmi .-128
        return (pm_getflag(s, 2) == 1) ? 7408 : 7536;
    }
    case 0x1d70: { // ldi r26, 0x10
        s->r[26] = 16;
        return 7538;
    }
    case 0x1d72: { // cpi r20, 0x00
        pm_sub(s, s->r[20], 0, 0, false);
        return 7540;
    }
    case 0x1d74: { // cpc r21, r26
        pm_sub(s, s->r[21], s->r[26], pm_getflag(s, CARRY), true);
        return 7542;
    }
    case 0x1d76: { // brge .-24
        return (pm_getflag(s, 4) == 0) ? 7520 : 7544;
    }
    case 0x1d78: { // rcall .+930
        s->calls[s->call_depth++] = 7546;
        return 8476;
    }
    case 0x1d7a: { // brcc .+2
        return (pm_getflag(s, 0) == 0) ? 7550 : 7548;
    }
    case 0x1d7c: { // ret
        return s->calls[--s->call_depth];
    }
    case 0x1d7e: { // sts 0x2230, r20
        uint16_t address = 8752;
        pm_write(s, address, s->r[20]);
        return 7554;
    }
    case 0x1d82: { // sts 0x2231, r21
        uint16_t address = 8753;
        pm_write(s, address, s->r[21]);
        return 7558;
    }
    case 0x1d86: { // ldi r18, 0x3D
        s->r[18] = 61;
        return 7560;
    }
    case 0x1d88: { // movw r16, r20
        uint16_t pair = pm_pointer(s, 20);
        pm_setpointer(s, 16, pair);
        return 7562;
    }
    case 0x1d8a: { // cli
        pm_irq(s, false);
        return 7564;
    }
    case 0x1d8c: { // rcall .+1408
        s->calls[s->call_depth++] = 7566;
        return 8974;
    }
    case 0x1d8e: { // sei
        pm_irq(s, true);
        return 7568;
    }
    case 0x1d90: { // rjmp .+592
        return 8162;
    }
    default: return UINT32_MAX;
    }
}
