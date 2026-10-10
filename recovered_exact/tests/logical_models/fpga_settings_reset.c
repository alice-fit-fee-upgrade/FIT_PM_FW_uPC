uint32_t pm_logical_fpga_settings_reset(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x098a: { // push r16
        s->stack[s->depth++] = s->r[16];
        return 2444;
    }
    case 0x098c: { // push r17
        s->stack[s->depth++] = s->r[17];
        return 2446;
    }
    case 0x098e: { // push r18
        s->stack[s->depth++] = s->r[18];
        return 2448;
    }
    case 0x0990: { // push r28
        s->stack[s->depth++] = s->r[28];
        return 2450;
    }
    case 0x0992: { // push r29
        s->stack[s->depth++] = s->r[29];
        return 2452;
    }
    case 0x0994: { // ldi r18, 0x80
        s->r[18] = 128;
        return 2454;
    }
    case 0x0996: { // ldi r28, 0xCF
        s->r[28] = 207;
        return 2456;
    }
    case 0x0998: { // ldi r29, 0x21
        s->r[29] = 33;
        return 2458;
    }
    case 0x099a: { // ld r16, Y+
        uint16_t address = pm_pointer(s, 28) + 0;
        pm_setpointer(s, 28, address + 1);
        s->r[16] = pm_read(s, address);
        return 2460;
    }
    case 0x099c: { // ld r17, Y+
        uint16_t address = pm_pointer(s, 28) + 0;
        pm_setpointer(s, 28, address + 1);
        s->r[17] = pm_read(s, address);
        return 2462;
    }
    case 0x099e: { // cli
        pm_irq(s, false);
        return 2464;
    }
    case 0x09a0: { // call 0x230e
        s->calls[s->call_depth++] = 2468;
        return 8974;
    }
    case 0x09a4: { // sei
        pm_irq(s, true);
        return 2470;
    }
    case 0x09a6: { // inc r18
        s->r[18]++;
        pm_nzv(s, s->r[18], s->r[18] == 128);
        return 2472;
    }
    case 0x09a8: { // cpi r18, 0xB0
        pm_sub(s, s->r[18], 176, 0, false);
        return 2474;
    }
    case 0x09aa: { // brne .-18
        return (pm_getflag(s, 1) == 0) ? 2458 : 2476;
    }
    case 0x09ac: { // ldi r28, 0x63
        s->r[28] = 99;
        return 2478;
    }
    case 0x09ae: { // ldi r29, 0x21
        s->r[29] = 33;
        return 2480;
    }
    case 0x09b0: { // ld r16, Y+
        uint16_t address = pm_pointer(s, 28) + 0;
        pm_setpointer(s, 28, address + 1);
        s->r[16] = pm_read(s, address);
        return 2482;
    }
    case 0x09b2: { // ld r17, Y+
        uint16_t address = pm_pointer(s, 28) + 0;
        pm_setpointer(s, 28, address + 1);
        s->r[17] = pm_read(s, address);
        return 2484;
    }
    case 0x09b4: { // cli
        pm_irq(s, false);
        return 2486;
    }
    case 0x09b6: { // call 0x230e
        s->calls[s->call_depth++] = 2490;
        return 8974;
    }
    case 0x09ba: { // sei
        pm_irq(s, true);
        return 2492;
    }
    case 0x09bc: { // inc r18
        s->r[18]++;
        pm_nzv(s, s->r[18], s->r[18] == 128);
        return 2494;
    }
    case 0x09be: { // cpi r18, 0xBC
        pm_sub(s, s->r[18], 188, 0, false);
        return 2496;
    }
    case 0x09c0: { // brne .-18
        return (pm_getflag(s, 1) == 0) ? 2480 : 2498;
    }
    case 0x09c2: { // lds r16, 0x2441
        uint16_t address = 9281;
        s->r[16] = pm_read(s, address);
        return 2502;
    }
    case 0x09c6: { // eor r17, r17
        s->r[17] ^= s->r[17];
        pm_nzv(s, s->r[17], false);
        return 2504;
    }
    case 0x09c8: { // ldi r18, 0xBE
        s->r[18] = 190;
        return 2506;
    }
    case 0x09ca: { // cli
        pm_irq(s, false);
        return 2508;
    }
    case 0x09cc: { // call 0x230e
        s->calls[s->call_depth++] = 2512;
        return 8974;
    }
    case 0x09d0: { // sei
        pm_irq(s, true);
        return 2514;
    }
    case 0x09d2: { // pop r29
        s->r[29] = s->stack[--s->depth];
        return 2516;
    }
    case 0x09d4: { // pop r28
        s->r[28] = s->stack[--s->depth];
        return 2518;
    }
    case 0x09d6: { // pop r18
        s->r[18] = s->stack[--s->depth];
        return 2520;
    }
    case 0x09d8: { // pop r17
        s->r[17] = s->stack[--s->depth];
        return 2522;
    }
    case 0x09da: { // pop r16
        s->r[16] = s->stack[--s->depth];
        return 2524;
    }
    case 0x09dc: { // ret
        return s->calls[--s->call_depth];
    }
    default: return UINT32_MAX;
    }
}
