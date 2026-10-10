uint32_t pm_logical_dma_gpio_handshake(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x046c: { // ldi r16, 0x80
        s->r[16] = 128;
        return 1134;
    }
    case 0x046e: { // ldi r17, 0x10
        s->r[17] = 16;
        return 1136;
    }
    case 0x0470: { // sei
        pm_irq(s, true);
        return 1138;
    }
    case 0x0472: { // cli
        pm_irq(s, false);
        return 1140;
    }
    case 0x0474: { // lds r18, 0x0668
        uint16_t address = 1640;
        s->r[18] = pm_read(s, address);
        return 1144;
    }
    case 0x0478: { // sbrs r18, 4
        return (!!(s->r[18] & (1u << 4)) == 1) ? 1148 : 1146;
    }
    case 0x047a: { // rjmp .-12
        return 1136;
    }
    case 0x047c: { // sts 0x243C, r16
        uint16_t address = 9276;
        pm_write(s, address, s->r[16]);
        return 1152;
    }
    case 0x0480: { // sts 0x0111, r17
        uint16_t address = 273;
        pm_write(s, address, s->r[17]);
        return 1156;
    }
    case 0x0484: { // sei
        pm_irq(s, true);
        return 1158;
    }
    case 0x0486: { // lds r16, 0x0111
        uint16_t address = 273;
        s->r[16] = pm_read(s, address);
        return 1162;
    }
    case 0x048a: { // sbrs r16, 4
        return (!!(s->r[16] & (1u << 4)) == 1) ? 1166 : 1164;
    }
    case 0x048c: { // rjmp .-8
        return 1158;
    }
    case 0x048e: { // eor r16, r16
        s->r[16] ^= s->r[16];
        pm_nzv(s, s->r[16], false);
        return 1168;
    }
    case 0x0490: { // sts 0x243C, r16
        uint16_t address = 9276;
        pm_write(s, address, s->r[16]);
        return 1172;
    }
    case 0x0494: { // lds r18, 0x0668
        uint16_t address = 1640;
        s->r[18] = pm_read(s, address);
        return 1176;
    }
    case 0x0498: { // sbrs r18, 4
        return (!!(s->r[18] & (1u << 4)) == 1) ? 1180 : 1178;
    }
    case 0x049a: { // rjmp .-8
        return 1172;
    }
    case 0x049c: { // ret
        return s->calls[--s->call_depth];
    }
    default: return UINT32_MAX;
    }
}
