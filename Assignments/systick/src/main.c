/* systick_int.c
 *
 * Toggle the red LED using the SysTick interrupt
 *
 * This program sets up the SysTick to interrupt at 1 Hz.
 * The system clock is running at 16 MHz.
 * 1sec/62.5ns = 16,000,000 for RELOAD register.
 * In the interrupt handler, the red LED is toggled.
*/
 
#include <stdint.h>
#include "inc/tm4c123gh6pm.h"
 
void enable_irq(void);
 
int main (void)
{
    /* enable clock to GPIOF at clock gating control register */
    SYSCTL_RCGC2_R |= 0x20;
    /* enable the GPIO pins for the LED (PF3, 2, 1) as output */
    GPIO_PORTF_DIR_R = 0x0E;
    /* enable the GPIO pins for digital function */
    GPIO_PORTF_DEN_R = 0x0E;
 
    /* Configure SysTick */
    NVIC_ST_RELOAD_R = 16000000-1;  /* reload with number of clocks per second */
    NVIC_ST_CTRL_R = 7;             /* enable SysTick interrupt, use system clock */
 
    enable_irq();                   /* global enable interrupt */
 
    while(1) {
        ;
    }
}
 
void SysTick_Handler(void)
{
    GPIO_PORTF_DATA_R ^= 2;         /* toggle the red LED */
}
 
/* global enable interrupts */
void inline enable_irq(void)
{
    __asm  ("    CPSIE  I\n");
}
