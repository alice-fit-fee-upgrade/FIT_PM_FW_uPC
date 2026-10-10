uint32_t pm_logical_channel_adc_delay_command(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x1f3a: { // rcall .+1784
        s->calls[s->call_depth++] = 7996;
        return 9780;
    }
    case 0x1f3c: { // brcs .-6
        return (pm_getflag(s, 0) == 1) ? 7992 : 7998;
    }
    case 0x1f3e: { // and r21, r21
        s->r[21] &= s->r[21];
        pm_nzv(s, s->r[21], false);
        return 8000;
    }
    case 0x1f40: { // brne .-10
        return (pm_getflag(s, 1) == 0) ? 7992 : 8002;
    }
    case 0x1f42: { // cpi r20, 0x0C
        pm_sub(s, s->r[20], 12, 0, false);
        return 8004;
    }
    case 0x1f44: { // brge .-14
        return (pm_getflag(s, 4) == 0) ? 7992 : 8006;
    }
    case 0x1f46: { // mov r22, r20
        s->r[22] = s->r[20];
        return 8008;
    }
    case 0x1f48: { // cpi r16, 0x20
        pm_sub(s, s->r[16], 32, 0, false);
        return 8010;
    }
    case 0x1f4a: { // brne .-20
        return (pm_getflag(s, 1) == 0) ? 7992 : 8012;
    }
    case 0x1f4c: { // rcall .+1766
        s->calls[s->call_depth++] = 8014;
        return 9780;
    }
    case 0x1f4e: { // brcs .-24
        return (pm_getflag(s, 0) == 1) ? 7992 : 8016;
    }
    case 0x1f50: { // cpi r16, 0x0D
        pm_sub(s, s->r[16], 13, 0, false);
        return 8018;
    }
    case 0x1f52: { // brne .-28
        return (pm_getflag(s, 1) == 0) ? 7992 : 8020;
    }
    case 0x1f54: { // and r21, r21
        s->r[21] &= s->r[21];
        pm_nzv(s, s->r[21], false);
        return 8022;
    }
    case 0x1f56: { // brmi .-32
        return (pm_getflag(s, 2) == 1) ? 7992 : 8024;
    }
    case 0x1f58: { // ldi r24, 0x4E
        s->r[24] = 78;
        return 8026;
    }
    case 0x1f5a: { // cpi r20, 0x21
        pm_sub(s, s->r[20], 33, 0, false);
        return 8028;
    }
    case 0x1f5c: { // cpc r21, r24
        pm_sub(s, s->r[21], s->r[24], pm_getflag(s, CARRY), true);
        return 8030;
    }
    case 0x1f5e: { // brge .-40
        return (pm_getflag(s, 4) == 0) ? 7992 : 8032;
    }
    case 0x1f60: { // rcall .+442
        s->calls[s->call_depth++] = 8034;
        return 8476;
    }
    case 0x1f62: { // brcc .+2
        return (pm_getflag(s, 0) == 0) ? 8038 : 8036;
    }
    case 0x1f64: { // ret
        return s->calls[--s->call_depth];
    }
    case 0x1f66: { // movw r16, r20
        uint16_t pair = pm_pointer(s, 20);
        pm_setpointer(s, 16, pair);
        return 8040;
    }
    case 0x1f68: { // mov r23, r22
        s->r[23] = s->r[22];
        return 8042;
    }
    case 0x1f6a: { // add r23, r23
        s->r[23] = pm_add(s, s->r[23], s->r[23], 0);
        return 8044;
    }
    case 0x1f6c: { // add r23, r23
        s->r[23] = pm_add(s, s->r[23], s->r[23], 0);
        return 8046;
    }
    case 0x1f6e: { // ldi r18, 0x83
        s->r[18] = 131;
        return 8048;
    }
    case 0x1f70: { // add r18, r23
        s->r[18] = pm_add(s, s->r[18], s->r[23], 0);
        return 8050;
    }
    case 0x1f72: { // cli
        pm_irq(s, false);
        return 8052;
    }
    case 0x1f74: { // rcall .+920
        s->calls[s->call_depth++] = 8054;
        return 8974;
    }
    case 0x1f76: { // sei
        pm_irq(s, true);
        return 8056;
    }
    case 0x1f78: { // ldi r28, 0xCF
        s->r[28] = 207;
        return 8058;
    }
    case 0x1f7a: { // ldi r29, 0x21
        s->r[29] = 33;
        return 8060;
    }
    case 0x1f7c: { // add r23, r23
        s->r[23] = pm_add(s, s->r[23], s->r[23], 0);
        return 8062;
    }
    case 0x1f7e: { // subi r23, 0xFA
        s->r[23] = pm_sub(s, s->r[23], 250, 0, false);
        return 8064;
    }
    case 0x1f80: { // eor r11, r11
        s->r[11] ^= s->r[11];
        pm_nzv(s, s->r[11], false);
        return 8066;
    }
    case 0x1f82: { // add r28, r23
        s->r[28] = pm_add(s, s->r[28], s->r[23], 0);
        return 8068;
    }
    case 0x1f84: { // adc r29, r11
        s->r[29] = pm_add(s, s->r[29], s->r[11], pm_getflag(s, CARRY));
        return 8070;
    }
    case 0x1f86: { // st Y+, r20
        uint16_t address = pm_pointer(s, 28) + 0;
        pm_setpointer(s, 28, address + 1);
        pm_write(s, address, s->r[20]);
        return 8072;
    }
    case 0x1f88: { // st Y, r21
        uint16_t address = pm_pointer(s, 28) + 0;
        pm_write(s, address, s->r[21]);
        return 8074;
    }
    case 0x1f8a: { // rcall .+324
        s->calls[s->call_depth++] = 8076;
        return 8400;
    }
    case 0x1f8c: { // rjmp .+84
        return 8162;
    }
    default: return UINT32_MAX;
    }
}
