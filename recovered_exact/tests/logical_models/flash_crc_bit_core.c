uint32_t pm_logical_flash_crc_bit_core(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x17e4: { // ldi r20, 0x08
        s->r[20] = 8;
        return 6118;
    }
    case 0x17e6: { // add r18, r18
        s->r[18] = pm_add(s, s->r[18], s->r[18], 0);
        return 6120;
    }
    case 0x17e8: { // adc r19, r19
        s->r[19] = pm_add(s, s->r[19], s->r[19], pm_getflag(s, CARRY));
        return 6122;
    }
    case 0x17ea: { // adc r16, r16
        s->r[16] = pm_add(s, s->r[16], s->r[16], pm_getflag(s, CARRY));
        return 6124;
    }
    case 0x17ec: { // adc r17, r17
        s->r[17] = pm_add(s, s->r[17], s->r[17], pm_getflag(s, CARRY));
        return 6126;
    }
    case 0x17ee: { // brcc .+6
        return (pm_getflag(s, 0) == 0) ? 6134 : 6128;
    }
    case 0x17f0: { // lsr r22
        bool carry = s->r[22] & 1;
        s->r[22] = (s->r[22] >> 1) | 0;
        pm_flag(s, CARRY, carry);
        pm_nzv(s, s->r[22], !!(s->r[22] & 128) ^ carry);
        return 6130;
    }
    case 0x17f2: { // brcs .+14
        return (pm_getflag(s, 0) == 1) ? 6146 : 6132;
    }
    case 0x17f4: { // rjmp .+4
        return 6138;
    }
    case 0x17f6: { // lsr r22
        bool carry = s->r[22] & 1;
        s->r[22] = (s->r[22] >> 1) | 0;
        pm_flag(s, CARRY, carry);
        pm_nzv(s, s->r[22], !!(s->r[22] & 128) ^ carry);
        return 6136;
    }
    case 0x17f8: { // brcc .+8
        return (pm_getflag(s, 0) == 0) ? 6146 : 6138;
    }
    case 0x17fa: { // eor r18, r23
        s->r[18] ^= s->r[23];
        pm_nzv(s, s->r[18], false);
        return 6140;
    }
    case 0x17fc: { // eor r19, r24
        s->r[19] ^= s->r[24];
        pm_nzv(s, s->r[19], false);
        return 6142;
    }
    case 0x17fe: { // eor r16, r25
        s->r[16] ^= s->r[25];
        pm_nzv(s, s->r[16], false);
        return 6144;
    }
    case 0x1800: { // eor r17, r26
        s->r[17] ^= s->r[26];
        pm_nzv(s, s->r[17], false);
        return 6146;
    }
    case 0x1802: { // dec r20
        s->r[20]--;
        pm_nzv(s, s->r[20], s->r[20] == 127);
        return 6148;
    }
    case 0x1804: { // brne .-32
        return (pm_getflag(s, 1) == 0) ? 6118 : 6150;
    }
    case 0x1806: { // ret
        return s->calls[--s->call_depth];
    }
    default: return UINT32_MAX;
    }
}
