uint32_t pm_logical_channel_threshold_calibration_command(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x1e7c: { // rcall .+1974
        s->calls[s->call_depth++] = 7806;
        return 9780;
    }
    case 0x1e7e: { // brcs .-6
        return (pm_getflag(s, 0) == 1) ? 7802 : 7808;
    }
    case 0x1e80: { // and r21, r21
        s->r[21] &= s->r[21];
        pm_nzv(s, s->r[21], false);
        return 7810;
    }
    case 0x1e82: { // brne .-10
        return (pm_getflag(s, 1) == 0) ? 7802 : 7812;
    }
    case 0x1e84: { // cpi r20, 0x0C
        pm_sub(s, s->r[20], 12, 0, false);
        return 7814;
    }
    case 0x1e86: { // brge .-14
        return (pm_getflag(s, 4) == 0) ? 7802 : 7816;
    }
    case 0x1e88: { // mov r22, r20
        s->r[22] = s->r[20];
        return 7818;
    }
    case 0x1e8a: { // cpi r16, 0x20
        pm_sub(s, s->r[16], 32, 0, false);
        return 7820;
    }
    case 0x1e8c: { // brne .-20
        return (pm_getflag(s, 1) == 0) ? 7802 : 7822;
    }
    case 0x1e8e: { // rcall .+1956
        s->calls[s->call_depth++] = 7824;
        return 9780;
    }
    case 0x1e90: { // brcs .-24
        return (pm_getflag(s, 0) == 1) ? 7802 : 7826;
    }
    case 0x1e92: { // cpi r16, 0x0D
        pm_sub(s, s->r[16], 13, 0, false);
        return 7828;
    }
    case 0x1e94: { // brne .-28
        return (pm_getflag(s, 1) == 0) ? 7802 : 7830;
    }
    case 0x1e96: { // and r21, r21
        s->r[21] &= s->r[21];
        pm_nzv(s, s->r[21], false);
        return 7832;
    }
    case 0x1e98: { // brmi .-32
        return (pm_getflag(s, 2) == 1) ? 7802 : 7834;
    }
    case 0x1e9a: { // ldi r24, 0x0F
        s->r[24] = 15;
        return 7836;
    }
    case 0x1e9c: { // cpi r20, 0xA1
        pm_sub(s, s->r[20], 161, 0, false);
        return 7838;
    }
    case 0x1e9e: { // cpc r21, r24
        pm_sub(s, s->r[21], s->r[24], pm_getflag(s, CARRY), true);
        return 7840;
    }
    case 0x1ea0: { // brge .-40
        return (pm_getflag(s, 4) == 0) ? 7802 : 7842;
    }
    case 0x1ea2: { // rcall .+632
        s->calls[s->call_depth++] = 7844;
        return 8476;
    }
    case 0x1ea4: { // brcc .+2
        return (pm_getflag(s, 0) == 0) ? 7848 : 7846;
    }
    case 0x1ea6: { // ret
        return s->calls[--s->call_depth];
    }
    case 0x1ea8: { // movw r16, r20
        uint16_t pair = pm_pointer(s, 20);
        pm_setpointer(s, 16, pair);
        return 7850;
    }
    case 0x1eaa: { // ldi r18, 0xB0
        s->r[18] = 176;
        return 7852;
    }
    case 0x1eac: { // add r18, r22
        s->r[18] = pm_add(s, s->r[18], s->r[22], 0);
        return 7854;
    }
    case 0x1eae: { // cli
        pm_irq(s, false);
        return 7856;
    }
    case 0x1eb0: { // rcall .+1116
        s->calls[s->call_depth++] = 7858;
        return 8974;
    }
    case 0x1eb2: { // sei
        pm_irq(s, true);
        return 7860;
    }
    case 0x1eb4: { // ldi r28, 0x63
        s->r[28] = 99;
        return 7862;
    }
    case 0x1eb6: { // ldi r29, 0x21
        s->r[29] = 33;
        return 7864;
    }
    case 0x1eb8: { // mov r23, r22
        s->r[23] = s->r[22];
        return 7866;
    }
    case 0x1eba: { // add r23, r23
        s->r[23] = pm_add(s, s->r[23], s->r[23], 0);
        return 7868;
    }
    case 0x1ebc: { // eor r11, r11
        s->r[11] ^= s->r[11];
        pm_nzv(s, s->r[11], false);
        return 7870;
    }
    case 0x1ebe: { // add r28, r23
        s->r[28] = pm_add(s, s->r[28], s->r[23], 0);
        return 7872;
    }
    case 0x1ec0: { // adc r29, r11
        s->r[29] = pm_add(s, s->r[29], s->r[11], pm_getflag(s, CARRY));
        return 7874;
    }
    case 0x1ec2: { // st Y+, r20
        uint16_t address = pm_pointer(s, 28) + 0;
        pm_setpointer(s, 28, address + 1);
        pm_write(s, address, s->r[20]);
        return 7876;
    }
    case 0x1ec4: { // st Y, r21
        uint16_t address = pm_pointer(s, 28) + 0;
        pm_write(s, address, s->r[21]);
        return 7878;
    }
    case 0x1ec6: { // ldi r28, 0xCF
        s->r[28] = 207;
        return 7880;
    }
    case 0x1ec8: { // ldi r29, 0x21
        s->r[29] = 33;
        return 7882;
    }
    case 0x1eca: { // mov r10, r22
        s->r[10] = s->r[22];
        return 7884;
    }
    case 0x1ecc: { // add r10, r10
        s->r[10] = pm_add(s, s->r[10], s->r[10], 0);
        return 7886;
    }
    case 0x1ece: { // add r10, r10
        s->r[10] = pm_add(s, s->r[10], s->r[10], 0);
        return 7888;
    }
    case 0x1ed0: { // add r10, r10
        s->r[10] = pm_add(s, s->r[10], s->r[10], 0);
        return 7890;
    }
    case 0x1ed2: { // eor r11, r11
        s->r[11] ^= s->r[11];
        pm_nzv(s, s->r[11], false);
        return 7892;
    }
    case 0x1ed4: { // add r28, r10
        s->r[28] = pm_add(s, s->r[28], s->r[10], 0);
        return 7894;
    }
    case 0x1ed6: { // adc r29, r11
        s->r[29] = pm_add(s, s->r[29], s->r[11], pm_getflag(s, CARRY));
        return 7896;
    }
    case 0x1ed8: { // ld r20, Y+
        uint16_t address = pm_pointer(s, 28) + 0;
        pm_setpointer(s, 28, address + 1);
        s->r[20] = pm_read(s, address);
        return 7898;
    }
    case 0x1eda: { // ld r21, Y
        uint16_t address = pm_pointer(s, 28) + 0;
        s->r[21] = pm_read(s, address);
        return 7900;
    }
    case 0x1edc: { // rcall .+456
        s->calls[s->call_depth++] = 7902;
        return 8358;
    }
    case 0x1ede: { // rjmp .+258
        return 8162;
    }
    default: return UINT32_MAX;
    }
}
