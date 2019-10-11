#include<stdint.h>
#include"inc/tm4c123gh6pm.h"
#include "i2c_lcd.h"

void DisableInterrupts(void)
{
    __asm ("CPSID  I\n");
}

void EnableInterrupts(void)
{
    __asm  ("CPSIE  I\n");
}

int main(void)
{ 
DisableInterrupts();
initialise_lcd_time();
EnableInterrupts();
while(1) {
}
return 0;
} 
