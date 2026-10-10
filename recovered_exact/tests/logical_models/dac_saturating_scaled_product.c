uint32_t pm_logical_dac_saturating_scaled_product(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x214c: { // eor r17, r17
        s->r[17] ^= s->r[17];
        pm_nzv(s, s->r[17], false);
        return 8526;
    }
    case 0x214e: { // mul r18, r20
        uint16_t product = (s->r[18]) * (int)s->r[20];
        pm_setpointer(s, 0, product);
        pm_flag(s, ZERO, product == 0);
        pm_flag(s, CARRY, product & 0x8000);
        return 8528;
    }
    case 0x2150: { // sbrc r0, 7
        return (!!(s->r[0] & (1u << 7)) == 0) ? 8532 : 8530;
    }
    case 0x2152: { // inc r1
        s->r[1]++;
        pm_nzv(s, s->r[1], s->r[1] == 128);
        return 8532;
    }
    case 0x2154: { // mov r16, r1
        s->r[16] = s->r[1];
        return 8534;
    }
    case 0x2156: { // mul r19, r20
        uint16_t product = (s->r[19]) * (int)s->r[20];
        pm_setpointer(s, 0, product);
        pm_flag(s, ZERO, product == 0);
        pm_flag(s, CARRY, product & 0x8000);
        return 8536;
    }
    case 0x2158: { // add r16, r0
        s->r[16] = pm_add(s, s->r[16], s->r[0], 0);
        return 8538;
    }
    case 0x215a: { // adc r17, r1
        s->r[17] = pm_add(s, s->r[17], s->r[1], pm_getflag(s, CARRY));
        return 8540;
    }
    case 0x215c: { // mul r18, r21
        uint16_t product = (s->r[18]) * (int)s->r[21];
        pm_setpointer(s, 0, product);
        pm_flag(s, ZERO, product == 0);
        pm_flag(s, CARRY, product & 0x8000);
        return 8542;
    }
    case 0x215e: { // add r16, r0
        s->r[16] = pm_add(s, s->r[16], s->r[0], 0);
        return 8544;
    }
    case 0x2160: { // adc r17, r1
        s->r[17] = pm_add(s, s->r[17], s->r[1], pm_getflag(s, CARRY));
        return 8546;
    }
    case 0x2162: { // mul r19, r21
        uint16_t product = (s->r[19]) * (int)s->r[21];
        pm_setpointer(s, 0, product);
        pm_flag(s, ZERO, product == 0);
        pm_flag(s, CARRY, product & 0x8000);
        return 8548;
    }
    case 0x2164: { // and r1, r1
        s->r[1] &= s->r[1];
        pm_nzv(s, s->r[1], false);
        return 8550;
    }
    case 0x2166: { // brne .+6
        return (pm_getflag(s, 1) == 0) ? 8558 : 8552;
    }
    case 0x2168: { // add r17, r0
        s->r[17] = pm_add(s, s->r[17], s->r[0], 0);
        return 8554;
    }
    case 0x216a: { // brcs .+2
        return (pm_getflag(s, 0) == 1) ? 8558 : 8556;
    }
    case 0x216c: { // ret
        return s->calls[--s->call_depth];
    }
    case 0x216e: { // ldi r16, 0xFF
        s->r[16] = 255;
        return 8560;
    }
    case 0x2170: { // ldi r17, 0xFF
        s->r[17] = 255;
        return 8562;
    }
    case 0x2172: { // ret
        return s->calls[--s->call_depth];
    }
    default: return UINT32_MAX;
    }
}
