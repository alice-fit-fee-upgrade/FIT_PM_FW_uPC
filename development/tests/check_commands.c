#include "extension.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
static const char *input;
static char output[160];
static unsigned used;
uint8_t pm_console_getc(void) { assert(*input); return (uint8_t)*input++; }
void pm_console_putc(uint8_t value) { assert(used+1 < sizeof output); output[used++] = (char)value; output[used]=0; }
static void check(const char *request, const char *reply)
{
    input=request; used=0; output[0]=0;
    pm_extension_dispatch(); assert(!*input); assert(strcmp(output,reply)==0);
}
uint8_t pm_read8(uint16_t a) { (void)a;return 0; }
void pm_write8(uint16_t a,uint8_t v) { (void)v;assert(a==15); }
uint8_t pm_irq_enter(void) { return 0; }
void pm_irq_leave(uint8_t s) { assert(s==0); }
void pm_i2c_delay(void) {}
int main(void)
{
    check("PING\r", "PONG\r\n");
    check("HELP\r", "@PING\r\n@HELP\r\n@HW\r\n@TEMP\r\n@TDC\r\n@PLL\r\n@PGTIME\r\n@FPGA\r\n@FPGATEST\r\n@CFG\r\n@FLASHID\r\n@FLASHREAD\r\n@I2C\r\n@INA\r\n@POWER\r\n");
    check("PING   \r", "PONG\r\n");
    check("PING 1\r", "ERR ARGUMENTS\r\n");
    check("PGTIME\r", "PGTIME STATE=00 TICKS=0000 OVF=00 US_NOMINAL=00000000\r\n");
    check("PGTIME 1\r", "ERR ARGUMENTS\r\n");
    check("TEMP\r", "ERR POWER\r\n");
    check("TEMP RESTORE\r", "ERR POWER\r\n");
    check("PLL\r", "ERR POWER\r\n");
    check("TDC 0 83\r", "ERR POWER\r\n");
    check("TDC 3 83\r", "ERR ARGUMENTS\r\n");
    check("TDC 0 8G\r", "ERR ARGUMENTS\r\n");
    check("TDC 0 8\r", "ERR ARGUMENTS\r\n");
    check("FPGA\r", "ERR POWER\r\n");
    check("FPGATEST\r", "ERR POWER\r\n");
    check("FLASHID\r", "ERR POWER\r\n");
    check("FLASHREAD 000000 10\r", "ERR POWER\r\n");
    check("FLASHREAD 000000 00\r", "ERR ARGUMENTS\r\n");
    check("FLASHREAD FFFFFF 02\r", "ERR ARGUMENTS\r\n");
    check("FLASHREAD 0\r", "ERR ARGUMENTS\r\n");
    check("CFG HOLD\r", "ERR POWER\r\n");
    check("CFG BOOT\r", "ERR POWER\r\n");
    check("CFG H\r", "ERR ARGUMENTS\r\n");
    check("HW 1\r", "ERR ARGUMENTS\r\n");
    check("NOPE\r", "ERR UNKNOWN\r\n");
    check("\r", "ERR UNKNOWN\r\n");
    char maximum[49]; memset(maximum,'X',47);maximum[47]='\r';maximum[48]=0;
    check(maximum,"ERR UNKNOWN\r\n");
    char overflow[56];memset(overflow,'X',48);strcpy(overflow+48,"\rPING\r");
    input=overflow;used=0;output[0]=0;pm_extension_dispatch();
    assert(strcmp(output,"ERR TOO LONG\r\n")==0 && strcmp(input,"PING\r")==0);
    used=0;output[0]=0;pm_extension_dispatch();assert(strcmp(output,"PONG\r\n")==0 && !*input);
    puts("PASS native C commands: valid/invalid arguments, line bounds and overflow recovery");
}
