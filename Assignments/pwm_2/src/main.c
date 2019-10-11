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
int x=15999;

SYSCTL_RCGCPWM_R |= 2;

SYSCTL_RCGCGPIO_R |= 0x20;

SYSCTL_RCC_R &= ~0x100000;

GPIO_PORTF_AFSEL_R = 8;

GPIO_PORTF_PCTL_R &= ~0xF000;

GPIO_PORTF_PCTL_R |= 0x5000;

GPIO_PORTF_DEN_R |= 8;

PWM1_3_CTL_R = 0;

PWM1_3_GENB_R = 0x8C;

PWM1_3_LOAD_R = 16000;

PWM1_3_CMPA_R = 12000;

PWM1_3_CTL_R=1;

PWM1_ENABLE_R=0x80;

}