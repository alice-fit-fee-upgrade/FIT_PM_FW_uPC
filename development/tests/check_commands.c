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
int main(void)
{
    check("PING\r", "PONG\r\n");
    check("HELP\r", "@PING\r\n@HELP\r\n");
    check("PING   \r", "PONG\r\n");
    check("PING 1\r", "ERR ARGUMENTS\r\n");
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
