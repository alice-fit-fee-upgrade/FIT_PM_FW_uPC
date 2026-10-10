uint32_t pm_logical_flash_interface_init(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x1664: { // ldi r16, 0x02
        s->r[16] = 2;
        return 5734;
    }
    case 0x1666: { // sts 0x0666, r16
        uint16_t address = 1638;
        pm_write(s, address, s->r[16]);
        return 5738;
    }
    case 0x166a: { // ldi r16, 0x10
        s->r[16] = 16;
        return 5740;
    }
    case 0x166c: { // sts 0x0685, r16
        uint16_t address = 1669;
        pm_write(s, address, s->r[16]);
        return 5744;
    }
    case 0x1670: { // ldi r16, 0xB0
        s->r[16] = 176;
        return 5746;
    }
    case 0x1672: { // sts 0x0681, r16
        uint16_t address = 1665;
        pm_write(s, address, s->r[16]);
        return 5750;
    }
    case 0x1676: { // ldi r16, 0x50
        s->r[16] = 80;
        return 5752;
    }
    case 0x1678: { // sts 0x0AC0, r16
        uint16_t address = 2752;
        pm_write(s, address, s->r[16]);
        return 5756;
    }
    case 0x167c: { // ret
        return s->calls[--s->call_depth];
    }
    default: return UINT32_MAX;
    }
}
