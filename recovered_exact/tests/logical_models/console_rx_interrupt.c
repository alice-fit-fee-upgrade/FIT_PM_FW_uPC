uint32_t pm_logical_console_rx_interrupt(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x0ee0: { // push r31
        s->stack[s->depth++] = s->r[31];
        return 3810;
    }
    case 0x0ee2: { // in r31, 0x3f
        s->r[31] = pm_io_read(s, 63);
        return 3812;
    }
    case 0x0ee4: { // push r31
        s->stack[s->depth++] = s->r[31];
        return 3814;
    }
    case 0x0ee6: { // push r30
        s->stack[s->depth++] = s->r[30];
        return 3816;
    }
    case 0x0ee8: { // push r19
        s->stack[s->depth++] = s->r[19];
        return 3818;
    }
    case 0x0eea: { // push r18
        s->stack[s->depth++] = s->r[18];
        return 3820;
    }
    case 0x0eec: { // push r17
        s->stack[s->depth++] = s->r[17];
        return 3822;
    }
    case 0x0eee: { // push r16
        s->stack[s->depth++] = s->r[16];
        return 3824;
    }
    case 0x0ef0: { // ldi r30, 0x00
        s->r[30] = 0;
        return 3826;
    }
    case 0x0ef2: { // ldi r31, 0x20
        s->r[31] = 32;
        return 3828;
    }
    case 0x0ef4: { // ld r16, Z+
        uint16_t address = pm_pointer(s, 30) + 0;
        pm_setpointer(s, 30, address + 1);
        s->r[16] = pm_read(s, address);
        return 3830;
    }
    case 0x0ef6: { // ld r17, Z
        uint16_t address = pm_pointer(s, 30) + 0;
        s->r[17] = pm_read(s, address);
        return 3832;
    }
    case 0x0ef8: { // inc r17
        s->r[17]++;
        pm_nzv(s, s->r[17], s->r[17] == 128);
        return 3834;
    }
    case 0x0efa: { // andi r17, 0x3F
        s->r[17] &= 63;
        pm_nzv(s, s->r[17], false);
        return 3836;
    }
    case 0x0efc: { // mov r18, r17
        s->r[18] = s->r[17];
        return 3838;
    }
    case 0x0efe: { // subi r18, 0xEC
        s->r[18] = pm_sub(s, s->r[18], 236, 0, false);
        return 3840;
    }
    case 0x0f00: { // andi r18, 0x3F
        s->r[18] &= 63;
        pm_nzv(s, s->r[18], false);
        return 3842;
    }
    case 0x0f02: { // cp r18, r16
        pm_sub(s, s->r[18], s->r[16], 0, false);
        return 3844;
    }
    case 0x0f04: { // brne .+16
        return (pm_getflag(s, 1) == 0) ? 3862 : 3846;
    }
    case 0x0f06: { // ldi r18, 0x01
        s->r[18] = 1;
        return 3848;
    }
    case 0x0f08: { // sts 0x06A5, r18
        uint16_t address = 1701;
        pm_write(s, address, s->r[18]);
        return 3852;
    }
    case 0x0f0c: { // lds r18, 0x2005
        uint16_t address = 8197;
        s->r[18] = pm_read(s, address);
        return 3856;
    }
    case 0x0f10: { // ori r18, 0x80
        s->r[18] |= 128;
        pm_nzv(s, s->r[18], false);
        return 3858;
    }
    case 0x0f12: { // sts 0x2005, r18
        uint16_t address = 8197;
        pm_write(s, address, s->r[18]);
        return 3862;
    }
    case 0x0f16: { // lds r19, 0x0BA1
        uint16_t address = 2977;
        s->r[19] = pm_read(s, address);
        return 3866;
    }
    case 0x0f1a: { // lds r18, 0x0BA0
        uint16_t address = 2976;
        s->r[18] = pm_read(s, address);
        return 3870;
    }
    case 0x0f1e: { // sbrc r19, 4
        return (!!(s->r[19] & (1u << 4)) == 0) ? 3874 : 3872;
    }
    case 0x0f20: { // rjmp .+32
        return 3906;
    }
    case 0x0f22: { // cp r16, r17
        pm_sub(s, s->r[16], s->r[17], 0, false);
        return 3876;
    }
    case 0x0f24: { // breq .+28
        return (pm_getflag(s, 1) == 1) ? 3906 : 3878;
    }
    case 0x0f26: { // st Z, r17
        uint16_t address = pm_pointer(s, 30) + 0;
        pm_write(s, address, s->r[17]);
        return 3880;
    }
    case 0x0f28: { // ldi r30, 0x07
        s->r[30] = 7;
        return 3882;
    }
    case 0x0f2a: { // ldi r31, 0x20
        s->r[31] = 32;
        return 3884;
    }
    case 0x0f2c: { // eor r16, r16
        s->r[16] ^= s->r[16];
        pm_nzv(s, s->r[16], false);
        return 3886;
    }
    case 0x0f2e: { // add r30, r17
        s->r[30] = pm_add(s, s->r[30], s->r[17], 0);
        return 3888;
    }
    case 0x0f30: { // adc r31, r16
        s->r[31] = pm_add(s, s->r[31], s->r[16], pm_getflag(s, CARRY));
        return 3890;
    }
    case 0x0f32: { // st Z, r18
        uint16_t address = pm_pointer(s, 30) + 0;
        pm_write(s, address, s->r[18]);
        return 3892;
    }
    case 0x0f34: { // cpi r18, 0x0D
        pm_sub(s, s->r[18], 13, 0, false);
        return 3894;
    }
    case 0x0f36: { // brne .+10
        return (pm_getflag(s, 1) == 0) ? 3906 : 3896;
    }
    case 0x0f38: { // lds r16, 0x2005
        uint16_t address = 8197;
        s->r[16] = pm_read(s, address);
        return 3900;
    }
    case 0x0f3c: { // inc r16
        s->r[16]++;
        pm_nzv(s, s->r[16], s->r[16] == 128);
        return 3902;
    }
    case 0x0f3e: { // sts 0x2005, r16
        uint16_t address = 8197;
        pm_write(s, address, s->r[16]);
        return 3906;
    }
    case 0x0f42: { // pop r16
        s->r[16] = s->stack[--s->depth];
        return 3908;
    }
    case 0x0f44: { // pop r17
        s->r[17] = s->stack[--s->depth];
        return 3910;
    }
    case 0x0f46: { // pop r18
        s->r[18] = s->stack[--s->depth];
        return 3912;
    }
    case 0x0f48: { // pop r19
        s->r[19] = s->stack[--s->depth];
        return 3914;
    }
    case 0x0f4a: { // pop r30
        s->r[30] = s->stack[--s->depth];
        return 3916;
    }
    case 0x0f4c: { // pop r31
        s->r[31] = s->stack[--s->depth];
        return 3918;
    }
    case 0x0f4e: { // out 0x3f, r31
        pm_io_write(s, 63, s->r[31]);
        return 3920;
    }
    case 0x0f50: { // pop r31
        s->r[31] = s->stack[--s->depth];
        return 3922;
    }
    case 0x0f52: { // reti
        pm_flag(s, INTERRUPT, true);
        return s->calls[--s->call_depth];
    }
    default: return UINT32_MAX;
    }
}
