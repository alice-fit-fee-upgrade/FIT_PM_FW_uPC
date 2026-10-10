uint32_t pm_logical_channel_tdc_values_command(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x1e2e: { // rcall .+2052
        s->calls[s->call_depth++] = 7728;
        return 9780;
    }
    case 0x1e30: { // brcs .+72
        return (pm_getflag(s, 0) == 1) ? 7802 : 7730;
    }
    case 0x1e32: { // and r21, r21
        s->r[21] &= s->r[21];
        pm_nzv(s, s->r[21], false);
        return 7732;
    }
    case 0x1e34: { // brne .+68
        return (pm_getflag(s, 1) == 0) ? 7802 : 7734;
    }
    case 0x1e36: { // cpi r20, 0x0C
        pm_sub(s, s->r[20], 12, 0, false);
        return 7736;
    }
    case 0x1e38: { // brge .+64
        return (pm_getflag(s, 4) == 0) ? 7802 : 7738;
    }
    case 0x1e3a: { // mov r22, r20
        s->r[22] = s->r[20];
        return 7740;
    }
    case 0x1e3c: { // cpi r16, 0x20
        pm_sub(s, s->r[16], 32, 0, false);
        return 7742;
    }
    case 0x1e3e: { // brne .+58
        return (pm_getflag(s, 1) == 0) ? 7802 : 7744;
    }
    case 0x1e40: { // rcall .+2034
        s->calls[s->call_depth++] = 7746;
        return 9780;
    }
    case 0x1e42: { // brcs .+54
        return (pm_getflag(s, 0) == 1) ? 7802 : 7748;
    }
    case 0x1e44: { // cpi r16, 0x0D
        pm_sub(s, s->r[16], 13, 0, false);
        return 7750;
    }
    case 0x1e46: { // brne .+50
        return (pm_getflag(s, 1) == 0) ? 7802 : 7752;
    }
    case 0x1e48: { // and r21, r21
        s->r[21] &= s->r[21];
        pm_nzv(s, s->r[21], false);
        return 7754;
    }
    case 0x1e4a: { // brmi .+46
        return (pm_getflag(s, 2) == 1) ? 7802 : 7756;
    }
    case 0x1e4c: { // ldi r24, 0x10
        s->r[24] = 16;
        return 7758;
    }
    case 0x1e4e: { // cpi r20, 0x00
        pm_sub(s, s->r[20], 0, 0, false);
        return 7760;
    }
    case 0x1e50: { // cpc r21, r24
        pm_sub(s, s->r[21], s->r[24], pm_getflag(s, CARRY), true);
        return 7762;
    }
    case 0x1e52: { // brge .+38
        return (pm_getflag(s, 4) == 0) ? 7802 : 7764;
    }
    case 0x1e54: { // rcall .+710
        s->calls[s->call_depth++] = 7766;
        return 8476;
    }
    case 0x1e56: { // brcc .+2
        return (pm_getflag(s, 0) == 0) ? 7770 : 7768;
    }
    case 0x1e58: { // ret
        return s->calls[--s->call_depth];
    }
    case 0x1e5a: { // ldi r28, 0xB7
        s->r[28] = 183;
        return 7772;
    }
    case 0x1e5c: { // ldi r29, 0x21
        s->r[29] = 33;
        return 7774;
    }
    case 0x1e5e: { // mov r23, r22
        s->r[23] = s->r[22];
        return 7776;
    }
    case 0x1e60: { // add r23, r23
        s->r[23] = pm_add(s, s->r[23], s->r[23], 0);
        return 7778;
    }
    case 0x1e62: { // eor r11, r11
        s->r[11] ^= s->r[11];
        pm_nzv(s, s->r[11], false);
        return 7780;
    }
    case 0x1e64: { // add r28, r23
        s->r[28] = pm_add(s, s->r[28], s->r[23], 0);
        return 7782;
    }
    case 0x1e66: { // adc r29, r11
        s->r[29] = pm_add(s, s->r[29], s->r[11], pm_getflag(s, CARRY));
        return 7784;
    }
    case 0x1e68: { // st Y+, r20
        uint16_t address = pm_pointer(s, 28) + 0;
        pm_setpointer(s, 28, address + 1);
        pm_write(s, address, s->r[20]);
        return 7786;
    }
    case 0x1e6a: { // st Y, r21
        uint16_t address = pm_pointer(s, 28) + 0;
        pm_write(s, address, s->r[21]);
        return 7788;
    }
    case 0x1e6c: { // ldi r18, 0x01
        s->r[18] = 1;
        return 7790;
    }
    case 0x1e6e: { // add r18, r22
        s->r[18] = pm_add(s, s->r[18], s->r[22], 0);
        return 7792;
    }
    case 0x1e70: { // movw r16, r20
        uint16_t pair = pm_pointer(s, 20);
        pm_setpointer(s, 16, pair);
        return 7794;
    }
    case 0x1e72: { // cli
        pm_irq(s, false);
        return 7796;
    }
    case 0x1e74: { // rcall .+1176
        s->calls[s->call_depth++] = 7798;
        return 8974;
    }
    case 0x1e76: { // sei
        pm_irq(s, true);
        return 7800;
    }
    case 0x1e78: { // rjmp .+360
        return 8162;
    }
    case 0x1e7a: { // rjmp .-2740
        return 5064;
    }
    default: return UINT32_MAX;
    }
}
