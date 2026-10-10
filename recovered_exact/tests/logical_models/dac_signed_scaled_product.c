uint32_t pm_logical_dac_signed_scaled_product(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x2130: { // eor r17, r17
        s->r[17] ^= s->r[17];
        pm_nzv(s, s->r[17], false);
        return 8498;
    }
    case 0x2132: { // mul r18, r20
        uint16_t product = (s->r[18]) * (int)s->r[20];
        pm_setpointer(s, 0, product);
        pm_flag(s, ZERO, product == 0);
        pm_flag(s, CARRY, product & 0x8000);
        return 8500;
    }
    case 0x2134: { // sbrc r0, 7
        return (!!(s->r[0] & (1u << 7)) == 0) ? 8504 : 8502;
    }
    case 0x2136: { // inc r1
        s->r[1]++;
        pm_nzv(s, s->r[1], s->r[1] == 128);
        return 8504;
    }
    case 0x2138: { // mov r16, r1
        s->r[16] = s->r[1];
        return 8506;
    }
    case 0x213a: { // mul r19, r20
        uint16_t product = (s->r[19]) * (int)s->r[20];
        pm_setpointer(s, 0, product);
        pm_flag(s, ZERO, product == 0);
        pm_flag(s, CARRY, product & 0x8000);
        return 8508;
    }
    case 0x213c: { // add r16, r0
        s->r[16] = pm_add(s, s->r[16], s->r[0], 0);
        return 8510;
    }
    case 0x213e: { // adc r17, r1
        s->r[17] = pm_add(s, s->r[17], s->r[1], pm_getflag(s, CARRY));
        return 8512;
    }
    case 0x2140: { // mulsu r21, r18
        uint16_t product = ((int8_t)s->r[21]) * (int)s->r[18];
        pm_setpointer(s, 0, product);
        pm_flag(s, ZERO, product == 0);
        pm_flag(s, CARRY, product & 0x8000);
        return 8514;
    }
    case 0x2142: { // add r16, r0
        s->r[16] = pm_add(s, s->r[16], s->r[0], 0);
        return 8516;
    }
    case 0x2144: { // adc r17, r1
        s->r[17] = pm_add(s, s->r[17], s->r[1], pm_getflag(s, CARRY));
        return 8518;
    }
    case 0x2146: { // mulsu r21, r19
        uint16_t product = ((int8_t)s->r[21]) * (int)s->r[19];
        pm_setpointer(s, 0, product);
        pm_flag(s, ZERO, product == 0);
        pm_flag(s, CARRY, product & 0x8000);
        return 8520;
    }
    case 0x2148: { // add r17, r0
        s->r[17] = pm_add(s, s->r[17], s->r[0], 0);
        return 8522;
    }
    case 0x214a: { // ret
        return s->calls[--s->call_depth];
    }
    default: return UINT32_MAX;
    }
}
