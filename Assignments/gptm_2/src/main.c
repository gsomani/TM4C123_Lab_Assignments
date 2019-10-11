#include<stdint.h>
#include"inc/tm4c123gh6pm.h"

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
SYSCTL_RCGCTIMER_R |= 0x6 ;

TIMER1_ICR_R = 0x1;
TIMER1_CTL_R=0;
TIMER1_CFG_R = 0x04;
TIMER1_TAMR_R = 0x02;
TIMER1_TAILR_R = 64000;
TIMER1_TAPR_R = 250;
TIMER1_ICR_R = 0x1;
TIMER1_IMR_R = 0x1;

TIMER2_ICR_R = 0x1;
TIMER2_CTL_R=0;
TIMER2_CFG_R = 0x04;
TIMER2_TAMR_R = 0x02;
TIMER2_TAILR_R = 64000;
TIMER2_TAPR_R = 25;
TIMER2_ICR_R = 0x1;
TIMER2_IMR_R = 0x1;

NVIC_EN0_R |= 0x00A00000;
NVIC_PRI5_R = (NVIC_PRI5_R | 0x60007000) & 0x70006000 ;

TIMER1_CTL_R |= 0x01; 
TIMER2_CTL_R |= 0x01; 
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

void timer1_interrupt_handler()
{
TIMER1_ICR_R = 0x1;
GPIO_PORTF_DATA_R ^= 2;
}

void timer2_interrupt_handler()
{
TIMER2_ICR_R = 0x1;
GPIO_PORTF_DATA_R ^= 8;
}


int main(void)
{
	init();	
	while(1){
//		WaitForInterrupt();
	}
return 0;
}

