uint32_t pm_logical_trigger_settings_command(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x1d92: { // rcall .+2728
        s->calls[s->call_depth++] = 7572;
        return 10300;
    }
    case 0x1d94: { // cpi r16, 0x20
        pm_sub(s, s->r[16], 32, 0, false);
        return 7574;
    }
    case 0x1d96: { // brne .-56
        return (pm_getflag(s, 1) == 0) ? 7520 : 7576;
    }
    case 0x1d98: { // rcall .+2202
        s->calls[s->call_depth++] = 7578;
        return 9780;
    }
    case 0x1d9a: { // brcs .-60
        return (pm_getflag(s, 0) == 1) ? 7520 : 7580;
    }
    case 0x1d9c: { // and r21, r21
        s->r[21] &= s->r[21];
        pm_nzv(s, s->r[21], false);
        return 7582;
    }
    case 0x1d9e: { // brne .-64
        return (pm_getflag(s, 1) == 0) ? 7520 : 7584;
    }
    case 0x1da0: { // rcall .+890
        s->calls[s->call_depth++] = 7586;
        return 8476;
    }
    case 0x1da2: { // brcc .+2
        return (pm_getflag(s, 0) == 0) ? 7590 : 7588;
    }
    case 0x1da4: { // ret
        return s->calls[--s->call_depth];
    }
    case 0x1da6: { // sts 0x222F, r20
        uint16_t address = 8751;
        pm_write(s, address, s->r[20]);
        return 7594;
    }
    case 0x1daa: { // ldi r18, 0x00
        s->r[18] = 0;
        return 7596;
    }
    case 0x1dac: { // movw r16, r20
        uint16_t pair = pm_pointer(s, 20);
        pm_setpointer(s, 16, pair);
        return 7598;
    }
    case 0x1dae: { // cli
        pm_irq(s, false);
        return 7600;
    }
    case 0x1db0: { // rcall .+1372
        s->calls[s->call_depth++] = 7602;
        return 8974;
    }
    case 0x1db2: { // sei
        pm_irq(s, true);
        return 7604;
    }
    case 0x1db4: { // rjmp .+556
        return 8162;
    }
    default: return UINT32_MAX;
    }
}
