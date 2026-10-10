uint32_t pm_logical_adt_bit_transfer(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x2608: { // ldi r18, 0x08
        s->r[18] = 8;
        return 9738;
    }
    case 0x260a: { // ldi r17, 0x08
        s->r[17] = 8;
        return 9740;
    }
    case 0x260c: { // sbrc r16, 7
        return (!!(s->r[16] & (1u << 7)) == 0) ? 9746 : 9742;
    }
    case 0x260e: { // sts 0x0605, r17
        uint16_t address = 1541;
        pm_write(s, address, s->r[17]);
        return 9746;
    }
    case 0x2612: { // sbrs r16, 7
        return (!!(s->r[16] & (1u << 7)) == 1) ? 9752 : 9748;
    }
    case 0x2614: { // sts 0x0606, r17
        uint16_t address = 1542;
        pm_write(s, address, s->r[17]);
        return 9752;
    }
    case 0x2618: { // ldi r17, 0x02
        s->r[17] = 2;
        return 9754;
    }
    case 0x261a: { // sts 0x0606, r17
        uint16_t address = 1542;
        pm_write(s, address, s->r[17]);
        return 9758;
    }
    case 0x261e: { // add r16, r16
        s->r[16] = pm_add(s, s->r[16], s->r[16], 0);
        return 9760;
    }
    case 0x2620: { // nop
        return 9762;
    }
    case 0x2622: { // sts 0x0605, r17
        uint16_t address = 1541;
        pm_write(s, address, s->r[17]);
        return 9766;
    }
    case 0x2626: { // lds r17, 0x0608
        uint16_t address = 1544;
        s->r[17] = pm_read(s, address);
        return 9770;
    }
    case 0x262a: { // bst r17, 2
        pm_flag(s, TRANSFER, s->r[17] & (1u << 2));
        return 9772;
    }
    case 0x262c: { // bld r16, 0
        s->r[16] = (s->r[16] & ~(1u << 0)) | (pm_getflag(s, TRANSFER) << 0);
        return 9774;
    }
    case 0x262e: { // dec r18
        s->r[18]--;
        pm_nzv(s, s->r[18], s->r[18] == 127);
        return 9776;
    }
    case 0x2630: { // brne .-40
        return (pm_getflag(s, 1) == 0) ? 9738 : 9778;
    }
    case 0x2632: { // ret
        return s->calls[--s->call_depth];
    }
    default: return UINT32_MAX;
    }
}
