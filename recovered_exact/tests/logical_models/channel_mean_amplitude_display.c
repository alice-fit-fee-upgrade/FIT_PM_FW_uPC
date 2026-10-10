uint32_t pm_logical_channel_mean_amplitude_display(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x188e: { // rcall .+4012
        s->calls[s->call_depth++] = 6288;
        return 10300;
    }
    case 0x1890: { // cpi r16, 0x0D
        pm_sub(s, s->r[16], 13, 0, false);
        return 6290;
    }
    case 0x1892: { // brne .+122
        return (pm_getflag(s, 1) == 0) ? 6414 : 6292;
    }
    case 0x1894: { // rcall .+2182
        s->calls[s->call_depth++] = 6294;
        return 8476;
    }
    case 0x1896: { // brcc .+2
        return (pm_getflag(s, 0) == 0) ? 6298 : 6296;
    }
    case 0x1898: { // ret
        return s->calls[--s->call_depth];
    }
    case 0x189a: { // ldi r18, 0x64
        s->r[18] = 100;
        return 6300;
    }
    case 0x189c: { // cli
        pm_irq(s, false);
        return 6302;
    }
    case 0x189e: { // rcall .+2760
        s->calls[s->call_depth++] = 6304;
        return 9064;
    }
    case 0x18a0: { // sei
        pm_irq(s, true);
        return 6306;
    }
    case 0x18a2: { // rcall .+3754
        s->calls[s->call_depth++] = 6308;
        return 10062;
    }
    case 0x18a4: { // ldi r16, 0x20
        s->r[16] = 32;
        return 6310;
    }
    case 0x18a6: { // call 0x28ac
        s->calls[s->call_depth++] = 6314;
        return 10412;
    }
    case 0x18aa: { // inc r18
        s->r[18]++;
        pm_nzv(s, s->r[18], s->r[18] == 128);
        return 6316;
    }
    case 0x18ac: { // cli
        pm_irq(s, false);
        return 6318;
    }
    case 0x18ae: { // rcall .+2744
        s->calls[s->call_depth++] = 6320;
        return 9064;
    }
    case 0x18b0: { // sei
        pm_irq(s, true);
        return 6322;
    }
    case 0x18b2: { // rcall .+3738
        s->calls[s->call_depth++] = 6324;
        return 10062;
    }
    case 0x18b4: { // rcall .+3944
        s->calls[s->call_depth++] = 6326;
        return 10270;
    }
    case 0x18b6: { // inc r18
        s->r[18]++;
        pm_nzv(s, s->r[18], s->r[18] == 128);
        return 6328;
    }
    case 0x18b8: { // cpi r18, 0x7C
        pm_sub(s, s->r[18], 124, 0, false);
        return 6330;
    }
    case 0x18ba: { // brcs .-32
        return (pm_getflag(s, 0) == 1) ? 6300 : 6332;
    }
    case 0x18bc: { // rjmp .+1828
        return 8162;
    }
    default: return UINT32_MAX;
    }
}
