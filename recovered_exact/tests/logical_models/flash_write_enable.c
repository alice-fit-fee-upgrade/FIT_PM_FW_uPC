uint32_t pm_logical_flash_write_enable(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x171e: { // ldi r16, 0x10
        s->r[16] = 16;
        return 5920;
    }
    case 0x1720: { // sts 0x0686, r16
        uint16_t address = 1670;
        pm_write(s, address, s->r[16]);
        return 5924;
    }
    case 0x1724: { // ldi r16, 0x06
        s->r[16] = 6;
        return 5926;
    }
    case 0x1726: { // sts 0x0AC3, r16
        uint16_t address = 2755;
        pm_write(s, address, s->r[16]);
        return 5930;
    }
    case 0x172a: { // lds r19, 0x0AC2
        uint16_t address = 2754;
        s->r[19] = pm_read(s, address);
        return 5934;
    }
    case 0x172e: { // sbrs r19, 7
        return (!!(s->r[19] & (1u << 7)) == 1) ? 5938 : 5936;
    }
    case 0x1730: { // rjmp .-8
        return 5930;
    }
    case 0x1732: { // ldi r16, 0x10
        s->r[16] = 16;
        return 5940;
    }
    case 0x1734: { // sts 0x0685, r16
        uint16_t address = 1669;
        pm_write(s, address, s->r[16]);
        return 5944;
    }
    case 0x1738: { // ret
        return s->calls[--s->call_depth];
    }
    default: return UINT32_MAX;
    }
}
