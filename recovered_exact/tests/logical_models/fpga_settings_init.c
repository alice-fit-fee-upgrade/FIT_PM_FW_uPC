uint32_t pm_logical_fpga_settings_init(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x08e4: { // push r18
        s->stack[s->depth++] = s->r[18];
        return 2278;
    }
    case 0x08e6: { // push r17
        s->stack[s->depth++] = s->r[17];
        return 2280;
    }
    case 0x08e8: { // push r16
        s->stack[s->depth++] = s->r[16];
        return 2282;
    }
    case 0x08ea: { // push r29
        s->stack[s->depth++] = s->r[29];
        return 2284;
    }
    case 0x08ec: { // push r30
        s->stack[s->depth++] = s->r[30];
        return 2286;
    }
    case 0x08ee: { // ldi r18, 0x7C
        s->r[18] = 124;
        return 2288;
    }
    case 0x08f0: { // eor r16, r16
        s->r[16] ^= s->r[16];
        pm_nzv(s, s->r[16], false);
        return 2290;
    }
    case 0x08f2: { // eor r17, r17
        s->r[17] ^= s->r[17];
        pm_nzv(s, s->r[17], false);
        return 2292;
    }
    case 0x08f4: { // cli
        pm_irq(s, false);
        return 2294;
    }
    case 0x08f6: { // call 0x230e
        s->calls[s->call_depth++] = 2298;
        return 8974;
    }
    case 0x08fa: { // sei
        pm_irq(s, true);
        return 2300;
    }
    case 0x08fc: { // lds r16, 0x222F
        uint16_t address = 8751;
        s->r[16] = pm_read(s, address);
        return 2304;
    }
    case 0x0900: { // eor r17, r17
        s->r[17] ^= s->r[17];
        pm_nzv(s, s->r[17], false);
        return 2306;
    }
    case 0x0902: { // eor r18, r18
        s->r[18] ^= s->r[18];
        pm_nzv(s, s->r[18], false);
        return 2308;
    }
    case 0x0904: { // cli
        pm_irq(s, false);
        return 2310;
    }
    case 0x0906: { // call 0x230e
        s->calls[s->call_depth++] = 2314;
        return 8974;
    }
    case 0x090a: { // sei
        pm_irq(s, true);
        return 2316;
    }
    case 0x090c: { // ldi r28, 0xB7
        s->r[28] = 183;
        return 2318;
    }
    case 0x090e: { // ldi r29, 0x21
        s->r[29] = 33;
        return 2320;
    }
    case 0x0910: { // ld r16, Y+
        uint16_t address = pm_pointer(s, 28) + 0;
        pm_setpointer(s, 28, address + 1);
        s->r[16] = pm_read(s, address);
        return 2322;
    }
    case 0x0912: { // ld r17, Y+
        uint16_t address = pm_pointer(s, 28) + 0;
        pm_setpointer(s, 28, address + 1);
        s->r[17] = pm_read(s, address);
        return 2324;
    }
    case 0x0914: { // inc r18
        s->r[18]++;
        pm_nzv(s, s->r[18], s->r[18] == 128);
        return 2326;
    }
    case 0x0916: { // cli
        pm_irq(s, false);
        return 2328;
    }
    case 0x0918: { // call 0x230e
        s->calls[s->call_depth++] = 2332;
        return 8974;
    }
    case 0x091c: { // sei
        pm_irq(s, true);
        return 2334;
    }
    case 0x091e: { // cpi r18, 0x0C
        pm_sub(s, s->r[18], 12, 0, false);
        return 2336;
    }
    case 0x0920: { // brne .-18
        return (pm_getflag(s, 1) == 0) ? 2320 : 2338;
    }
    case 0x0922: { // ldi r18, 0x24
        s->r[18] = 36;
        return 2340;
    }
    case 0x0924: { // ldi r28, 0x87
        s->r[28] = 135;
        return 2342;
    }
    case 0x0926: { // ldi r29, 0x21
        s->r[29] = 33;
        return 2344;
    }
    case 0x0928: { // ld r16, Y+
        uint16_t address = pm_pointer(s, 28) + 0;
        pm_setpointer(s, 28, address + 1);
        s->r[16] = pm_read(s, address);
        return 2346;
    }
    case 0x092a: { // ld r17, Y+
        uint16_t address = pm_pointer(s, 28) + 0;
        pm_setpointer(s, 28, address + 1);
        s->r[17] = pm_read(s, address);
        return 2348;
    }
    case 0x092c: { // inc r18
        s->r[18]++;
        pm_nzv(s, s->r[18], s->r[18] == 128);
        return 2350;
    }
    case 0x092e: { // cli
        pm_irq(s, false);
        return 2352;
    }
    case 0x0930: { // call 0x230e
        s->calls[s->call_depth++] = 2356;
        return 8974;
    }
    case 0x0934: { // sei
        pm_irq(s, true);
        return 2358;
    }
    case 0x0936: { // cpi r18, 0x3C
        pm_sub(s, s->r[18], 60, 0, false);
        return 2360;
    }
    case 0x0938: { // brne .-18
        return (pm_getflag(s, 1) == 0) ? 2344 : 2362;
    }
    case 0x093a: { // inc r18
        s->r[18]++;
        pm_nzv(s, s->r[18], s->r[18] == 128);
        return 2364;
    }
    case 0x093c: { // lds r16, 0x2230
        uint16_t address = 8752;
        s->r[16] = pm_read(s, address);
        return 2368;
    }
    case 0x0940: { // lds r17, 0x2231
        uint16_t address = 8753;
        s->r[17] = pm_read(s, address);
        return 2372;
    }
    case 0x0944: { // cli
        pm_irq(s, false);
        return 2374;
    }
    case 0x0946: { // call 0x230e
        s->calls[s->call_depth++] = 2378;
        return 8974;
    }
    case 0x094a: { // sei
        pm_irq(s, true);
        return 2380;
    }
    case 0x094c: { // lds r16, 0x2160
        uint16_t address = 8544;
        s->r[16] = pm_read(s, address);
        return 2384;
    }
    case 0x0950: { // lds r17, 0x2161
        uint16_t address = 8545;
        s->r[17] = pm_read(s, address);
        return 2388;
    }
    case 0x0954: { // ldi r18, 0xBC
        s->r[18] = 188;
        return 2390;
    }
    case 0x0956: { // cli
        pm_irq(s, false);
        return 2392;
    }
    case 0x0958: { // call 0x230e
        s->calls[s->call_depth++] = 2396;
        return 8974;
    }
    case 0x095c: { // sei
        pm_irq(s, true);
        return 2398;
    }
    case 0x095e: { // lds r16, 0x2232
        uint16_t address = 8754;
        s->r[16] = pm_read(s, address);
        return 2402;
    }
    case 0x0962: { // lds r17, 0x2233
        uint16_t address = 8755;
        s->r[17] = pm_read(s, address);
        return 2406;
    }
    case 0x0966: { // ldi r18, 0xBD
        s->r[18] = 189;
        return 2408;
    }
    case 0x0968: { // cli
        pm_irq(s, false);
        return 2410;
    }
    case 0x096a: { // call 0x230e
        s->calls[s->call_depth++] = 2414;
        return 8974;
    }
    case 0x096e: { // sei
        pm_irq(s, true);
        return 2416;
    }
    case 0x0970: { // ldi r18, 0x7C
        s->r[18] = 124;
        return 2418;
    }
    case 0x0972: { // ldi r16, 0xFF
        s->r[16] = 255;
        return 2420;
    }
    case 0x0974: { // ldi r17, 0x0F
        s->r[17] = 15;
        return 2422;
    }
    case 0x0976: { // cli
        pm_irq(s, false);
        return 2424;
    }
    case 0x0978: { // call 0x230e
        s->calls[s->call_depth++] = 2428;
        return 8974;
    }
    case 0x097c: { // sei
        pm_irq(s, true);
        return 2430;
    }
    case 0x097e: { // pop r30
        s->r[30] = s->stack[--s->depth];
        return 2432;
    }
    case 0x0980: { // pop r29
        s->r[29] = s->stack[--s->depth];
        return 2434;
    }
    case 0x0982: { // pop r16
        s->r[16] = s->stack[--s->depth];
        return 2436;
    }
    case 0x0984: { // pop r17
        s->r[17] = s->stack[--s->depth];
        return 2438;
    }
    case 0x0986: { // pop r18
        s->r[18] = s->stack[--s->depth];
        return 2440;
    }
    case 0x0988: { // ret
        return s->calls[--s->call_depth];
    }
    default: return UINT32_MAX;
    }
}
