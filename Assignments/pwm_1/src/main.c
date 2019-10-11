#include<stdint.h>
#include"inc/tm4c123gh6pm.h"

void delayMs(int n)
{
int i, j;
for(i = 0 ; i < n; i++)
for(j = 0; j < 3180; j++) {}
}

int main()
{
int freq=1,duty_cycle=75;

SYSCTL_RCGCPWM_R |= 1;

SYSCTL_RCGCGPIO_R |= 0x2;

SYSCTL_RCC_R &= ~0x100000;

GPIO_PORTB_AFSEL_R |= 0x40;  
GPIO_PORTB_PCTL_R &= ~0x0F000000; 
GPIO_PORTB_PCTL_R |= 0x04000000;

GPIO_PORTB_DEN_R |= 0x40;

PWM0_0_CTL_R = 0;

PWM0_0_GENA_R = 0x8C;

PWM0_0_LOAD_R = 16000/freq;

PWM0_0_CMPA_R = PWM0_0_LOAD_R*(100-duty_cycle)/100 ;

PWM0_0_CTL_R=1;

PWM0_ENABLE_R=1;

}