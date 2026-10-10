uint32_t pm_logical_system_deinit(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x0c96: { // eor r16, r16
        s->r[16] ^= s->r[16];
        pm_nzv(s, s->r[16], false);
        return 3224;
    }
    case 0x0c98: { // sts 0x215C, r16
        uint16_t address = 8540;
        pm_write(s, address, s->r[16]);
        return 3228;
    }
    case 0x0c9c: { // sts 0x215D, r16
        uint16_t address = 8541;
        pm_write(s, address, s->r[16]);
        return 3232;
    }
    case 0x0ca0: { // sts 0x215B, r16
        uint16_t address = 8539;
        pm_write(s, address, s->r[16]);
        return 3236;
    }
    case 0x0ca4: { // sts 0x2159, r16
        uint16_t address = 8537;
        pm_write(s, address, s->r[16]);
        return 3240;
    }
    case 0x0ca8: { // sts 0x2441, r16
        uint16_t address = 9281;
        pm_write(s, address, s->r[16]);
        return 3244;
    }
    case 0x0cac: { // sts 0x2006, r16
        uint16_t address = 8198;
        pm_write(s, address, s->r[16]);
        return 3248;
    }
    case 0x0cb0: { // sts 0x2162, r16
        uint16_t address = 8546;
        pm_write(s, address, s->r[16]);
        return 3252;
    }
    case 0x0cb4: { // ldi r16, 0x01
        s->r[16] = 1;
        return 3254;
    }
    case 0x0cb6: { // sts 0x0689, r16
        uint16_t address = 1673;
        pm_write(s, address, s->r[16]);
        return 3258;
    }
    case 0x0cba: { // ldi r16, 0x02
        s->r[16] = 2;
        return 3260;
    }
    case 0x0cbc: { // sts 0x06A9, r16
        uint16_t address = 1705;
        pm_write(s, address, s->r[16]);
        return 3264;
    }
    case 0x0cc0: { // ldi r16, 0x00
        s->r[16] = 0;
        return 3266;
    }
    case 0x0cc2: { // sts 0x0629, r16
        uint16_t address = 1577;
        pm_write(s, address, s->r[16]);
        return 3270;
    }
    case 0x0cc6: { // sts 0x0669, r16
        uint16_t address = 1641;
        pm_write(s, address, s->r[16]);
        return 3274;
    }
    case 0x0cca: { // ldi r16, 0xBF
        s->r[16] = 191;
        return 3276;
    }
    case 0x0ccc: { // sts 0x0622, r16
        uint16_t address = 1570;
        pm_write(s, address, s->r[16]);
        return 3280;
    }
    case 0x0cd0: { // eor r16, r16
        s->r[16] ^= s->r[16];
        pm_nzv(s, s->r[16], false);
        return 3282;
    }
    case 0x0cd2: { // sts 0x08C0, r16
        uint16_t address = 2240;
        pm_write(s, address, s->r[16]);
        return 3286;
    }
    case 0x0cd6: { // ldi r16, 0xFF
        s->r[16] = 255;
        return 3288;
    }
    case 0x0cd8: { // sts 0x0642, r16
        uint16_t address = 1602;
        pm_write(s, address, s->r[16]);
        return 3292;
    }
    case 0x0cdc: { // ldi r16, 0x41
        s->r[16] = 65;
        return 3294;
    }
    case 0x0cde: { // sts 0x0662, r16
        uint16_t address = 1634;
        pm_write(s, address, s->r[16]);
        return 3298;
    }
    case 0x0ce2: { // ldi r16, 0x04
        s->r[16] = 4;
        return 3300;
    }
    case 0x0ce4: { // sts 0x0666, r16
        uint16_t address = 1638;
        pm_write(s, address, s->r[16]);
        return 3304;
    }
    case 0x0ce8: { // ldi r16, 0x20
        s->r[16] = 32;
        return 3306;
    }
    case 0x0cea: { // sts 0x06A6, r16
        uint16_t address = 1702;
        pm_write(s, address, s->r[16]);
        return 3310;
    }
    case 0x0cee: { // ldi r16, 0x20
        s->r[16] = 32;
        return 3312;
    }
    case 0x0cf0: { // sts 0x06A2, r16
        uint16_t address = 1698;
        pm_write(s, address, s->r[16]);
        return 3316;
    }
    case 0x0cf4: { // lds r16, 0x2157
        uint16_t address = 8535;
        s->r[16] = pm_read(s, address);
        return 3320;
    }
    case 0x0cf8: { // andi r16, 0xEF
        s->r[16] &= 239;
        pm_nzv(s, s->r[16], false);
        return 3322;
    }
    case 0x0cfa: { // sts 0x2157, r16
        uint16_t address = 8535;
        pm_write(s, address, s->r[16]);
        return 3326;
    }
    case 0x0cfe: { // ldi r16, 0xA0
        s->r[16] = 160;
        return 3328;
    }
    case 0x0d00: { // sts 0x0605, r16
        uint16_t address = 1541;
        pm_write(s, address, s->r[16]);
        return 3332;
    }
    case 0x0d04: { // eor r16, r16
        s->r[16] ^= s->r[16];
        pm_nzv(s, s->r[16], false);
        return 3334;
    }
    case 0x0d06: { // sts 0x09C0, r16
        uint16_t address = 2496;
        pm_write(s, address, s->r[16]);
        return 3338;
    }
    case 0x0d0a: { // sts 0x0100, r16
        uint16_t address = 256;
        pm_write(s, address, s->r[16]);
        return 3342;
    }
    case 0x0d0e: { // ret
        return s->calls[--s->call_depth];
    }
    default: return UINT32_MAX;
    }
}
