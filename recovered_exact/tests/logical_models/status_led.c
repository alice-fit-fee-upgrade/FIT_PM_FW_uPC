uint32_t pm_logical_status_led(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x0c7e: { // sbic 0x00, 1
        return (!!(pm_io_read(s, 0) & (1u << 1)) == 0) ? 3202 : 3200;
    }
    case 0x0c80: { // rjmp .+10
        return 3212;
    }
    case 0x0c82: { // cbi 0x00, 0
        uint8_t value = pm_io_read(s, 0);
        pm_io_write(s, 0, value & ~ (1u << 0));
        return 3204;
    }
    case 0x0c84: { // ldi r16, 0x01
        s->r[16] = 1;
        return 3206;
    }
    case 0x0c86: { // sts 0x0605, r16
        uint16_t address = 1541;
        pm_write(s, address, s->r[16]);
        return 3210;
    }
    case 0x0c8a: { // ret
        return s->calls[--s->call_depth];
    }
    case 0x0c8c: { // ldi r16, 0x01
        s->r[16] = 1;
        return 3214;
    }
    case 0x0c8e: { // sts 0x0606, r16
        uint16_t address = 1542;
        pm_write(s, address, s->r[16]);
        return 3218;
    }
    case 0x0c92: { // cbi 0x00, 0
        uint8_t value = pm_io_read(s, 0);
        pm_io_write(s, 0, value & ~ (1u << 0));
        return 3220;
    }
    case 0x0c94: { // ret
        return s->calls[--s->call_depth];
    }
    default: return UINT32_MAX;
    }
}
