uint32_t pm_logical_fpga_status_interrupt(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x0c2a: { // push r31
        s->stack[s->depth++] = s->r[31];
        return 3116;
    }
    case 0x0c2c: { // in r31, 0x3f
        s->r[31] = pm_io_read(s, 63);
        return 3118;
    }
    case 0x0c2e: { // push r16
        s->stack[s->depth++] = s->r[16];
        return 3120;
    }
    case 0x0c30: { // push r17
        s->stack[s->depth++] = s->r[17];
        return 3122;
    }
    case 0x0c32: { // push r18
        s->stack[s->depth++] = s->r[18];
        return 3124;
    }
    case 0x0c34: { // ldi r18, 0x7F
        s->r[18] = 127;
        return 3126;
    }
    case 0x0c36: { // call 0x2368
        s->calls[s->call_depth++] = 3130;
        return 9064;
    }
    case 0x0c3a: { // sbrs r17, 1
        return (!!(s->r[17] & (1u << 1)) == 1) ? 3134 : 3132;
    }
    case 0x0c3c: { // rjmp .+10
        return 3144;
    }
    case 0x0c3e: { // lds r18, 0x2006
        uint16_t address = 8198;
        s->r[18] = pm_read(s, address);
        return 3138;
    }
    case 0x0c42: { // ori r18, 0x01
        s->r[18] |= 1;
        pm_nzv(s, s->r[18], false);
        return 3140;
    }
    case 0x0c44: { // sts 0x2006, r18
        uint16_t address = 8198;
        pm_write(s, address, s->r[18]);
        return 3144;
    }
    case 0x0c48: { // swap r16
        s->r[16] = (s->r[16] >> 4) | (s->r[16] << 4);
        return 3146;
    }
    case 0x0c4a: { // andi r16, 0x0F
        s->r[16] &= 15;
        pm_nzv(s, s->r[16], false);
        return 3148;
    }
    case 0x0c4c: { // swap r17
        s->r[17] = (s->r[17] >> 4) | (s->r[17] << 4);
        return 3150;
    }
    case 0x0c4e: { // andi r17, 0x10
        s->r[17] &= 16;
        pm_nzv(s, s->r[17], false);
        return 3152;
    }
    case 0x0c50: { // or r17, r16
        s->r[17] |= s->r[16];
        pm_nzv(s, s->r[17], false);
        return 3154;
    }
    case 0x0c52: { // lds r16, 0x2158
        uint16_t address = 8536;
        s->r[16] = pm_read(s, address);
        return 3158;
    }
    case 0x0c56: { // andi r16, 0xFE
        s->r[16] &= 254;
        pm_nzv(s, s->r[16], false);
        return 3160;
    }
    case 0x0c58: { // or r16, r17
        s->r[16] |= s->r[17];
        pm_nzv(s, s->r[16], false);
        return 3162;
    }
    case 0x0c5a: { // sts 0x2158, r16
        uint16_t address = 8536;
        pm_write(s, address, s->r[16]);
        return 3166;
    }
    case 0x0c5e: { // andi r17, 0x1C
        s->r[17] &= 28;
        pm_nzv(s, s->r[17], false);
        return 3168;
    }
    case 0x0c60: { // breq .+16
        return (pm_getflag(s, 1) == 1) ? 3186 : 3170;
    }
    case 0x0c62: { // lds r16, 0x2157
        uint16_t address = 8535;
        s->r[16] = pm_read(s, address);
        return 3174;
    }
    case 0x0c66: { // ori r16, 0x80
        s->r[16] |= 128;
        pm_nzv(s, s->r[16], false);
        return 3176;
    }
    case 0x0c68: { // sts 0x2157, r16
        uint16_t address = 8535;
        pm_write(s, address, s->r[16]);
        return 3180;
    }
    case 0x0c6c: { // ldi r16, 0x40
        s->r[16] = 64;
        return 3182;
    }
    case 0x0c6e: { // sts 0x0606, r16
        uint16_t address = 1542;
        pm_write(s, address, s->r[16]);
        return 3186;
    }
    case 0x0c72: { // pop r18
        s->r[18] = s->stack[--s->depth];
        return 3188;
    }
    case 0x0c74: { // pop r17
        s->r[17] = s->stack[--s->depth];
        return 3190;
    }
    case 0x0c76: { // pop r16
        s->r[16] = s->stack[--s->depth];
        return 3192;
    }
    case 0x0c78: { // out 0x3f, r31
        pm_io_write(s, 63, s->r[31]);
        return 3194;
    }
    case 0x0c7a: { // pop r31
        s->r[31] = s->stack[--s->depth];
        return 3196;
    }
    case 0x0c7c: { // reti
        pm_flag(s, INTERRUPT, true);
        return s->calls[--s->call_depth];
    }
    default: return UINT32_MAX;
    }
}
