uint32_t pm_logical_flash_device_id(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x1840: { // ldi r16, 0x10
        s->r[16] = 16;
        return 6210;
    }
    case 0x1842: { // sts 0x0686, r16
        uint16_t address = 1670;
        pm_write(s, address, s->r[16]);
        return 6214;
    }
    case 0x1846: { // ldi r16, 0x9F
        s->r[16] = 159;
        return 6216;
    }
    case 0x1848: { // sts 0x0AC3, r16
        uint16_t address = 2755;
        pm_write(s, address, s->r[16]);
        return 6220;
    }
    case 0x184c: { // eor r20, r20
        s->r[20] ^= s->r[20];
        pm_nzv(s, s->r[20], false);
        return 6222;
    }
    case 0x184e: { // lds r19, 0x0AC2
        uint16_t address = 2754;
        s->r[19] = pm_read(s, address);
        return 6226;
    }
    case 0x1852: { // sbrs r19, 7
        return (!!(s->r[19] & (1u << 7)) == 1) ? 6230 : 6228;
    }
    case 0x1854: { // rjmp .-8
        return 6222;
    }
    case 0x1856: { // sts 0x0AC3, r20
        uint16_t address = 2755;
        pm_write(s, address, s->r[20]);
        return 6234;
    }
    case 0x185a: { // lds r19, 0x0AC2
        uint16_t address = 2754;
        s->r[19] = pm_read(s, address);
        return 6238;
    }
    case 0x185e: { // sbrs r19, 7
        return (!!(s->r[19] & (1u << 7)) == 1) ? 6242 : 6240;
    }
    case 0x1860: { // rjmp .-8
        return 6234;
    }
    case 0x1862: { // lds r18, 0x0AC3
        uint16_t address = 2755;
        s->r[18] = pm_read(s, address);
        return 6246;
    }
    case 0x1866: { // sts 0x0AC3, r20
        uint16_t address = 2755;
        pm_write(s, address, s->r[20]);
        return 6250;
    }
    case 0x186a: { // lds r19, 0x0AC2
        uint16_t address = 2754;
        s->r[19] = pm_read(s, address);
        return 6254;
    }
    case 0x186e: { // sbrs r19, 7
        return (!!(s->r[19] & (1u << 7)) == 1) ? 6258 : 6256;
    }
    case 0x1870: { // rjmp .-8
        return 6250;
    }
    case 0x1872: { // lds r19, 0x0AC3
        uint16_t address = 2755;
        s->r[19] = pm_read(s, address);
        return 6262;
    }
    case 0x1876: { // sts 0x0AC3, r20
        uint16_t address = 2755;
        pm_write(s, address, s->r[20]);
        return 6266;
    }
    case 0x187a: { // lds r20, 0x0AC2
        uint16_t address = 2754;
        s->r[20] = pm_read(s, address);
        return 6270;
    }
    case 0x187e: { // sbrs r20, 7
        return (!!(s->r[20] & (1u << 7)) == 1) ? 6274 : 6272;
    }
    case 0x1880: { // rjmp .-8
        return 6266;
    }
    case 0x1882: { // lds r16, 0x0AC3
        uint16_t address = 2755;
        s->r[16] = pm_read(s, address);
        return 6278;
    }
    case 0x1886: { // ldi r20, 0x10
        s->r[20] = 16;
        return 6280;
    }
    case 0x1888: { // sts 0x0685, r20
        uint16_t address = 1669;
        pm_write(s, address, s->r[20]);
        return 6284;
    }
    case 0x188c: { // ret
        return s->calls[--s->call_depth];
    }
    default: return UINT32_MAX;
    }
}
