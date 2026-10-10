uint32_t pm_logical_fpga_ready_guard(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x211c: { // clc
        pm_flag(s, CARRY, false);
        return 8478;
    }
    case 0x211e: { // lds r16, 0x2157
        uint16_t address = 8535;
        s->r[16] = pm_read(s, address);
        return 8482;
    }
    case 0x2122: { // sbrc r16, 4
        return (!!(s->r[16] & (1u << 4)) == 0) ? 8486 : 8484;
    }
    case 0x2124: { // ret
        return s->calls[--s->call_depth];
    }
    case 0x2126: { // ldi r30, 0xB4
        s->r[30] = 180;
        return 8488;
    }
    case 0x2128: { // ldi r31, 0x2A
        s->r[31] = 42;
        return 8490;
    }
    case 0x212a: { // rcall .+1786
        s->calls[s->call_depth++] = 8492;
        return 10278;
    }
    case 0x212c: { // sec
        pm_flag(s, CARRY, true);
        return 8494;
    }
    case 0x212e: { // ret
        return s->calls[--s->call_depth];
    }
    default: return UINT32_MAX;
    }
}
