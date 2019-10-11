#include <stdint.h>
#include <stdio.h>
#include "inc/tm4c123gh6pm.h"
#include "pll.h" 

uint32_t pw;
int done;
void Timer0Capture_Init(void)
{
	SYSCTL_RCGCTIMER_R |= 1;        /* enable clock to Timer Block 0 */
        SYSCTL_RCGC2_R |= 2;            /* enable clock to PORTB */
        GPIO_PORTB_DIR_R &= ~0xC0;      /* make PB6 an input pin */
        GPIO_PORTB_DEN_R |= 0xC0;       /* make PB6 as digital pin */
        GPIO_PORTB_AFSEL_R |= 0xC0;     /* use PB6 alternate function */
        GPIO_PORTB_PCTL_R &= ~0xFF000000; /* configure PB6 for T0CCP0 */
        GPIO_PORTB_PCTL_R |= 0x77000000;
     

        TIMER0_CTL_R &= ~101;             /* disable timer0A during setup */
        TIMER0_CFG_R = 4;               /* 16-bit timer mode */
        TIMER0_TAMR_R = 0x17;           /* up-count, edge-time, capture mode */
	TIMER0_TBMR_R = 0x17;        
	TIMER0_CTL_R &= ~0x0C;          /* capture the rising edge */
	TIMER0_CTL_R &= ~0x800;
        TIMER0_CTL_R |= 0x400;
	TIMER0_ICR_R = 0x404;        
	TIMER0_CTL_R |= 0x101;              /* enable timer0A */
}



    int Timer0_periodCapture(void)

    {
        int lastEdge, thisEdge;
    /* capture the first rising edge */
        TIMER0_ICR_R = 0x4;               /* clear 1 Vtimer0A capture flag */
        while((TIMER0_RIS_R & 0x4) == 0)
            ;                           /* wait till captured */

        lastEdge = TIMER0_TAR_R;        /* save the timestamp */
        /* capture the second rising edge */
        TIMER0_ICR_R = 0x400;               /* clear timer0A capture flag */
        while((TIMER0_RIS_R & 0x400) == 0);                           /* wait till captured */
        thisEdge = TIMER0_TBR_R;        /* save the timestamp */
        return (thisEdge - lastEdge) & 0x00FFFFFF; /* return the time difference */

    }

  
    int main (void)

    { long int pulse_width,pulse_ns;

PLL_Init();
	 	
      Timer0Capture_Init();
      while(1) {
          pulse_width=Timer0_periodCapture();
	    pulse_ns=(pulse_width*25)/2;		
            printf("%ld ns\n", pulse_ns);
        }

    }