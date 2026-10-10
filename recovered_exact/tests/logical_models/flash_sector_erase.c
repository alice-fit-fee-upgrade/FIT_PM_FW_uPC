uint32_t pm_logical_flash_sector_erase(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x1710: { // rcall .+12
        s->calls[s->call_depth++] = 5906;
        return 5918;
    }
    case 0x1712: { // ldi r16, 0xD8
        s->r[16] = 216;
        return 5908;
    }
    case 0x1714: { // rcall .+242
        s->calls[s->call_depth++] = 5910;
        return 6152;
    }
    case 0x1716: { // ldi r16, 0x10
        s->r[16] = 16;
        return 5912;
    }
    case 0x1718: { // sts 0x0685, r16
        uint16_t address = 1669;
        pm_write(s, address, s->r[16]);
        return 5916;
    }
    case 0x171c: { // ret
        return s->calls[--s->call_depth];
    }
    default: return UINT32_MAX;
    }
}
