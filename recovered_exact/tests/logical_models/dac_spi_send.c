uint32_t pm_logical_dac_spi_send(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x22aa: { // push r22
        s->stack[s->depth++] = s->r[22];
        return 8876;
    }
    case 0x22ac: { // push r23
        s->stack[s->depth++] = s->r[23];
        return 8878;
    }
    case 0x22ae: { // push r24
        s->stack[s->depth++] = s->r[24];
        return 8880;
    }
    case 0x22b0: { // cli
        pm_irq(s, false);
        return 8882;
    }
    case 0x22b2: { // ldi r23, 0xD5
        s->r[23] = 213;
        return 8884;
    }
    case 0x22b4: { // sts 0x08C0, r23
        uint16_t address = 2240;
        pm_write(s, address, s->r[23]);
        return 8888;
    }
    case 0x22b8: { // mov r23, r22
        s->r[23] = s->r[22];
        return 8890;
    }
    case 0x22ba: { // ldi r24, 0x01
        s->r[24] = 1;
        return 8892;
    }
    case 0x22bc: { // andi r23, 0x30
        s->r[23] &= 48;
        pm_nzv(s, s->r[23], false);
        return 8894;
    }
    case 0x22be: { // breq .+8
        return (pm_getflag(s, 1) == 1) ? 8904 : 8896;
    }
    case 0x22c0: { // add r24, r24
        s->r[24] = pm_add(s, s->r[24], s->r[24], 0);
        return 8898;
    }
    case 0x22c2: { // subi r23, 0x10
        s->r[23] = pm_sub(s, s->r[23], 16, 0, false);
        return 8900;
    }
    case 0x22c4: { // breq .+2
        return (pm_getflag(s, 1) == 1) ? 8904 : 8902;
    }
    case 0x22c6: { // add r24, r24
        s->r[24] = pm_add(s, s->r[24], s->r[24], 0);
        return 8904;
    }
    case 0x22c8: { // mov r23, r22
        s->r[23] = s->r[22];
        return 8906;
    }
    case 0x22ca: { // swap r22
        s->r[22] = (s->r[22] >> 4) | (s->r[22] << 4);
        return 8908;
    }
    case 0x22cc: { // andi r22, 0xC0
        s->r[22] &= 192;
        pm_nzv(s, s->r[22], false);
        return 8910;
    }
    case 0x22ce: { // add r23, r23
        s->r[23] = pm_add(s, s->r[23], s->r[23], 0);
        return 8912;
    }
    case 0x22d0: { // andi r23, 0x06
        s->r[23] &= 6;
        pm_nzv(s, s->r[23], false);
        return 8914;
    }
    case 0x22d2: { // or r22, r23
        s->r[22] |= s->r[23];
        pm_nzv(s, s->r[22], false);
        return 8916;
    }
    case 0x22d4: { // ori r22, 0x10
        s->r[22] |= 16;
        pm_nzv(s, s->r[22], false);
        return 8918;
    }
    case 0x22d6: { // sts 0x0646, r24
        uint16_t address = 1606;
        pm_write(s, address, s->r[24]);
        return 8922;
    }
    case 0x22da: { // sts 0x08C3, r22
        uint16_t address = 2243;
        pm_write(s, address, s->r[22]);
        return 8926;
    }
    case 0x22de: { // lds r22, 0x08C2
        uint16_t address = 2242;
        s->r[22] = pm_read(s, address);
        return 8930;
    }
    case 0x22e2: { // sbrs r22, 7
        return (!!(s->r[22] & (1u << 7)) == 1) ? 8934 : 8932;
    }
    case 0x22e4: { // rjmp .-8
        return 8926;
    }
    case 0x22e6: { // sts 0x08C3, r17
        uint16_t address = 2243;
        pm_write(s, address, s->r[17]);
        return 8938;
    }
    case 0x22ea: { // lds r22, 0x08C2
        uint16_t address = 2242;
        s->r[22] = pm_read(s, address);
        return 8942;
    }
    case 0x22ee: { // sbrs r22, 7
        return (!!(s->r[22] & (1u << 7)) == 1) ? 8946 : 8944;
    }
    case 0x22f0: { // rjmp .-8
        return 8938;
    }
    case 0x22f2: { // sts 0x08C3, r16
        uint16_t address = 2243;
        pm_write(s, address, s->r[16]);
        return 8950;
    }
    case 0x22f6: { // lds r22, 0x08C2
        uint16_t address = 2242;
        s->r[22] = pm_read(s, address);
        return 8954;
    }
    case 0x22fa: { // sbrs r22, 7
        return (!!(s->r[22] & (1u << 7)) == 1) ? 8958 : 8956;
    }
    case 0x22fc: { // rjmp .-8
        return 8950;
    }
    case 0x22fe: { // ldi r22, 0x07
        s->r[22] = 7;
        return 8960;
    }
    case 0x2300: { // sts 0x0645, r22
        uint16_t address = 1605;
        pm_write(s, address, s->r[22]);
        return 8964;
    }
    case 0x2304: { // sei
        pm_irq(s, true);
        return 8966;
    }
    case 0x2306: { // pop r24
        s->r[24] = s->stack[--s->depth];
        return 8968;
    }
    case 0x2308: { // pop r23
        s->r[23] = s->stack[--s->depth];
        return 8970;
    }
    case 0x230a: { // pop r22
        s->r[22] = s->stack[--s->depth];
        return 8972;
    }
    case 0x230c: { // ret
        return s->calls[--s->call_depth];
    }
    default: return UINT32_MAX;
    }
}
