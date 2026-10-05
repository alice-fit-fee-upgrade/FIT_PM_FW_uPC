#ifndef PM_LEGACY_CLI_GUARD_C_H
#define PM_LEGACY_CLI_GUARD_C_H
#include "legacy_cli.h"

/* Opt-in C return from the private carry-result guard. The short call and
 * carry branch remain exact ASM; the compiler emits the ordinary RET.
 * Use only in a void legacy entry after whole-FLASH exact-check. */
#undef PM_FPGA_GUARD
#define PM_FPGA_GUARD(result) do { \
    __label__ ready; \
    asm goto("rcall FUN_code_00108e\n\tbrcc %l[ready]" \
        : : "r" (result) : "r16", "r30", "r31", "memory", "cc" : ready); \
    return; \
ready:; \
} while (0)
/* A caller which already performed the short call can retain the original
 * carry branch and express only its early RET in C. */
#define PM_RETURN_UNLESS_CARRY_CLEAR() do { \
    __label__ ready; \
    asm goto("brcc %l[ready]" : : : "memory", "cc" : ready); \
    return; \
ready:; \
} while (0)
#endif

/* BEGIN COMPILED LOGICAL C EQUIVALENT
 * Validation: PASS_INSTRUCTION_TRANSITIONS: 3808 file cases; shared exhaustive operand tests also passed.
 * Logical C equivalent: explicit private registers, SREG, RAM/MMIO and control flow.
 * Compiled verbatim and differentially tested by tests/check_logical_comments.py.
 * PASS applies only when docs/logical_c_validation.json matches this model hash.
 * Scope: every instruction transition, not timing/async IRQ or whole-path coverage.
 * Calls return the next PC to a dispatcher; callbacks/callees retain the private ABI.
 * Runtime/helper definitions: tests/logical_c_runtime.h. RETI restores I architecturally.
 * This is explanatory C, not a proposed GNU ABI replacement or a binary acceptance.
 *
uint32_t pm_logical_legacy_cli_guard_c(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x0242: { // rcall .+2340
        s->calls[s->call_depth++] = 580;
        return 2920;
    }
    case 0x03bc: { // rcall .+174
        s->calls[s->call_depth++] = 958;
        return 1132;
    }
    case 0x04f8: { // rcall .+1168
        s->calls[s->call_depth++] = 1274;
        return 2442;
    }
    case 0x0694: { // rcall .+1512
        s->calls[s->call_depth++] = 1686;
        return 3198;
    }
    case 0x06a8: { // rcall .+1638
        s->calls[s->call_depth++] = 1706;
        return 3344;
    }
    case 0x06bc: { // rcall .+1822
        s->calls[s->call_depth++] = 1726;
        return 3548;
    }
    case 0x06f6: { // rcall .+742
        s->calls[s->call_depth++] = 1784;
        return 2526;
    }
    case 0x072e: { // rcall .+436
        s->calls[s->call_depth++] = 1840;
        return 2276;
    }
    case 0x07ca: { // rcall .+1202
        s->calls[s->call_depth++] = 1996;
        return 3198;
    }
    case 0x084a: { // brcc .+10
        return (pm_getflag(s, 0) == 0) ? 2134 : 2124;
    }
    case 0x084e: { // brcc .+102
        return (pm_getflag(s, 0) == 0) ? 2230 : 2128;
    }
    case 0x0bd2: { // rcall .+194
        s->calls[s->call_depth++] = 3028;
        return 3222;
    }
    case 0x0c16: { // rcall .+126
        s->calls[s->call_depth++] = 3096;
        return 3222;
    }
    case 0x0c1c: { // rcall .+96
        s->calls[s->call_depth++] = 3102;
        return 3198;
    }
    case 0x1158: { // rcall .+400
        s->calls[s->call_depth++] = 4442;
        return 4842;
    }
    case 0x11b4: { // brcc .+14
        return (pm_getflag(s, 0) == 0) ? 4548 : 4534;
    }
    case 0x11bc: { // brcc .+26
        return (pm_getflag(s, 0) == 0) ? 4568 : 4542;
    }
    case 0x11e6: { // rcall .+3774
        s->calls[s->call_depth++] = 4584;
        return 8358;
    }
    case 0x1226: { // rcall .+3780
        s->calls[s->call_depth++] = 4648;
        return 8428;
    }
    case 0x1266: { // rcall .+3740
        s->calls[s->call_depth++] = 4712;
        return 8452;
    }
    case 0x1294: { // rcall .+3642
        s->calls[s->call_depth++] = 4758;
        return 8400;
    }
    case 0x12d2: { // rcall .+3538
        s->calls[s->call_depth++] = 4820;
        return 8358;
    }
    case 0x1480: { // rcall .+3226
        s->calls[s->call_depth++] = 5250;
        return 8476;
    }
    case 0x14ea: { // rcall .+3120
        s->calls[s->call_depth++] = 5356;
        return 8476;
    }
    case 0x14fe: { // rcall .+3598
        s->calls[s->call_depth++] = 5376;
        return 8974;
    }
    case 0x1504: { // rcall .+116
        s->calls[s->call_depth++] = 5382;
        return 5498;
    }
    case 0x1542: { // rcall .-2222
        s->calls[s->call_depth++] = 5444;
        return 3222;
    }
    case 0x15a6: { // rcall .-46
        s->calls[s->call_depth++] = 5544;
        return 5498;
    }
    case 0x15b0: { // rcall .+178
        s->calls[s->call_depth++] = 5554;
        return 5732;
    }
    case 0x15c0: { // rcall .+638
        s->calls[s->call_depth++] = 5570;
        return 6208;
    }
    case 0x15fa: { // rcall .+276
        s->calls[s->call_depth++] = 5628;
        return 5904;
    }
    case 0x1640: { // rcall .+248
        s->calls[s->call_depth++] = 5698;
        return 5946;
    }
    case 0x1642: { // rcall .+110
        s->calls[s->call_depth++] = 5700;
        return 5810;
    }
    case 0x164a: { // rcall .+238
        s->calls[s->call_depth++] = 5708;
        return 5946;
    }
    case 0x164e: { // rcall .+234
        s->calls[s->call_depth++] = 5712;
        return 5946;
    }
    case 0x1650: { // rcall .+288
        s->calls[s->call_depth++] = 5714;
        return 6002;
    }
    case 0x1660: { // rcall .+28
        s->calls[s->call_depth++] = 5730;
        return 5758;
    }
    case 0x167c: { // ret
        return s->calls[--s->call_depth];
    }
    case 0x16ac: { // rcall .-3090
        s->calls[s->call_depth++] = 5806;
        return 2716;
    }
    case 0x16b6: { // rcall .+336
        s->calls[s->call_depth++] = 5816;
        return 6152;
    }
    case 0x1710: { // rcall .+12
        s->calls[s->call_depth++] = 5906;
        return 5918;
    }
    case 0x1714: { // rcall .+242
        s->calls[s->call_depth++] = 5910;
        return 6152;
    }
    case 0x17d8: { // rcall .+10
        s->calls[s->call_depth++] = 6106;
        return 6116;
    }
    case 0x17ee: { // brcc .+6
        return (pm_getflag(s, 0) == 0) ? 6134 : 6128;
    }
    case 0x17f8: { // brcc .+8
        return (pm_getflag(s, 0) == 0) ? 6146 : 6138;
    }
    case 0x188e: { // rcall .+4012
        s->calls[s->call_depth++] = 6288;
        return 10300;
    }
    case 0x1894: { // rcall .+2182
        s->calls[s->call_depth++] = 6294;
        return 8476;
    }
    case 0x189e: { // rcall .+2760
        s->calls[s->call_depth++] = 6304;
        return 9064;
    }
    case 0x18a2: { // rcall .+3754
        s->calls[s->call_depth++] = 6308;
        return 10062;
    }
    case 0x18ae: { // rcall .+2744
        s->calls[s->call_depth++] = 6320;
        return 9064;
    }
    case 0x18b2: { // rcall .+3738
        s->calls[s->call_depth++] = 6324;
        return 10062;
    }
    case 0x18b4: { // rcall .+3944
        s->calls[s->call_depth++] = 6326;
        return 10270;
    }
    case 0x18be: { // rcall .+3964
        s->calls[s->call_depth++] = 6336;
        return 10300;
    }
    case 0x18c4: { // rcall .+2134
        s->calls[s->call_depth++] = 6342;
        return 8476;
    }
    case 0x18d2: { // rcall .+2708
        s->calls[s->call_depth++] = 6356;
        return 9064;
    }
    case 0x18d6: { // rcall .+3702
        s->calls[s->call_depth++] = 6360;
        return 10062;
    }
    case 0x18da: { // rcall .+4048
        s->calls[s->call_depth++] = 6364;
        return 10412;
    }
    case 0x18e0: { // rcall .+2694
        s->calls[s->call_depth++] = 6370;
        return 9064;
    }
    case 0x18e4: { // rcall .+3688
        s->calls[s->call_depth++] = 6374;
        return 10062;
    }
    case 0x18e8: { // rcall .+4034
        s->calls[s->call_depth++] = 6378;
        return 10412;
    }
    case 0x18f0: { // rcall .+2678
        s->calls[s->call_depth++] = 6386;
        return 9064;
    }
    case 0x18f4: { // rcall .+3672
        s->calls[s->call_depth++] = 6390;
        return 10062;
    }
    case 0x18f8: { // rcall .+4018
        s->calls[s->call_depth++] = 6394;
        return 10412;
    }
    case 0x18fe: { // rcall .+2664
        s->calls[s->call_depth++] = 6400;
        return 9064;
    }
    case 0x1902: { // rcall .+3658
        s->calls[s->call_depth++] = 6404;
        return 10062;
    }
    case 0x1904: { // rcall .+3864
        s->calls[s->call_depth++] = 6406;
        return 10270;
    }
    case 0x1910: { // rcall .+3882
        s->calls[s->call_depth++] = 6418;
        return 10300;
    }
    case 0x1920: { // rcall .+2630
        s->calls[s->call_depth++] = 6434;
        return 9064;
    }
    case 0x192c: { // rcall .+3608
        s->calls[s->call_depth++] = 6446;
        return 10054;
    }
    case 0x1930: { // rcall .+3962
        s->calls[s->call_depth++] = 6450;
        return 10412;
    }
    case 0x193a: { // rcall .+3594
        s->calls[s->call_depth++] = 6460;
        return 10054;
    }
    case 0x193e: { // rcall .+3948
        s->calls[s->call_depth++] = 6464;
        return 10412;
    }
    case 0x1944: { // rcall .+2594
        s->calls[s->call_depth++] = 6470;
        return 9064;
    }
    case 0x194e: { // rcall .+3574
        s->calls[s->call_depth++] = 6480;
        return 10054;
    }
    case 0x1950: { // rcall .+3788
        s->calls[s->call_depth++] = 6482;
        return 10270;
    }
    case 0x195e: { // rcall .+2568
        s->calls[s->call_depth++] = 6496;
        return 9064;
    }
    case 0x1962: { // rcall .+3476
        s->calls[s->call_depth++] = 6500;
        return 9976;
    }
    case 0x1970: { // brcc .+4
        return (pm_getflag(s, 0) == 0) ? 6518 : 6514;
    }
    case 0x1982: { // rcall .+3880
        s->calls[s->call_depth++] = 6532;
        return 10412;
    }
    case 0x1986: { // rcall .+3518
        s->calls[s->call_depth++] = 6536;
        return 10054;
    }
    case 0x198a: { // rcall .+3872
        s->calls[s->call_depth++] = 6540;
        return 10412;
    }
    case 0x199a: { // rcall .+3498
        s->calls[s->call_depth++] = 6556;
        return 10054;
    }
    case 0x199e: { // rcall .+3852
        s->calls[s->call_depth++] = 6560;
        return 10412;
    }
    case 0x19a0: { // rcall .+3708
        s->calls[s->call_depth++] = 6562;
        return 10270;
    }
    case 0x19a8: { // rcall .+3700
        s->calls[s->call_depth++] = 6570;
        return 10270;
    }
    case 0x19ae: { // rcall .+3724
        s->calls[s->call_depth++] = 6576;
        return 10300;
    }
    case 0x1a08: { // rcall .+4
        s->calls[s->call_depth++] = 6666;
        return 6670;
    }
    case 0x1a3e: { // rcall .+3580
        s->calls[s->call_depth++] = 6720;
        return 10300;
    }
    case 0x1a54: { // rcall .+2608
        s->calls[s->call_depth++] = 6742;
        return 9350;
    }
    case 0x1a60: { // rcall .+3524
        s->calls[s->call_depth++] = 6754;
        return 10278;
    }
    case 0x1a64: { // rcall .+3542
        s->calls[s->call_depth++] = 6758;
        return 10300;
    }
    case 0x1a6e: { // rcall .+3510
        s->calls[s->call_depth++] = 6768;
        return 10278;
    }
    case 0x1a78: { // rcall .+3198
        s->calls[s->call_depth++] = 6778;
        return 9976;
    }
    case 0x1a7e: { // rcall .+3494
        s->calls[s->call_depth++] = 6784;
        return 10278;
    }
    case 0x1a8c: { // rcall .+3178
        s->calls[s->call_depth++] = 6798;
        return 9976;
    }
    case 0x1a90: { // rcall .+3174
        s->calls[s->call_depth++] = 6802;
        return 9976;
    }
    case 0x1a96: { // rcall .+3470
        s->calls[s->call_depth++] = 6808;
        return 10278;
    }
    case 0x1ab8: { // rcall .+3436
        s->calls[s->call_depth++] = 6842;
        return 10278;
    }
    case 0x1ac0: { // rcall .+3428
        s->calls[s->call_depth++] = 6850;
        return 10278;
    }
    case 0x1ac2: { // rcall .+3186
        s->calls[s->call_depth++] = 6852;
        return 10038;
    }
    case 0x1ae0: { // rcall .+3396
        s->calls[s->call_depth++] = 6882;
        return 10278;
    }
    case 0x1ae6: { // rcall .+3390
        s->calls[s->call_depth++] = 6888;
        return 10278;
    }
    case 0x1af0: { // rcall .+3380
        s->calls[s->call_depth++] = 6898;
        return 10278;
    }
    case 0x1af8: { // rcall .+3372
        s->calls[s->call_depth++] = 6906;
        return 10278;
    }
    case 0x1b06: { // rcall .+3358
        s->calls[s->call_depth++] = 6920;
        return 10278;
    }
    case 0x1b12: { // rcall .+3346
        s->calls[s->call_depth++] = 6932;
        return 10278;
    }
    case 0x1b18: { // rcall .+3340
        s->calls[s->call_depth++] = 6938;
        return 10278;
    }
    case 0x1b32: { // rcall .+3314
        s->calls[s->call_depth++] = 6964;
        return 10278;
    }
    case 0x1b38: { // rcall .+3308
        s->calls[s->call_depth++] = 6970;
        return 10278;
    }
    case 0x1b46: { // rcall .+3294
        s->calls[s->call_depth++] = 6984;
        return 10278;
    }
    case 0x1b56: { // rcall .+3278
        s->calls[s->call_depth++] = 7000;
        return 10278;
    }
    case 0x1b5c: { // rcall .+3272
        s->calls[s->call_depth++] = 7006;
        return 10278;
    }
    case 0x1b6a: { // rcall .+3258
        s->calls[s->call_depth++] = 7020;
        return 10278;
    }
    case 0x1b7a: { // rcall .+3242
        s->calls[s->call_depth++] = 7036;
        return 10278;
    }
    case 0x1b80: { // rcall .+3370
        s->calls[s->call_depth++] = 7042;
        return 10412;
    }
    case 0x1b86: { // rcall .+3230
        s->calls[s->call_depth++] = 7048;
        return 10278;
    }
    case 0x1b8a: { // brcc .+86
        return (pm_getflag(s, 0) == 0) ? 7138 : 7052;
    }
    case 0x1b92: { // rcall .+3218
        s->calls[s->call_depth++] = 7060;
        return 10278;
    }
    case 0x1b98: { // rcall .+3212
        s->calls[s->call_depth++] = 7066;
        return 10278;
    }
    case 0x1bb0: { // rcall .+3188
        s->calls[s->call_depth++] = 7090;
        return 10278;
    }
    case 0x1bb6: { // rcall .+3182
        s->calls[s->call_depth++] = 7096;
        return 10278;
    }
    case 0x1bc0: { // rcall .+3172
        s->calls[s->call_depth++] = 7106;
        return 10278;
    }
    case 0x1bca: { // rcall .+3162
        s->calls[s->call_depth++] = 7116;
        return 10278;
    }
    case 0x1bd2: { // rcall .+3154
        s->calls[s->call_depth++] = 7124;
        return 10278;
    }
    case 0x1be0: { // rcall .+3140
        s->calls[s->call_depth++] = 7138;
        return 10278;
    }
    case 0x1be4: { // rcall .+3158
        s->calls[s->call_depth++] = 7142;
        return 10300;
    }
    case 0x1c5c: { // rcall .+3016
        s->calls[s->call_depth++] = 7262;
        return 10278;
    }
    case 0x1c62: { // rcall .+3032
        s->calls[s->call_depth++] = 7268;
        return 10300;
    }
    case 0x1c70: { // rcall .+2996
        s->calls[s->call_depth++] = 7282;
        return 10278;
    }
    case 0x1c76: { // rcall .+2774
        s->calls[s->call_depth++] = 7288;
        return 10062;
    }
    case 0x1c78: { // rcall .+2988
        s->calls[s->call_depth++] = 7290;
        return 10278;
    }
    case 0x1c8a: { // rcall .+2754
        s->calls[s->call_depth++] = 7308;
        return 10062;
    }
    case 0x1c8c: { // rcall .+2968
        s->calls[s->call_depth++] = 7310;
        return 10278;
    }
    case 0x1ca2: { // rcall .+1380
        s->calls[s->call_depth++] = 7332;
        return 8712;
    }
    case 0x1cae: { // rcall .+2710
        s->calls[s->call_depth++] = 7344;
        return 10054;
    }
    case 0x1cb0: { // rcall .+2932
        s->calls[s->call_depth++] = 7346;
        return 10278;
    }
    case 0x1cc2: { // rcall .+2690
        s->calls[s->call_depth++] = 7364;
        return 10054;
    }
    case 0x1cc4: { // rcall .+2912
        s->calls[s->call_depth++] = 7366;
        return 10278;
    }
    case 0x1cd8: { // rcall .+2676
        s->calls[s->call_depth++] = 7386;
        return 10062;
    }
    case 0x1cdc: { // rcall .+3022
        s->calls[s->call_depth++] = 7390;
        return 10412;
    }
    case 0x1ce2: { // rcall .+2666
        s->calls[s->call_depth++] = 7396;
        return 10062;
    }
    case 0x1ce4: { // rcall .+2872
        s->calls[s->call_depth++] = 7398;
        return 10270;
    }
    case 0x1cf2: { // rcall .+2888
        s->calls[s->call_depth++] = 7412;
        return 10300;
    }
    case 0x1d02: { // rcall .+2850
        s->calls[s->call_depth++] = 7428;
        return 10278;
    }
    case 0x1d08: { // rcall .+2628
        s->calls[s->call_depth++] = 7434;
        return 10062;
    }
    case 0x1d0a: { // rcall .+2842
        s->calls[s->call_depth++] = 7436;
        return 10278;
    }
    case 0x1d14: { // rcall .+2584
        s->calls[s->call_depth++] = 7446;
        return 10030;
    }
    case 0x1d16: { // rcall .+2830
        s->calls[s->call_depth++] = 7448;
        return 10278;
    }
    case 0x1d20: { // rcall .+2572
        s->calls[s->call_depth++] = 7458;
        return 10030;
    }
    case 0x1d22: { // rcall .+2818
        s->calls[s->call_depth++] = 7460;
        return 10278;
    }
    case 0x1d2c: { // rcall .+2560
        s->calls[s->call_depth++] = 7470;
        return 10030;
    }
    case 0x1d2e: { // rcall .+2806
        s->calls[s->call_depth++] = 7472;
        return 10278;
    }
    case 0x1d38: { // rcall .+2564
        s->calls[s->call_depth++] = 7482;
        return 10046;
    }
    case 0x1d3a: { // rcall .+2786
        s->calls[s->call_depth++] = 7484;
        return 10270;
    }
    case 0x1d46: { // rcall .+2782
        s->calls[s->call_depth++] = 7496;
        return 10278;
    }
    case 0x1d4e: { // rcall .+2558
        s->calls[s->call_depth++] = 7504;
        return 10062;
    }
    case 0x1d50: { // rcall .+2772
        s->calls[s->call_depth++] = 7506;
        return 10278;
    }
    case 0x1d5a: { // rcall .+2546
        s->calls[s->call_depth++] = 7516;
        return 10062;
    }
    case 0x1d5c: { // rcall .+2752
        s->calls[s->call_depth++] = 7518;
        return 10270;
    }
    case 0x1d62: { // rcall .+2776
        s->calls[s->call_depth++] = 7524;
        return 10300;
    }
    case 0x1d68: { // rcall .+2250
        s->calls[s->call_depth++] = 7530;
        return 9780;
    }
    case 0x1d78: { // rcall .+930
        s->calls[s->call_depth++] = 7546;
        return 8476;
    }
    case 0x1d7a: { // brcc .+2
        return (pm_getflag(s, 0) == 0) ? 7550 : 7548;
    }
    case 0x1d8c: { // rcall .+1408
        s->calls[s->call_depth++] = 7566;
        return 8974;
    }
    case 0x1d92: { // rcall .+2728
        s->calls[s->call_depth++] = 7572;
        return 10300;
    }
    case 0x1d98: { // rcall .+2202
        s->calls[s->call_depth++] = 7578;
        return 9780;
    }
    case 0x1da0: { // rcall .+890
        s->calls[s->call_depth++] = 7586;
        return 8476;
    }
    case 0x1db0: { // rcall .+1372
        s->calls[s->call_depth++] = 7602;
        return 8974;
    }
    case 0x1db6: { // rcall .+2172
        s->calls[s->call_depth++] = 7608;
        return 9780;
    }
    case 0x1dc8: { // rcall .+2154
        s->calls[s->call_depth++] = 7626;
        return 9780;
    }
    case 0x1dd2: { // rcall .+2144
        s->calls[s->call_depth++] = 7636;
        return 9780;
    }
    case 0x1df6: { // rcall .+804
        s->calls[s->call_depth++] = 7672;
        return 8476;
    }
    case 0x1e1c: { // rcall .+1264
        s->calls[s->call_depth++] = 7710;
        return 8974;
    }
    case 0x1e26: { // rcall .+1254
        s->calls[s->call_depth++] = 7720;
        return 8974;
    }
    case 0x1e2e: { // rcall .+2052
        s->calls[s->call_depth++] = 7728;
        return 9780;
    }
    case 0x1e40: { // rcall .+2034
        s->calls[s->call_depth++] = 7746;
        return 9780;
    }
    case 0x1e54: { // rcall .+710
        s->calls[s->call_depth++] = 7766;
        return 8476;
    }
    case 0x1e74: { // rcall .+1176
        s->calls[s->call_depth++] = 7798;
        return 8974;
    }
    case 0x1e7c: { // rcall .+1974
        s->calls[s->call_depth++] = 7806;
        return 9780;
    }
    case 0x1e8e: { // rcall .+1956
        s->calls[s->call_depth++] = 7824;
        return 9780;
    }
    case 0x1ea2: { // rcall .+632
        s->calls[s->call_depth++] = 7844;
        return 8476;
    }
    case 0x1eb0: { // rcall .+1116
        s->calls[s->call_depth++] = 7858;
        return 8974;
    }
    case 0x1edc: { // rcall .+456
        s->calls[s->call_depth++] = 7902;
        return 8358;
    }
    case 0x1ee0: { // rcall .+1874
        s->calls[s->call_depth++] = 7906;
        return 9780;
    }
    case 0x1ef2: { // rcall .+1856
        s->calls[s->call_depth++] = 7924;
        return 9780;
    }
    case 0x1f0a: { // rcall .+528
        s->calls[s->call_depth++] = 7948;
        return 8476;
    }
    case 0x1f32: { // rcall .+576
        s->calls[s->call_depth++] = 7988;
        return 8564;
    }
    case 0x1f3a: { // rcall .+1784
        s->calls[s->call_depth++] = 7996;
        return 9780;
    }
    case 0x1f4c: { // rcall .+1766
        s->calls[s->call_depth++] = 8014;
        return 9780;
    }
    case 0x1f60: { // rcall .+442
        s->calls[s->call_depth++] = 8034;
        return 8476;
    }
    case 0x1f74: { // rcall .+920
        s->calls[s->call_depth++] = 8054;
        return 8974;
    }
    case 0x1f8a: { // rcall .+324
        s->calls[s->call_depth++] = 8076;
        return 8400;
    }
    case 0x1f8e: { // rcall .+1700
        s->calls[s->call_depth++] = 8080;
        return 9780;
    }
    case 0x1fa0: { // rcall .+1682
        s->calls[s->call_depth++] = 8098;
        return 9780;
    }
    case 0x1fb8: { // rcall .+354
        s->calls[s->call_depth++] = 8122;
        return 8476;
    }
    case 0x1fcc: { // rcall .+832
        s->calls[s->call_depth++] = 8142;
        return 8974;
    }
    case 0x1fe0: { // rcall .+196
        s->calls[s->call_depth++] = 8162;
        return 8358;
    }
    case 0x1fe6: { // rcall .+2110
        s->calls[s->call_depth++] = 8168;
        return 10278;
    }
    case 0x1fec: { // rcall .+1606
        s->calls[s->call_depth++] = 8174;
        return 9780;
    }
    case 0x1ffe: { // rcall .+1588
        s->calls[s->call_depth++] = 8192;
        return 9780;
    }
    case 0x2016: { // rcall .+260
        s->calls[s->call_depth++] = 8216;
        return 8476;
    }
    case 0x202c: { // rcall .+736
        s->calls[s->call_depth++] = 8238;
        return 8974;
    }
    case 0x2046: { // rcall .+188
        s->calls[s->call_depth++] = 8264;
        return 8452;
    }
    case 0x204e: { // rcall .+1508
        s->calls[s->call_depth++] = 8272;
        return 9780;
    }
    case 0x2060: { // rcall .+1490
        s->calls[s->call_depth++] = 8290;
        return 9780;
    }
    case 0x2078: { // rcall .+162
        s->calls[s->call_depth++] = 8314;
        return 8476;
    }
    case 0x208c: { // rcall .+640
        s->calls[s->call_depth++] = 8334;
        return 8974;
    }
    case 0x20a2: { // rcall .+72
        s->calls[s->call_depth++] = 8356;
        return 8428;
    }
    case 0x20ac: { // rcall .+158
        s->calls[s->call_depth++] = 8366;
        return 8524;
    }
    case 0x20ca: { // rcall .+478
        s->calls[s->call_depth++] = 8396;
        return 8874;
    }
    case 0x20e0: { // rcall .+106
        s->calls[s->call_depth++] = 8418;
        return 8524;
    }
    case 0x20e6: { // rcall .+450
        s->calls[s->call_depth++] = 8424;
        return 8874;
    }
    case 0x20f2: { // rcall .+60
        s->calls[s->call_depth++] = 8436;
        return 8496;
    }
    case 0x20fe: { // rcall .+426
        s->calls[s->call_depth++] = 8448;
        return 8874;
    }
    case 0x210a: { // rcall .+36
        s->calls[s->call_depth++] = 8460;
        return 8496;
    }
    case 0x2116: { // rcall .+402
        s->calls[s->call_depth++] = 8472;
        return 8874;
    }
    case 0x212a: { // rcall .+1786
        s->calls[s->call_depth++] = 8492;
        return 10278;
    }
    case 0x24da: { // rcall .-86
        s->calls[s->call_depth++] = 9436;
        return 9350;
    }
    case 0x25a8: { // rcall .+94
        s->calls[s->call_depth++] = 9642;
        return 9736;
    }
    case 0x25ac: { // rcall .+90
        s->calls[s->call_depth++] = 9646;
        return 9736;
    }
    case 0x25ce: { // rcall .+56
        s->calls[s->call_depth++] = 9680;
        return 9736;
    }
    case 0x25d2: { // rcall .+52
        s->calls[s->call_depth++] = 9684;
        return 9736;
    }
    case 0x25d8: { // rcall .+46
        s->calls[s->call_depth++] = 9690;
        return 9736;
    }
    case 0x25f2: { // rcall .+20
        s->calls[s->call_depth++] = 9716;
        return 9736;
    }
    case 0x25f6: { // rcall .+16
        s->calls[s->call_depth++] = 9720;
        return 9736;
    }
    case 0x25fe: { // rcall .+8
        s->calls[s->call_depth++] = 9728;
        return 9736;
    }
    case 0x264a: { // rcall .+496
        s->calls[s->call_depth++] = 9804;
        return 10300;
    }
    case 0x267e: { // brcc .-54
        return (pm_getflag(s, 0) == 0) ? 9802 : 9856;
    }
    case 0x26b6: { // rcall .+388
        s->calls[s->call_depth++] = 9912;
        return 10300;
    }
    case 0x26c8: { // brcc .+30
        return (pm_getflag(s, 0) == 0) ? 9960 : 9930;
    }
    case 0x26e6: { // rcall .+340
        s->calls[s->call_depth++] = 9960;
        return 10300;
    }
    case 0x2704: { // rcall .+26
        s->calls[s->call_depth++] = 9990;
        return 10016;
    }
    case 0x2710: { // rcall .+14
        s->calls[s->call_depth++] = 10002;
        return 10016;
    }
    case 0x272a: { // rcall .+384
        s->calls[s->call_depth++] = 10028;
        return 10412;
    }
    case 0x2768: { // brcc .+16
        return (pm_getflag(s, 0) == 0) ? 10106 : 10090;
    }
    case 0x2804: { // rcall .+166
        s->calls[s->call_depth++] = 10246;
        return 10412;
    }
    case 0x2822: { // rcall .+2
        s->calls[s->call_depth++] = 10276;
        return 10278;
    }
    case 0x282e: { // rcall .+124
        s->calls[s->call_depth++] = 10288;
        return 10412;
    }
    default: return UINT32_MAX;
    }
}
END COMPILED LOGICAL C EQUIVALENT */
