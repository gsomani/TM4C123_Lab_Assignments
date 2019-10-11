#include<stdint.h>
#include"inc/tm4c123gh6pm.h"

int flag =0;

void EnableInterrupts(void)
{
    __asm  ("CPSIE  I\n");
}

void WaitForInterrupt(void)
{
    __asm  ("WFI\n");
}

void init_timer()
{
SYSCTL_RCGCTIMER_R |= 1;
TIMER0_ICR_R = 0x1;
TIMER0_CTL_R=0;
TIMER0_CFG_R = 0x04;
TIMER0_TAMR_R = 0x02;
TIMER0_TAILR_R = 65535;
TIMER0_TAPR_R = 15;
TIMER0_ICR_R = 0x1;
TIMER0_IMR_R = 0x1;
NVIC_EN0_R=0x00080000;
NVIC_PRI4_R = (NVIC_PRI4_R | 0x60000000) & 0x70000000 ;
TIMER0_CTL_R |= 0x01; 

}

void init_leds()
{
SYSCTL_RCGC2_R |= 0x00000020;   /* enable clock to GPIOF at clock gating control register */
 
    GPIO_PORTF_DIR_R = 0x0E;        /* enable the GPIO pins for the LED (PF3, 2, and 1) as output */
    GPIO_PORTF_DEN_R = 0x0E; 
}

void init()
{
EnableInterrupts();
init_timer();
init_leds();
}

void timer0_interrupt_handler()
{
TIMER0_ICR_R = 0x1;
switch(flag)
{
case 0: GPIO_PORTF_DATA_R = 2;flag=1;break;
case 1: GPIO_PORTF_DATA_R = 4;flag=2;break;
case 2: GPIO_PORTF_DATA_R = 8;flag=0;break;
}
}


int main(void)
{
	init();	
	while(1){
//		WaitForInterrupt();
	}
return 0;
}

