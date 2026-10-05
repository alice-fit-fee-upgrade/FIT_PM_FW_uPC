#ifndef PM_LEGACY_CLI_H
#define PM_LEGACY_CLI_H
#include <stdint.h>
/* Private parser ABI: R20:R21 is the result, R16 is its following delimiter.
 * Preserve historical signed range branches and shared error entries exactly. */
#define PM_PARSE_CHANNEL(result, error) \
    asm volatile("rcall cli_get_integer\n\tbrcs " error "\n\ttst r21\n\tbrne " error \
        "\n\tcpi r20, 12\n\tbrge " error : "=r" (result) : : "r16", "memory", "cc")
#define PM_PARSE_VALUE(result, error) \
    asm volatile("cpi r16, 0x20\n\tbrne " error "\n\trcall cli_get_integer\n\tbrcs " error \
        "\n\tcpi r16, 13\n\tbrne " error : "=r" (result) : : "r16", "memory", "cc")
#define PM_FPGA_GUARD(result) \
    asm volatile("rcall FUN_code_00108e\n\tbrcc 1f\n\tret\n1:" \
        : : "r" (result) : "r16", "r30", "r31", "memory", "cc")
#endif

/* BEGIN COMPILED LOGICAL C EQUIVALENT
 * Validation: PASS_INSTRUCTION_TRANSITIONS: 7104 file cases; shared exhaustive operand tests also passed.
 * Logical C equivalent: explicit private registers, SREG, RAM/MMIO and control flow.
 * Compiled verbatim and differentially tested by tests/check_logical_comments.py.
 * PASS applies only when docs/logical_c_validation.json matches this model hash.
 * Scope: every instruction transition, not timing/async IRQ or whole-path coverage.
 * Calls return the next PC to a dispatcher; callbacks/callees retain the private ABI.
 * Runtime/helper definitions: tests/logical_c_runtime.h. RETI restores I architecturally.
 * This is explanatory C, not a proposed GNU ABI replacement or a binary acceptance.
 *
uint32_t pm_logical_legacy_cli(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x0200: { // brne .-12
        return (pm_getflag(s, 1) == 0) ? 502 : 514;
    }
    case 0x0220: { // cpi r16, 0xAA
        pm_sub(s, s->r[16], 170, 0, false);
        return 546;
    }
    case 0x022c: { // cpi r17, 0x07
        pm_sub(s, s->r[17], 7, 0, false);
        return 558;
    }
    case 0x0232: { // cpi r28, 0x41
        pm_sub(s, s->r[28], 65, 0, false);
        return 564;
    }
    case 0x0242: { // rcall .+2340
        s->calls[s->call_depth++] = 580;
        return 2920;
    }
    case 0x026a: { // brne .-72
        return (pm_getflag(s, 1) == 0) ? 548 : 620;
    }
    case 0x03bc: { // rcall .+174
        s->calls[s->call_depth++] = 958;
        return 1132;
    }
    case 0x03d2: { // and r31, r31
        s->r[31] &= s->r[31];
        pm_nzv(s, s->r[31], false);
        return 980;
    }
    case 0x0410: { // brcs .+12
        return (pm_getflag(s, 0) == 1) ? 1054 : 1042;
    }
    case 0x04ec: { // cpi r16, 0x0F
        pm_sub(s, s->r[16], 15, 0, false);
        return 1262;
    }
    case 0x04f8: { // rcall .+1168
        s->calls[s->call_depth++] = 1274;
        return 2442;
    }
    case 0x0540: { // cpi r16, 0xC4
        pm_sub(s, s->r[16], 196, 0, false);
        return 1346;
    }
    case 0x0564: { // cpi r17, 0x01
        pm_sub(s, s->r[17], 1, 0, false);
        return 1382;
    }
    case 0x0566: { // brne .+8
        return (pm_getflag(s, 1) == 0) ? 1392 : 1384;
    }
    case 0x0624: { // cpi r16, 0x8C
        pm_sub(s, s->r[16], 140, 0, false);
        return 1574;
    }
    case 0x064a: { // brne .+98
        return (pm_getflag(s, 1) == 0) ? 1710 : 1612;
    }
    case 0x067c: { // brne .+54
        return (pm_getflag(s, 1) == 0) ? 1716 : 1662;
    }
    case 0x0694: { // rcall .+1512
        s->calls[s->call_depth++] = 1686;
        return 3198;
    }
    case 0x06a8: { // rcall .+1638
        s->calls[s->call_depth++] = 1706;
        return 3344;
    }
    case 0x06b4: { // cpi r16, 0x03
        pm_sub(s, s->r[16], 3, 0, false);
        return 1718;
    }
    case 0x06bc: { // rcall .+1822
        s->calls[s->call_depth++] = 1726;
        return 3548;
    }
    case 0x06d8: { // cpi r16, 0x04
        pm_sub(s, s->r[16], 4, 0, false);
        return 1754;
    }
    case 0x06da: { // brne .+38
        return (pm_getflag(s, 1) == 0) ? 1794 : 1756;
    }
    case 0x06f6: { // rcall .+742
        s->calls[s->call_depth++] = 1784;
        return 2526;
    }
    case 0x06fc: { // brne .-80
        return (pm_getflag(s, 1) == 0) ? 1710 : 1790;
    }
    case 0x072e: { // rcall .+436
        s->calls[s->call_depth++] = 1840;
        return 2276;
    }
    case 0x0736: { // cpi r24, 0xF4
        pm_sub(s, s->r[24], 244, 0, false);
        return 1848;
    }
    case 0x073a: { // brne .+24
        return (pm_getflag(s, 1) == 0) ? 1876 : 1852;
    }
    case 0x07ba: { // cpi r17, 0x06
        pm_sub(s, s->r[17], 6, 0, false);
        return 1980;
    }
    case 0x07ca: { // rcall .+1202
        s->calls[s->call_depth++] = 1996;
        return 3198;
    }
    case 0x0848: { // cpi r16, 0x97
        pm_sub(s, s->r[16], 151, 0, false);
        return 2122;
    }
    case 0x084a: { // brcc .+10
        return (pm_getflag(s, 0) == 0) ? 2134 : 2124;
    }
    case 0x084c: { // cpi r20, 0x1E
        pm_sub(s, s->r[20], 30, 0, false);
        return 2126;
    }
    case 0x084e: { // brcc .+102
        return (pm_getflag(s, 0) == 0) ? 2230 : 2128;
    }
    case 0x085c: { // brcs .+88
        return (pm_getflag(s, 0) == 1) ? 2230 : 2142;
    }
    case 0x0890: { // cpi r21, 0x0C
        pm_sub(s, s->r[21], 12, 0, false);
        return 2194;
    }
    case 0x0894: { // brge .+10
        return (pm_getflag(s, 4) == 0) ? 2208 : 2198;
    }
    case 0x091e: { // cpi r18, 0x0C
        pm_sub(s, s->r[18], 12, 0, false);
        return 2336;
    }
    case 0x0936: { // cpi r18, 0x3C
        pm_sub(s, s->r[18], 60, 0, false);
        return 2360;
    }
    case 0x09a8: { // cpi r18, 0xB0
        pm_sub(s, s->r[18], 176, 0, false);
        return 2474;
    }
    case 0x09aa: { // brne .-18
        return (pm_getflag(s, 1) == 0) ? 2458 : 2476;
    }
    case 0x09be: { // cpi r18, 0xBC
        pm_sub(s, s->r[18], 188, 0, false);
        return 2496;
    }
    case 0x0a2c: { // brne .-46
        return (pm_getflag(s, 1) == 0) ? 2560 : 2606;
    }
    case 0x0aaa: { // and r16, r16
        s->r[16] &= s->r[16];
        pm_nzv(s, s->r[16], false);
        return 2732;
    }
    case 0x0aae: { // cpi r16, 0x05
        pm_sub(s, s->r[16], 5, 0, false);
        return 2736;
    }
    case 0x0bd2: { // rcall .+194
        s->calls[s->call_depth++] = 3028;
        return 3222;
    }
    case 0x0bf8: { // cpi r16, 0x02
        pm_sub(s, s->r[16], 2, 0, false);
        return 3066;
    }
    case 0x0bfa: { // brne .+26
        return (pm_getflag(s, 1) == 0) ? 3094 : 3068;
    }
    case 0x0c16: { // rcall .+126
        s->calls[s->call_depth++] = 3096;
        return 3222;
    }
    case 0x0c1c: { // rcall .+96
        s->calls[s->call_depth++] = 3102;
        return 3198;
    }
    case 0x0e02: { // brne .-20
        return (pm_getflag(s, 1) == 0) ? 3568 : 3588;
    }
    case 0x0ea2: { // and r18, r18
        s->r[18] &= s->r[18];
        pm_nzv(s, s->r[18], false);
        return 3748;
    }
    case 0x0ea8: { // brne .+12
        return (pm_getflag(s, 1) == 0) ? 3766 : 3754;
    }
    case 0x0f04: { // brne .+16
        return (pm_getflag(s, 1) == 0) ? 3862 : 3846;
    }
    case 0x0f34: { // cpi r18, 0x0D
        pm_sub(s, s->r[18], 13, 0, false);
        return 3894;
    }
    case 0x0f36: { // brne .+10
        return (pm_getflag(s, 1) == 0) ? 3906 : 3896;
    }
    case 0x1158: { // rcall .+400
        s->calls[s->call_depth++] = 4442;
        return 4842;
    }
    case 0x11ac: { // brne .+60
        return (pm_getflag(s, 1) == 0) ? 4586 : 4526;
    }
    case 0x11b0: { // cpi r16, 0x31
        pm_sub(s, s->r[16], 49, 0, false);
        return 4530;
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
    case 0x11ea: { // cpi r19, 0x01
        pm_sub(s, s->r[19], 1, 0, false);
        return 4588;
    }
    case 0x11f0: { // cpi r16, 0xF5
        pm_sub(s, s->r[16], 245, 0, false);
        return 4594;
    }
    case 0x11f4: { // brge .+14
        return (pm_getflag(s, 4) == 0) ? 4612 : 4598;
    }
    case 0x11f8: { // cpi r16, 0x0C
        pm_sub(s, s->r[16], 12, 0, false);
        return 4602;
    }
    case 0x11fc: { // brge .+26
        return (pm_getflag(s, 4) == 0) ? 4632 : 4606;
    }
    case 0x1226: { // rcall .+3780
        s->calls[s->call_depth++] = 4648;
        return 8428;
    }
    case 0x122a: { // cpi r19, 0x02
        pm_sub(s, s->r[19], 2, 0, false);
        return 4652;
    }
    case 0x1266: { // rcall .+3740
        s->calls[s->call_depth++] = 4712;
        return 8452;
    }
    case 0x126c: { // cpi r16, 0x21
        pm_sub(s, s->r[16], 33, 0, false);
        return 4718;
    }
    case 0x1294: { // rcall .+3642
        s->calls[s->call_depth++] = 4758;
        return 8400;
    }
    case 0x129a: { // cpi r16, 0xA1
        pm_sub(s, s->r[16], 161, 0, false);
        return 4764;
    }
    case 0x129e: { // brcs .+20
        return (pm_getflag(s, 0) == 1) ? 4788 : 4768;
    }
    case 0x12d2: { // rcall .+3538
        s->calls[s->call_depth++] = 4820;
        return 8358;
    }
    case 0x12f4: { // cpi r16, 0x43
        pm_sub(s, s->r[16], 67, 0, false);
        return 4854;
    }
    case 0x12fe: { // brne .+2
        return (pm_getflag(s, 1) == 0) ? 4866 : 4864;
    }
    case 0x1302: { // cpi r16, 0x50
        pm_sub(s, s->r[16], 80, 0, false);
        return 4868;
    }
    case 0x1308: { // cpi r16, 0x4F
        pm_sub(s, s->r[16], 79, 0, false);
        return 4874;
    }
    case 0x1310: { // cpi r16, 0x4E
        pm_sub(s, s->r[16], 78, 0, false);
        return 4882;
    }
    case 0x1316: { // cpi r16, 0x46
        pm_sub(s, s->r[16], 70, 0, false);
        return 4888;
    }
    case 0x1318: { // brne .+72
        return (pm_getflag(s, 1) == 0) ? 4962 : 4890;
    }
    case 0x131e: { // brne .+22
        return (pm_getflag(s, 1) == 0) ? 4918 : 4896;
    }
    case 0x1332: { // brne .+46
        return (pm_getflag(s, 1) == 0) ? 4962 : 4916;
    }
    case 0x1336: { // cpi r16, 0x52
        pm_sub(s, s->r[16], 82, 0, false);
        return 4920;
    }
    case 0x1338: { // brne .+42
        return (pm_getflag(s, 1) == 0) ? 4964 : 4922;
    }
    case 0x134a: { // cpi r16, 0x53
        pm_sub(s, s->r[16], 83, 0, false);
        return 4940;
    }
    case 0x1356: { // cpi r16, 0x54
        pm_sub(s, s->r[16], 84, 0, false);
        return 4952;
    }
    case 0x135c: { // cpi r16, 0x5A
        pm_sub(s, s->r[16], 90, 0, false);
        return 4958;
    }
    case 0x1366: { // brne .+78
        return (pm_getflag(s, 1) == 0) ? 5046 : 4968;
    }
    case 0x136e: { // brne .+28
        return (pm_getflag(s, 1) == 0) ? 5004 : 4976;
    }
    case 0x1374: { // cpi r16, 0x4C
        pm_sub(s, s->r[16], 76, 0, false);
        return 4982;
    }
    case 0x138c: { // cpi r16, 0x44
        pm_sub(s, s->r[16], 68, 0, false);
        return 5006;
    }
    case 0x13aa: { // cpi r16, 0x56
        pm_sub(s, s->r[16], 86, 0, false);
        return 5036;
    }
    case 0x13b2: { // brne .+20
        return (pm_getflag(s, 1) == 0) ? 5064 : 5044;
    }
    case 0x13b6: { // cpi r16, 0x57
        pm_sub(s, s->r[16], 87, 0, false);
        return 5048;
    }
    case 0x13b8: { // brne .+14
        return (pm_getflag(s, 1) == 0) ? 5064 : 5050;
    }
    case 0x1442: { // brne .+44
        return (pm_getflag(s, 1) == 0) ? 5232 : 5188;
    }
    case 0x1480: { // rcall .+3226
        s->calls[s->call_depth++] = 5250;
        return 8476;
    }
    case 0x14e4: { // brcs .-16
        return (pm_getflag(s, 0) == 1) ? 5334 : 5350;
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
    case 0x1506: { // brcs .-50
        return (pm_getflag(s, 0) == 1) ? 5334 : 5384;
    }
    case 0x1530: { // cpi r22, 0x02
        pm_sub(s, s->r[22], 2, 0, false);
        return 5426;
    }
    case 0x1532: { // brge .-94
        return (pm_getflag(s, 4) == 0) ? 5334 : 5428;
    }
    case 0x153a: { // brcs .-102
        return (pm_getflag(s, 0) == 1) ? 5334 : 5436;
    }
    case 0x1542: { // rcall .-2222
        s->calls[s->call_depth++] = 5444;
        return 3222;
    }
    case 0x1580: { // brne .+30
        return (pm_getflag(s, 1) == 0) ? 5536 : 5506;
    }
    case 0x1590: { // cpi r30, 0x7E
        pm_sub(s, s->r[30], 126, 0, false);
        return 5522;
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
    case 0x1636: { // brne .-22
        return (pm_getflag(s, 1) == 0) ? 5666 : 5688;
    }
    case 0x1640: { // rcall .+248
        s->calls[s->call_depth++] = 5698;
        return 5946;
    }
    case 0x1642: { // rcall .+110
        s->calls[s->call_depth++] = 5700;
        return 5810;
    }
    case 0x1644: { // brcs .+8
        return (pm_getflag(s, 0) == 1) ? 5710 : 5702;
    }
    case 0x1648: { // brne .-78
        return (pm_getflag(s, 1) == 0) ? 5628 : 5706;
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
    case 0x16e0: { // brne .+18
        return (pm_getflag(s, 1) == 0) ? 5876 : 5858;
    }
    case 0x16fa: { // and r20, r20
        s->r[20] &= s->r[20];
        pm_nzv(s, s->r[20], false);
        return 5884;
    }
    case 0x16fc: { // brne .-62
        return (pm_getflag(s, 1) == 0) ? 5824 : 5886;
    }
    case 0x1710: { // rcall .+12
        s->calls[s->call_depth++] = 5906;
        return 5918;
    }
    case 0x1714: { // rcall .+242
        s->calls[s->call_depth++] = 5910;
        return 6152;
    }
    case 0x1740: { // brne .-4
        return (pm_getflag(s, 1) == 0) ? 5950 : 5954;
    }
    case 0x17d8: { // rcall .+10
        s->calls[s->call_depth++] = 6106;
        return 6116;
    }
    case 0x17ee: { // brcc .+6
        return (pm_getflag(s, 0) == 0) ? 6134 : 6128;
    }
    case 0x17f2: { // brcs .+14
        return (pm_getflag(s, 0) == 1) ? 6146 : 6132;
    }
    case 0x17f8: { // brcc .+8
        return (pm_getflag(s, 0) == 0) ? 6146 : 6138;
    }
    case 0x188e: { // rcall .+4012
        s->calls[s->call_depth++] = 6288;
        return 10300;
    }
    case 0x1892: { // brne .+122
        return (pm_getflag(s, 1) == 0) ? 6414 : 6292;
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
    case 0x18b8: { // cpi r18, 0x7C
        pm_sub(s, s->r[18], 124, 0, false);
        return 6330;
    }
    case 0x18ba: { // brcs .-32
        return (pm_getflag(s, 0) == 1) ? 6300 : 6332;
    }
    case 0x18be: { // rcall .+3964
        s->calls[s->call_depth++] = 6336;
        return 10300;
    }
    case 0x18c2: { // brne .+74
        return (pm_getflag(s, 1) == 0) ? 6414 : 6340;
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
    case 0x1908: { // cpi r19, 0x18
        pm_sub(s, s->r[19], 24, 0, false);
        return 6410;
    }
    case 0x190a: { // brcs .-64
        return (pm_getflag(s, 0) == 1) ? 6348 : 6412;
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
    case 0x1964: { // cpi r16, 0x60
        pm_sub(s, s->r[16], 96, 0, false);
        return 6502;
    }
    case 0x196e: { // cpi r16, 0x10
        pm_sub(s, s->r[16], 16, 0, false);
        return 6512;
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
    case 0x19a4: { // cpi r23, 0x0C
        pm_sub(s, s->r[23], 12, 0, false);
        return 6566;
    }
    case 0x19a8: { // rcall .+3700
        s->calls[s->call_depth++] = 6570;
        return 10270;
    }
    case 0x19ae: { // rcall .+3724
        s->calls[s->call_depth++] = 6576;
        return 10300;
    }
    case 0x19b0: { // cpi r16, 0x0D
        pm_sub(s, s->r[16], 13, 0, false);
        return 6578;
    }
    case 0x19b2: { // brne .-8
        return (pm_getflag(s, 1) == 0) ? 6572 : 6580;
    }
    case 0x19f6: { // cpi r30, 0x35
        pm_sub(s, s->r[30], 53, 0, false);
        return 6648;
    }
    case 0x1a00: { // cpi r18, 0x1F
        pm_sub(s, s->r[18], 31, 0, false);
        return 6658;
    }
    case 0x1a02: { // brne .-28
        return (pm_getflag(s, 1) == 0) ? 6632 : 6660;
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
    case 0x1a58: { // brne .-14
        return (pm_getflag(s, 1) == 0) ? 6732 : 6746;
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
    case 0x1ad0: { // cpi r19, 0x06
        pm_sub(s, s->r[19], 6, 0, false);
        return 6866;
    }
    case 0x1ada: { // brne .+4
        return (pm_getflag(s, 1) == 0) ? 6880 : 6876;
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
    case 0x1b88: { // cpi r17, 0x02
        pm_sub(s, s->r[17], 2, 0, false);
        return 7050;
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
    case 0x1be8: { // brne .+118
        return (pm_getflag(s, 1) == 0) ? 7264 : 7146;
    }
    case 0x1bf4: { // brne .+34
        return (pm_getflag(s, 1) == 0) ? 7192 : 7158;
    }
    case 0x1c1a: { // cpi r16, 0x01
        pm_sub(s, s->r[16], 1, 0, false);
        return 7196;
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
    case 0x1d40: { // brne .-68
        return (pm_getflag(s, 1) == 0) ? 7422 : 7490;
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
    case 0x1d64: { // cpi r16, 0x20
        pm_sub(s, s->r[16], 32, 0, false);
        return 7526;
    }
    case 0x1d68: { // rcall .+2250
        s->calls[s->call_depth++] = 7530;
        return 9780;
    }
    case 0x1d6a: { // brcs .-124
        return (pm_getflag(s, 0) == 1) ? 7408 : 7532;
    }
    case 0x1d6c: { // and r21, r21
        s->r[21] &= s->r[21];
        pm_nzv(s, s->r[21], false);
        return 7534;
    }
    case 0x1d72: { // cpi r20, 0x00
        pm_sub(s, s->r[20], 0, 0, false);
        return 7540;
    }
    case 0x1d76: { // brge .-24
        return (pm_getflag(s, 4) == 0) ? 7520 : 7544;
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
    case 0x1d96: { // brne .-56
        return (pm_getflag(s, 1) == 0) ? 7520 : 7576;
    }
    case 0x1d98: { // rcall .+2202
        s->calls[s->call_depth++] = 7578;
        return 9780;
    }
    case 0x1d9a: { // brcs .-60
        return (pm_getflag(s, 0) == 1) ? 7520 : 7580;
    }
    case 0x1d9e: { // brne .-64
        return (pm_getflag(s, 1) == 0) ? 7520 : 7584;
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
    case 0x1db8: { // brcs .+114
        return (pm_getflag(s, 0) == 1) ? 7724 : 7610;
    }
    case 0x1dbc: { // brne .+110
        return (pm_getflag(s, 1) == 0) ? 7724 : 7614;
    }
    case 0x1dc0: { // brge .+106
        return (pm_getflag(s, 4) == 0) ? 7724 : 7618;
    }
    case 0x1dc6: { // brne .+100
        return (pm_getflag(s, 1) == 0) ? 7724 : 7624;
    }
    case 0x1dc8: { // rcall .+2154
        s->calls[s->call_depth++] = 7626;
        return 9780;
    }
    case 0x1dca: { // brcs .+96
        return (pm_getflag(s, 0) == 1) ? 7724 : 7628;
    }
    case 0x1dcc: { // cpi r16, 0x2C
        pm_sub(s, s->r[16], 44, 0, false);
        return 7630;
    }
    case 0x1dce: { // brne .+92
        return (pm_getflag(s, 1) == 0) ? 7724 : 7632;
    }
    case 0x1dd2: { // rcall .+2144
        s->calls[s->call_depth++] = 7636;
        return 9780;
    }
    case 0x1dd4: { // brcs .+86
        return (pm_getflag(s, 0) == 1) ? 7724 : 7638;
    }
    case 0x1dd8: { // brne .+82
        return (pm_getflag(s, 1) == 0) ? 7724 : 7642;
    }
    case 0x1de0: { // brge .+74
        return (pm_getflag(s, 4) == 0) ? 7724 : 7650;
    }
    case 0x1de2: { // cpi r24, 0x00
        pm_sub(s, s->r[24], 0, 0, false);
        return 7652;
    }
    case 0x1de6: { // brge .+68
        return (pm_getflag(s, 4) == 0) ? 7724 : 7656;
    }
    case 0x1dea: { // cpi r20, 0x55
        pm_sub(s, s->r[20], 85, 0, false);
        return 7660;
    }
    case 0x1df0: { // cpi r24, 0x55
        pm_sub(s, s->r[24], 85, 0, false);
        return 7666;
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
    case 0x1e30: { // brcs .+72
        return (pm_getflag(s, 0) == 1) ? 7802 : 7730;
    }
    case 0x1e34: { // brne .+68
        return (pm_getflag(s, 1) == 0) ? 7802 : 7734;
    }
    case 0x1e38: { // brge .+64
        return (pm_getflag(s, 4) == 0) ? 7802 : 7738;
    }
    case 0x1e3e: { // brne .+58
        return (pm_getflag(s, 1) == 0) ? 7802 : 7744;
    }
    case 0x1e40: { // rcall .+2034
        s->calls[s->call_depth++] = 7746;
        return 9780;
    }
    case 0x1e42: { // brcs .+54
        return (pm_getflag(s, 0) == 1) ? 7802 : 7748;
    }
    case 0x1e46: { // brne .+50
        return (pm_getflag(s, 1) == 0) ? 7802 : 7752;
    }
    case 0x1e52: { // brge .+38
        return (pm_getflag(s, 4) == 0) ? 7802 : 7764;
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
    case 0x1e9c: { // cpi r20, 0xA1
        pm_sub(s, s->r[20], 161, 0, false);
        return 7838;
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
    case 0x1ee2: { // brcs .+84
        return (pm_getflag(s, 0) == 1) ? 7992 : 7908;
    }
    case 0x1ee6: { // brne .+80
        return (pm_getflag(s, 1) == 0) ? 7992 : 7912;
    }
    case 0x1eea: { // brge .+76
        return (pm_getflag(s, 4) == 0) ? 7992 : 7916;
    }
    case 0x1ef0: { // brne .+70
        return (pm_getflag(s, 1) == 0) ? 7992 : 7922;
    }
    case 0x1ef2: { // rcall .+1856
        s->calls[s->call_depth++] = 7924;
        return 9780;
    }
    case 0x1ef4: { // brcs .+66
        return (pm_getflag(s, 0) == 1) ? 7992 : 7926;
    }
    case 0x1ef8: { // brne .+62
        return (pm_getflag(s, 1) == 0) ? 7992 : 7930;
    }
    case 0x1efc: { // cpi r20, 0x40
        pm_sub(s, s->r[20], 64, 0, false);
        return 7934;
    }
    case 0x1f00: { // brge .+54
        return (pm_getflag(s, 4) == 0) ? 7992 : 7938;
    }
    case 0x1f04: { // cpi r20, 0xC0
        pm_sub(s, s->r[20], 192, 0, false);
        return 7942;
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
    case 0x1f3c: { // brcs .-6
        return (pm_getflag(s, 0) == 1) ? 7992 : 7998;
    }
    case 0x1f40: { // brne .-10
        return (pm_getflag(s, 1) == 0) ? 7992 : 8002;
    }
    case 0x1f42: { // cpi r20, 0x0C
        pm_sub(s, s->r[20], 12, 0, false);
        return 8004;
    }
    case 0x1f44: { // brge .-14
        return (pm_getflag(s, 4) == 0) ? 7992 : 8006;
    }
    case 0x1f4c: { // rcall .+1766
        s->calls[s->call_depth++] = 8014;
        return 9780;
    }
    case 0x1f4e: { // brcs .-24
        return (pm_getflag(s, 0) == 1) ? 7992 : 8016;
    }
    case 0x1f5a: { // cpi r20, 0x21
        pm_sub(s, s->r[20], 33, 0, false);
        return 8028;
    }
    case 0x1f5e: { // brge .-40
        return (pm_getflag(s, 4) == 0) ? 7992 : 8032;
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
    case 0x1f90: { // brcs .-90
        return (pm_getflag(s, 0) == 1) ? 7992 : 8082;
    }
    case 0x1f94: { // brne .-94
        return (pm_getflag(s, 1) == 0) ? 7992 : 8086;
    }
    case 0x1f98: { // brge .-98
        return (pm_getflag(s, 4) == 0) ? 7992 : 8090;
    }
    case 0x1f9e: { // brne .-104
        return (pm_getflag(s, 1) == 0) ? 7992 : 8096;
    }
    case 0x1fa0: { // rcall .+1682
        s->calls[s->call_depth++] = 8098;
        return 9780;
    }
    case 0x1fa2: { // brcs .-108
        return (pm_getflag(s, 0) == 1) ? 7992 : 8100;
    }
    case 0x1fa6: { // brne .-112
        return (pm_getflag(s, 1) == 0) ? 7992 : 8104;
    }
    case 0x1faa: { // cpi r20, 0x31
        pm_sub(s, s->r[20], 49, 0, false);
        return 8108;
    }
    case 0x1fae: { // brge .-120
        return (pm_getflag(s, 4) == 0) ? 7992 : 8112;
    }
    case 0x1fb2: { // cpi r20, 0x2C
        pm_sub(s, s->r[20], 44, 0, false);
        return 8116;
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
    case 0x2050: { // brcs .-104
        return (pm_getflag(s, 0) == 1) ? 8170 : 8274;
    }
    case 0x2054: { // brne .-108
        return (pm_getflag(s, 1) == 0) ? 8170 : 8278;
    }
    case 0x2058: { // brge .-112
        return (pm_getflag(s, 4) == 0) ? 8170 : 8282;
    }
    case 0x205e: { // brne .-118
        return (pm_getflag(s, 1) == 0) ? 8170 : 8288;
    }
    case 0x2060: { // rcall .+1490
        s->calls[s->call_depth++] = 8290;
        return 9780;
    }
    case 0x2062: { // brcs .-122
        return (pm_getflag(s, 0) == 1) ? 8170 : 8292;
    }
    case 0x2066: { // brne .-126
        return (pm_getflag(s, 1) == 0) ? 8170 : 8296;
    }
    case 0x206a: { // cpi r20, 0xF5
        pm_sub(s, s->r[20], 245, 0, false);
        return 8300;
    }
    case 0x206e: { // brge .-36
        return (pm_getflag(s, 4) == 0) ? 8268 : 8304;
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
    case 0x2164: { // and r1, r1
        s->r[1] &= s->r[1];
        pm_nzv(s, s->r[1], false);
        return 8550;
    }
    case 0x2186: { // cpi r19, 0x03
        pm_sub(s, s->r[19], 3, 0, false);
        return 8584;
    }
    case 0x2188: { // brcs .+4
        return (pm_getflag(s, 0) == 1) ? 8590 : 8586;
    }
    case 0x21d8: { // brne .-30
        return (pm_getflag(s, 1) == 0) ? 8636 : 8666;
    }
    case 0x225c: { // brne .-26
        return (pm_getflag(s, 1) == 0) ? 8772 : 8798;
    }
    case 0x2282: { // brne .-24
        return (pm_getflag(s, 1) == 0) ? 8812 : 8836;
    }
    case 0x24da: { // rcall .-86
        s->calls[s->call_depth++] = 9436;
        return 9350;
    }
    case 0x2582: { // cpi r30, 0x96
        pm_sub(s, s->r[30], 150, 0, false);
        return 9604;
    }
    case 0x2584: { // brne .-32
        return (pm_getflag(s, 1) == 0) ? 9574 : 9606;
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
    case 0x2630: { // brne .-40
        return (pm_getflag(s, 1) == 0) ? 9738 : 9778;
    }
    case 0x264a: { // rcall .+496
        s->calls[s->call_depth++] = 9804;
        return 10300;
    }
    case 0x2650: { // cpi r16, 0x2D
        pm_sub(s, s->r[16], 45, 0, false);
        return 9810;
    }
    case 0x2662: { // brge .+30
        return (pm_getflag(s, 4) == 0) ? 9858 : 9828;
    }
    case 0x267a: { // brne .+32
        return (pm_getflag(s, 1) == 0) ? 9884 : 9852;
    }
    case 0x267e: { // brcc .-54
        return (pm_getflag(s, 0) == 0) ? 9802 : 9856;
    }
    case 0x2682: { // cpi r18, 0x01
        pm_sub(s, s->r[18], 1, 0, false);
        return 9860;
    }
    case 0x268a: { // cpi r18, 0x02
        pm_sub(s, s->r[18], 2, 0, false);
        return 9868;
    }
    case 0x26b6: { // rcall .+388
        s->calls[s->call_depth++] = 9912;
        return 10300;
    }
    case 0x26ba: { // cpi r16, 0x30
        pm_sub(s, s->r[16], 48, 0, false);
        return 9916;
    }
    case 0x26bc: { // brcs .+42
        return (pm_getflag(s, 0) == 1) ? 9960 : 9918;
    }
    case 0x26c0: { // brcs .+10
        return (pm_getflag(s, 0) == 1) ? 9932 : 9922;
    }
    case 0x26c2: { // cpi r16, 0x41
        pm_sub(s, s->r[16], 65, 0, false);
        return 9924;
    }
    case 0x26c4: { // brcs .+34
        return (pm_getflag(s, 0) == 1) ? 9960 : 9926;
    }
    case 0x26c6: { // cpi r16, 0x47
        pm_sub(s, s->r[16], 71, 0, false);
        return 9928;
    }
    case 0x26c8: { // brcc .+30
        return (pm_getflag(s, 0) == 0) ? 9960 : 9930;
    }
    case 0x26e2: { // cpi r18, 0x04
        pm_sub(s, s->r[18], 4, 0, false);
        return 9956;
    }
    case 0x26e4: { // brne .-48
        return (pm_getflag(s, 1) == 0) ? 9910 : 9958;
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
    case 0x2724: { // cpi r16, 0x3A
        pm_sub(s, s->r[16], 58, 0, false);
        return 10022;
    }
    case 0x2726: { // brcs .+2
        return (pm_getflag(s, 0) == 1) ? 10026 : 10024;
    }
    case 0x272a: { // rcall .+384
        s->calls[s->call_depth++] = 10028;
        return 10412;
    }
    case 0x2768: { // brcc .+16
        return (pm_getflag(s, 0) == 0) ? 10106 : 10090;
    }
    case 0x276c: { // and r17, r17
        s->r[17] &= s->r[17];
        pm_nzv(s, s->r[17], false);
        return 10094;
    }
    case 0x27ba: { // brne .-58
        return (pm_getflag(s, 1) == 0) ? 10114 : 10172;
    }
    case 0x27e6: { // and r15, r15
        s->r[15] &= s->r[15];
        pm_nzv(s, s->r[15], false);
        return 10216;
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
    case 0x2892: { // cpi r18, 0x60
        pm_sub(s, s->r[18], 96, 0, false);
        return 10388;
    }
    case 0x28ca: { // brne .+6
        return (pm_getflag(s, 1) == 0) ? 10450 : 10444;
    }
    case 0x28ec: { // and r19, r19
        s->r[19] &= s->r[19];
        pm_nzv(s, s->r[19], false);
        return 10478;
    }
    default: return UINT32_MAX;
    }
}
END COMPILED LOGICAL C EQUIVALENT */
