#include "extension.h"
#include "hw_diag.h"
#include "bus_lock.h"
#include "i2c_diag.h"
#define TEXT(s) do { static const PM_FLASH char t[]=s;pm_console_write(t); } while(0)
static void hx(uint16_t value,uint8_t digits)
{while(digits){uint8_t n=(value>>(--digits*4))&15;pm_console_putc(n<10?'0'+n:'A'+n-10);}}
static void number(int32_t value)
{
 char digits[11];uint8_t n=0;uint32_t u;
 if(value<0){pm_console_putc('-');u=(uint32_t)(-(value+1))+1;}else u=(uint32_t)value;
 do{digits[n++]='0'+u%10;u/=10;}while(u);
 while(n)pm_console_putc(digits[--n]);
}
static void error(uint8_t status)
{
 TEXT("ERR I2C ");
 switch(status){
 case PM_I2C_BUSY:TEXT("BUSY");break;
 case PM_I2C_NACK:TEXT("NACK");break;
 case PM_I2C_TIMEOUT:TEXT("TIMEOUT");break;
 case PM_I2C_RESET:TEXT("RESET_OR_ALARM");break;
 case PM_I2C_STUCK:TEXT("BUS_STUCK");break;
 case PM_I2C_RESTORE:TEXT("MUX_RESTORE_FAILED");break;
 case PM_I2C_ID:TEXT("ID");break;
 case PM_I2C_POWER:TEXT("POWER_UNAVAILABLE");break;
 default:TEXT("UNKNOWN");break;
 }
 TEXT("\r\n");
}
void pm_command_i2c(const char *args)
{
 if(*args){TEXT("ERR ARGUMENTS\r\n");return;}
 uint8_t irq=pm_irq_enter(),owner=pm_read8(PM_BUS_OWNER_ADDR),deferred=pm_read8(PM_BUS_DEFER_ADDR);
 uint8_t lines=(!!(pm_read8(0x6a8)&128))|((!!(pm_read8(0x608)&2))<<1)|((!!(pm_read8(0x628)&64))<<2);
 pm_irq_leave(irq);
 TEXT("I2C PF7/PA1 RESET_PB6 LINES=");hx(lines,2);TEXT(" OWNER=");hx(owner,2);TEXT(" DEFER=");hx(deferred,2);TEXT("\r\n");
}
void pm_command_ina(const char *args)
{
 if(args[0]<'4'||args[0]>'6'||args[1]){TEXT("ERR ARGUMENTS\r\n");return;}
 uint8_t route=args[0]-'0';pm_ina_sample s;
 uint8_t result=pm_ina_read(route,&s);if(result){error(result);return;}
 TEXT("INA ");number(route);TEXT(" ADDR=40 CONFIG=");hx(s.configuration,4);
 TEXT(" MASKS_RESTORED=");hx(s.mask70,2);pm_console_putc('/');hx(s.mask71,2);TEXT("\r\n");
 for(uint8_t ch=0;ch<3;++ch){
  int16_t shunt=pm_ina_code(s.raw[ch*2]),bus=pm_ina_code(s.raw[ch*2+1]);
  TEXT("INA ");number(route);TEXT(" CH=");number(ch+1);TEXT(" RAW=");hx(s.raw[ch*2],4);pm_console_putc('/');hx(s.raw[ch*2+1],4);
  TEXT(" MV=");number((int32_t)bus*8);TEXT(" MA=");
  if((route==5&&ch==0)||(route==6&&ch==2)){
   TEXT("NA");TEXT(" STATUS=");if(route==5)TEXT("R15_NOT_FITTED_BUS_INPUT_ONLY");else TEXT("NO_SHUNT");
  }else if(shunt==-4096||shunt==4095){TEXT("NA STATUS=CLIPPED");}
  else{number((int32_t)shunt*(route==4?4:40));TEXT(" STATUS=NOMINAL_UNCALIBRATED");}
  TEXT("\r\n");
 }
}
void pm_command_power(const char *args)
{
 if(*args){TEXT("ERR ARGUMENTS\r\n");return;}
 for(char route='4';route<='6';++route){char arg[2]={route,0};pm_command_ina(arg);}
}
