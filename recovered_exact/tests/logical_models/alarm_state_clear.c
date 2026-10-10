uint32_t pm_logical_alarm_state_clear(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x1be4: { // rcall .+3158
        s->calls[s->call_depth++] = 7142;
        return 10300;
    }
    case 0x1be6: { // cpi r16, 0x0D
        pm_sub(s, s->r[16], 13, 0, false);
        return 7144;
    }
    case 0x1be8: { // brne .+118
        return (pm_getflag(s, 1) == 0) ? 7264 : 7146;
    }
    case 0x1bea: { // cli
        pm_irq(s, false);
        return 7148;
    }
    case 0x1bec: { // lds r16, 0x2157
        uint16_t address = 8535;
        s->r[16] = pm_read(s, address);
        return 7152;
    }
    case 0x1bf0: { // in r17, 0x00
        s->r[17] = pm_io_read(s, 0);
        return 7154;
    }
    case 0x1bf2: { // andi r17, 0x03
        s->r[17] &= 3;
        pm_nzv(s, s->r[17], false);
        return 7156;
    }
    case 0x1bf4: { // brne .+34
        return (pm_getflag(s, 1) == 0) ? 7192 : 7158;
    }
    case 0x1bf6: { // lds r16, 0x2157
        uint16_t address = 8535;
        s->r[16] = pm_read(s, address);
        return 7162;
    }
    case 0x1bfa: { // andi r16, 0x7F
        s->r[16] &= 127;
        pm_nzv(s, s->r[16], false);
        return 7164;
    }
    case 0x1bfc: { // sts 0x2157, r16
        uint16_t address = 8535;
        pm_write(s, address, s->r[16]);
        return 7168;
    }
    case 0x1c00: { // lds r16, 0x2158
        uint16_t address = 8536;
        s->r[16] = pm_read(s, address);
        return 7172;
    }
    case 0x1c04: { // andi r16, 0xE1
        s->r[16] &= 225;
        pm_nzv(s, s->r[16], false);
        return 7174;
    }
    case 0x1c06: { // sts 0x2158, r16
        uint16_t address = 8536;
        pm_write(s, address, s->r[16]);
        return 7178;
    }
    case 0x1c0a: { // sei
        pm_irq(s, true);
        return 7180;
    }
    case 0x1c0c: { // ldi r16, 0x40
        s->r[16] = 64;
        return 7182;
    }
    case 0x1c0e: { // sts 0x0605, r16
        uint16_t address = 1541;
        pm_write(s, address, s->r[16]);
        return 7186;
    }
    case 0x1c12: { // ldi r30, 0x08
        s->r[30] = 8;
        return 7188;
    }
    case 0x1c14: { // ldi r31, 0x2A
        s->r[31] = 42;
        return 7190;
    }
    case 0x1c16: { // rjmp .+68
        return 7260;
    }
    case 0x1c18: { // andi r16, 0x07
        s->r[16] &= 7;
        pm_nzv(s, s->r[16], false);
        return 7194;
    }
    case 0x1c1a: { // cpi r16, 0x01
        pm_sub(s, s->r[16], 1, 0, false);
        return 7196;
    }
    case 0x1c1c: { // breq .+8
        return (pm_getflag(s, 1) == 1) ? 7206 : 7198;
    }
    case 0x1c1e: { // sei
        pm_irq(s, true);
        return 7200;
    }
    case 0x1c20: { // ldi r30, 0x9E
        s->r[30] = 158;
        return 7202;
    }
    case 0x1c22: { // ldi r31, 0x29
        s->r[31] = 41;
        return 7204;
    }
    case 0x1c24: { // rjmp .+54
        return 7260;
    }
    case 0x1c26: { // lds r16, 0x2157
        uint16_t address = 8535;
        s->r[16] = pm_read(s, address);
        return 7210;
    }
    case 0x1c2a: { // andi r16, 0x1F
        s->r[16] &= 31;
        pm_nzv(s, s->r[16], false);
        return 7212;
    }
    case 0x1c2c: { // sts 0x2157, r16
        uint16_t address = 8535;
        pm_write(s, address, s->r[16]);
        return 7216;
    }
    case 0x1c30: { // eor r16, r16
        s->r[16] ^= s->r[16];
        pm_nzv(s, s->r[16], false);
        return 7218;
    }
    case 0x1c32: { // sts 0x2158, r16
        uint16_t address = 8536;
        pm_write(s, address, s->r[16]);
        return 7222;
    }
    case 0x1c36: { // ldi r16, 0x01
        s->r[16] = 1;
        return 7224;
    }
    case 0x1c38: { // sts 0x215B, r16
        uint16_t address = 8539;
        pm_write(s, address, s->r[16]);
        return 7228;
    }
    case 0x1c3c: { // ldi r16, 0x04
        s->r[16] = 4;
        return 7230;
    }
    case 0x1c3e: { // sts 0x0686, r16
        uint16_t address = 1670;
        pm_write(s, address, s->r[16]);
        return 7234;
    }
    case 0x1c42: { // ldi r16, 0xD0
        s->r[16] = 208;
        return 7236;
    }
    case 0x1c44: { // sts 0x215C, r16
        uint16_t address = 8540;
        pm_write(s, address, s->r[16]);
        return 7240;
    }
    case 0x1c48: { // ldi r16, 0x07
        s->r[16] = 7;
        return 7242;
    }
    case 0x1c4a: { // sts 0x215D, r16
        uint16_t address = 8541;
        pm_write(s, address, s->r[16]);
        return 7246;
    }
    case 0x1c4e: { // cbi 0x00, 1
        uint8_t value = pm_io_read(s, 0);
        pm_io_write(s, 0, value & ~ (1u << 1));
        return 7248;
    }
    case 0x1c50: { // ldi r16, 0x41
        s->r[16] = 65;
        return 7250;
    }
    case 0x1c52: { // sts 0x0605, r16
        uint16_t address = 1541;
        pm_write(s, address, s->r[16]);
        return 7254;
    }
    case 0x1c56: { // sei
        pm_irq(s, true);
        return 7256;
    }
    case 0x1c58: { // ldi r30, 0x98
        s->r[30] = 152;
        return 7258;
    }
    case 0x1c5a: { // ldi r31, 0x29
        s->r[31] = 41;
        return 7260;
    }
    case 0x1c5c: { // rcall .+3016
        s->calls[s->call_depth++] = 7262;
        return 10278;
    }
    case 0x1c5e: { // ret
        return s->calls[--s->call_depth];
    }
    case 0x1c60: { // rjmp .-2202
        return 5064;
    }
    default: return UINT32_MAX;
    }
}
