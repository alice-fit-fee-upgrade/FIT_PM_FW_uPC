uint32_t pm_logical_pll_status_interrupt(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x0ae0: { // push r31
        s->stack[s->depth++] = s->r[31];
        return 2786;
    }
    case 0x0ae2: { // in r31, 0x3f
        s->r[31] = pm_io_read(s, 63);
        return 2788;
    }
    case 0x0ae4: { // push r16
        s->stack[s->depth++] = s->r[16];
        return 2790;
    }
    case 0x0ae6: { // push r17
        s->stack[s->depth++] = s->r[17];
        return 2792;
    }
    case 0x0ae8: { // push r18
        s->stack[s->depth++] = s->r[18];
        return 2794;
    }
    case 0x0aea: { // push r19
        s->stack[s->depth++] = s->r[19];
        return 2796;
    }
    case 0x0aec: { // lds r16, 0x06A8
        uint16_t address = 1704;
        s->r[16] = pm_read(s, address);
        return 2800;
    }
    case 0x0af0: { // bst r16, 6
        pm_flag(s, TRANSFER, s->r[16] & (1u << 6));
        return 2802;
    }
    case 0x0af2: { // brtc .+22
        return (pm_getflag(s, 6) == 0) ? 2826 : 2804;
    }
    case 0x0af4: { // sbrc r16, 5
        return (!!(s->r[16] & (1u << 5)) == 0) ? 2808 : 2806;
    }
    case 0x0af6: { // rjmp .+18
        return 2826;
    }
    case 0x0af8: { // ldi r16, 0xF5
        s->r[16] = 245;
        return 2810;
    }
    case 0x0afa: { // ldi r17, 0x0F
        s->r[17] = 15;
        return 2812;
    }
    case 0x0afc: { // ldi r18, 0x04
        s->r[18] = 4;
        return 2814;
    }
    case 0x0afe: { // ldi r19, 0x40
        s->r[19] = 64;
        return 2816;
    }
    case 0x0b00: { // call 0x2486
        s->calls[s->call_depth++] = 2820;
        return 9350;
    }
    case 0x0b04: { // ldi r16, 0x20
        s->r[16] = 32;
        return 2822;
    }
    case 0x0b06: { // sts 0x06A5, r16
        uint16_t address = 1701;
        pm_write(s, address, s->r[16]);
        return 2826;
    }
    case 0x0b0a: { // call 0x24ce
        s->calls[s->call_depth++] = 2830;
        return 9422;
    }
    case 0x0b0e: { // swap r19
        s->r[19] = (s->r[19] >> 4) | (s->r[19] << 4);
        return 2832;
    }
    case 0x0b10: { // andi r19, 0x0E
        s->r[19] &= 14;
        pm_nzv(s, s->r[19], false);
        return 2834;
    }
    case 0x0b12: { // bld r19, 0
        s->r[19] = (s->r[19] & ~(1u << 0)) | (pm_getflag(s, TRANSFER) << 0);
        return 2836;
    }
    case 0x0b14: { // lds r16, 0x2162
        uint16_t address = 8546;
        s->r[16] = pm_read(s, address);
        return 2840;
    }
    case 0x0b18: { // andi r16, 0x0E
        s->r[16] &= 14;
        pm_nzv(s, s->r[16], false);
        return 2842;
    }
    case 0x0b1a: { // sts 0x2162, r19
        uint16_t address = 8546;
        pm_write(s, address, s->r[19]);
        return 2846;
    }
    case 0x0b1e: { // eor r16, r19
        s->r[16] ^= s->r[19];
        pm_nzv(s, s->r[16], false);
        return 2848;
    }
    case 0x0b20: { // breq .+32
        return (pm_getflag(s, 1) == 1) ? 2882 : 2850;
    }
    case 0x0b22: { // ldi r16, 0x20
        s->r[16] = 32;
        return 2852;
    }
    case 0x0b24: { // sbrs r19, 1
        return (!!(s->r[19] & (1u << 1)) == 1) ? 2856 : 2854;
    }
    case 0x0b26: { // rjmp .+8
        return 2864;
    }
    case 0x0b28: { // sts 0x0606, r16
        uint16_t address = 1542;
        pm_write(s, address, s->r[16]);
        return 2860;
    }
    case 0x0b2c: { // cbi 0x00, 2
        uint8_t value = pm_io_read(s, 0);
        pm_io_write(s, 0, value & ~ (1u << 2));
        return 2862;
    }
    case 0x0b2e: { // rjmp .+18
        return 2882;
    }
    case 0x0b30: { // sbrs r19, 2
        return (!!(s->r[19] & (1u << 2)) == 1) ? 2868 : 2866;
    }
    case 0x0b32: { // rjmp .+2
        return 2870;
    }
    case 0x0b34: { // sbi 0x00, 2
        uint8_t value = pm_io_read(s, 0);
        pm_io_write(s, 0, value | (1u << 2));
        return 2870;
    }
    case 0x0b36: { // sbrs r19, 3
        return (!!(s->r[19] & (1u << 3)) == 1) ? 2874 : 2872;
    }
    case 0x0b38: { // rjmp .+8
        return 2882;
    }
    case 0x0b3a: { // ldi r16, 0x20
        s->r[16] = 32;
        return 2876;
    }
    case 0x0b3c: { // sts 0x0605, r16
        uint16_t address = 1541;
        pm_write(s, address, s->r[16]);
        return 2880;
    }
    case 0x0b40: { // cbi 0x00, 2
        uint8_t value = pm_io_read(s, 0);
        pm_io_write(s, 0, value & ~ (1u << 2));
        return 2882;
    }
    case 0x0b42: { // brtc .+14
        return (pm_getflag(s, 6) == 0) ? 2898 : 2884;
    }
    case 0x0b44: { // ldi r16, 0x0A
        s->r[16] = 10;
        return 2886;
    }
    case 0x0b46: { // sts 0x215A, r16
        uint16_t address = 8538;
        pm_write(s, address, s->r[16]);
        return 2890;
    }
    case 0x0b4a: { // ldi r16, 0x01
        s->r[16] = 1;
        return 2892;
    }
    case 0x0b4c: { // sts 0x2159, r16
        uint16_t address = 8537;
        pm_write(s, address, s->r[16]);
        return 2896;
    }
    case 0x0b50: { // rjmp .+8
        return 2906;
    }
    case 0x0b52: { // rcall .+20
        s->calls[s->call_depth++] = 2900;
        return 2920;
    }
    case 0x0b54: { // ldi r16, 0x02
        s->r[16] = 2;
        return 2902;
    }
    case 0x0b56: { // sts 0x2441, r16
        uint16_t address = 9281;
        pm_write(s, address, s->r[16]);
        return 2906;
    }
    case 0x0b5a: { // pop r19
        s->r[19] = s->stack[--s->depth];
        return 2908;
    }
    case 0x0b5c: { // pop r18
        s->r[18] = s->stack[--s->depth];
        return 2910;
    }
    case 0x0b5e: { // pop r17
        s->r[17] = s->stack[--s->depth];
        return 2912;
    }
    case 0x0b60: { // pop r16
        s->r[16] = s->stack[--s->depth];
        return 2914;
    }
    case 0x0b62: { // out 0x3f, r31
        pm_io_write(s, 63, s->r[31]);
        return 2916;
    }
    case 0x0b64: { // pop r31
        s->r[31] = s->stack[--s->depth];
        return 2918;
    }
    case 0x0b66: { // reti
        pm_flag(s, INTERRUPT, true);
        return s->calls[--s->call_depth];
    }
    default: return UINT32_MAX;
    }
}
