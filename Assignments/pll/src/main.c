/* Configure the system to get its clock from the PLL */
#include <stdint.h>
#include "inc/tm4c123gh6pm.h"
#define SYSCTL_RIS_PLLLRIS 0x00000040
#define SYSCTL_RCC_XTAL_M 0x000007C0
#define SYSCTL_RCC_XTAL_6MHZ 0x000002C0
#define SYSCTL_RCC_XTAL_8MHZ 0x00000380
#define SYSCTL_RCC_XTAL_16MHZ 0x00000540
#define SYSCTL_RCC2_USERCC2 0x80000000
#define SYSCTL_RCC2_DIV400 0x40000000
#define SYSCTL_RCC2_SYSDIV2_M 0x1F800000
#define SYSCTL_RCC2_SYSDIV2LSB 0x00400000
#define SYSCTL_RCC2_PWRDN2 0x00002000
#define SYSCTL_RCC2_BYPASS2 0x00000800
#define SYSCTL_RCC2_OSCSRC2_M 0x00000070
#define SYSCTL_RCC2_OSCSRC2_MO 0x00000000
#define Bus50MHz 4

int flag =0;
void enable_irq(void);
void PLL_Init(void)
{
/* 1) configure the system to use RCC2 for advanced features
such as 400 MHz PLL and non-integer System Clock Divisor */
SYSCTL_RCC2_R |= SYSCTL_RCC2_USERCC2;
/* 2) bypass PLL while initializing */
SYSCTL_RCC2_R |= SYSCTL_RCC2_BYPASS2;/* 3) select the crystal value and oscillator source */
SYSCTL_RCC_R &= ~SYSCTL_RCC_XTAL_M;
/* clear XTAL field */
SYSCTL_RCC_R += SYSCTL_RCC_XTAL_16MHZ;
/* configure for 16 MHz crystal */
SYSCTL_RCC2_R &= ~SYSCTL_RCC2_OSCSRC2_M;
/* clear oscillator source field */
SYSCTL_RCC2_R += SYSCTL_RCC2_OSCSRC2_MO;
/* configure for main oscillator source */
/* 4) activate PLL by clearing PWRDN */
SYSCTL_RCC2_R &= ~SYSCTL_RCC2_PWRDN2;
/* 5) set the desired system divider and the system divider least significant bit */
SYSCTL_RCC2_R |= SYSCTL_RCC2_DIV400;
/* use 400 MHz PLL */
SYSCTL_RCC2_R = (SYSCTL_RCC2_R&~0x1FC00000)
/* clear system clock divider field */
+ (Bus50MHz<<22);
/* configure for 80 MHz clock */
/* 6) wait for the PLL to lock by polling PLLLRIS */
while((SYSCTL_RIS_R&SYSCTL_RIS_PLLLRIS)==0){
;
}
/* 7) enable use of PLL by clearing BYPASS */
SYSCTL_RCC2_R &= ~SYSCTL_RCC2_BYPASS2;
}

void SysTick_Handler(void)
{
flag++;
flag=flag%6;
if(flag<3)
GPIO_PORTF_DATA_R=0;
else
GPIO_PORTF_DATA_R=2;
} 

/* global enable interrupts */
void inline enable_irq(void)
{
    __asm  ("    CPSIE  I\n");
}

int main()
{

	PLL_Init();
	SYSCTL_RCGC2_R |= 0x20;
    /* enable the GPIO pins for the LED (PF3, 2, 1) as output */
    GPIO_PORTF_DIR_R = 0x0E;
    /* enable the GPIO pins for digital function */
    GPIO_PORTF_DEN_R = 0x0E;
 
    /* Configure SysTick */
    NVIC_ST_RELOAD_R = 16666666;  /* reload with number of clocks per second */
    NVIC_ST_CTRL_R = 7;             /* enable SysTick interrupt, use system clock */
 
    enable_irq();                   /* global enable interrupt */
 
    while(1) {
        
    }

}
