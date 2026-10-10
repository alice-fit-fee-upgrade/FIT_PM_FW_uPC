uint32_t pm_logical_flash_address_send(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x1808: { // ldi r17, 0x10
        s->r[17] = 16;
        return 6154;
    }
    case 0x180a: { // sts 0x0686, r17
        uint16_t address = 1670;
        pm_write(s, address, s->r[17]);
        return 6158;
    }
    case 0x180e: { // sts 0x0AC3, r16
        uint16_t address = 2755;
        pm_write(s, address, s->r[16]);
        return 6162;
    }
    case 0x1812: { // lds r19, 0x0AC2
        uint16_t address = 2754;
        s->r[19] = pm_read(s, address);
        return 6166;
    }
    case 0x1816: { // sbrs r19, 7
        return (!!(s->r[19] & (1u << 7)) == 1) ? 6170 : 6168;
    }
    case 0x1818: { // rjmp .-8
        return 6162;
    }
    case 0x181a: { // sts 0x0AC3, r22
        uint16_t address = 2755;
        pm_write(s, address, s->r[22]);
        return 6174;
    }
    case 0x181e: { // lds r19, 0x0AC2
        uint16_t address = 2754;
        s->r[19] = pm_read(s, address);
        return 6178;
    }
    case 0x1822: { // sbrs r19, 7
        return (!!(s->r[19] & (1u << 7)) == 1) ? 6182 : 6180;
    }
    case 0x1824: { // rjmp .-8
        return 6174;
    }
    case 0x1826: { // sts 0x0AC3, r21
        uint16_t address = 2755;
        pm_write(s, address, s->r[21]);
        return 6186;
    }
    case 0x182a: { // lds r19, 0x0AC2
        uint16_t address = 2754;
        s->r[19] = pm_read(s, address);
        return 6190;
    }
    case 0x182e: { // sbrs r19, 7
        return (!!(s->r[19] & (1u << 7)) == 1) ? 6194 : 6192;
    }
    case 0x1830: { // rjmp .-8
        return 6186;
    }
    case 0x1832: { // sts 0x0AC3, r20
        uint16_t address = 2755;
        pm_write(s, address, s->r[20]);
        return 6198;
    }
    case 0x1836: { // lds r19, 0x0AC2
        uint16_t address = 2754;
        s->r[19] = pm_read(s, address);
        return 6202;
    }
    case 0x183a: { // sbrs r19, 7
        return (!!(s->r[19] & (1u << 7)) == 1) ? 6206 : 6204;
    }
    case 0x183c: { // rjmp .-8
        return 6198;
    }
    case 0x183e: { // ret
        return s->calls[--s->call_depth];
    }
    default: return UINT32_MAX;
    }
}
