#include <stdint.h>
#include <stdio.h>
#include "inc/tm4c123gh6pm.h"

int main(void)

{
   volatile int result;

    /* enable clocks */

    SYSCTL_RCGCGPIO_R |= 0x10; /* enable clock to PE (AIN0 is on PE3) */
    SYSCTL_RCGCADC_R |= 1;     /* enable clock to ADC0 */
    /* initialize PE3 for AIN0 input  */

    GPIO_PORTE_AFSEL_R |= 8;   /* enable alternate function */
    GPIO_PORTE_DEN_R &= ~8;    /* disable digital function */
    GPIO_PORTE_AMSEL_R |= 8;   /* enable analog function */
    /* initialize ADC0 */
    ADC0_ACTSS_R &= ~8;        /* disable SS3 during configuration */
    ADC0_EMUX_R &= ~0xF000;    /* software trigger conversion */
    ADC0_SSMUX3_R = 0;         /* get input from channel 0 */
    ADC0_SSCTL3_R |= 6;        /* take one sample at a time, set flag at 1st sample */
    ADC0_ACTSS_R |= 8;         /* enable ADC0 sequencer 3 */

    while(1) {
        ADC0_PSSI_R |= 8;      /* start a conversion sequence 3 */
        while((ADC0_RIS_R & 8) == 0)
            ;                  /* wait for conversion complete */

        result = ADC0_SSFIFO3_R; /* read conversion result */
	printf("%i\n",result);
        ADC0_ISC_R = 8;        /* clear completion flag */
    }
}