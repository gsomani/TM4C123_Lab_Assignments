#include <stdint.h>
#include <stdio.h>
#include "inc/tm4c123gh6pm.h"
#include "pll.h" 

long int cur;
uint32_t pw,last_falling=0; 

void EnableInterrupts(void)
{
    __asm  ("CPSIE  I\n");
}

void WaitForInterrupt(void)
{
    __asm  ("WFI\n");
}

void Timer0A_Handler()
{	
	TIMER0_ICR_R = 0x4;	
	cur=TIMER0_TBR_R-TIMER0_TAR_R;
	if(TIMER0_TBR_R<last_falling && cur>0) pw=cur;
	last_falling=TIMER0_TAR_R;	
}

void Timer0Capture_Init(void)
{
        SYSCTL_RCGCTIMER_R |= 1;        /* enable clock to Timer Block 0 */
        SYSCTL_RCGCGPIO_R |= 2;           /* enable clock to PORTB */
        GPIO_PORTB_DIR_R &= ~0xC0;      /* make PB6 an input pin */
        GPIO_PORTB_DEN_R |= 0xC0;       /* make PB6 as digital pin */
        GPIO_PORTB_AFSEL_R |= 0xC0;     /* use PB6 alternate function */
        GPIO_PORTB_PCTL_R &= ~0xFF000000; /* configure PB6 for T0CCP0 */
        GPIO_PORTB_PCTL_R |= 0x77000000;
     

	TIMER0_CTL_R &= ~0x101;
	TIMER0_CFG_R = 4;


	TIMER0_TAMR_R = 0x7;
	TIMER0_CTL_R &= ~0x0C;
	TIMER0_CTL_R |= 0x04;
	TIMER0_TAILR_R=0xFFFF;
	TIMER0_TAPR_R=0xFF;
	TIMER0_IMR_R |= TIMER_IMR_CAEIM;
	TIMER0_ICR_R |= TIMER_ICR_CAECINT;
	
	TIMER0_TBMR_R = 0x7;
	TIMER0_CTL_R &= ~0x0C00;
	TIMER0_TBILR_R=0xFFFF;
	TIMER0_TBPR_R=0xFF;
	TIMER0_IMR_R &= ~0x700;
	TIMER0_ICR_R |= 0x400;	
	
	NVIC_PRI4_R = (NVIC_PRI4_R & 0xFFFFFF) | 0x40000000 ;
	NVIC_EN0_R = 1<<19;
	
	TIMER0_CTL_R |= 0x101;

	EnableInterrupts();
}
  
    int main (void)

    {  
	long int pulse_ns;
	PLL_Init();
	 	
      Timer0Capture_Init();
      while(1) {
	pulse_ns=(pw*25)/2;
	printf("\n %ld ns\n",pulse_ns);	
        }

    }