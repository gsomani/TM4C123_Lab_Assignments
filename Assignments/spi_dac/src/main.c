#include <stdint.h>
#include "inc/tm4c123gh6pm.h"
#include <math.h>
#define M_PI		3.14159265358979323846
#define M_PI_2		1.57079632679489661923
#define M_PI_512	0.00613592315152
#define M_TWOPI         (M_PI * 2.0)


void Init_SSI1(void);
void SSI1_Write(uint16_t data,char channel);
uint16_t sine[1024];int cur=0;

void generate_sine_table()
{	float t;int i;
	for(t=0,i=0;t<M_TWOPI && i<1024;t+=M_PI_512,i++)
		 sine[i]=  round ( 511.5  * ( 1.0+sinf(t)  ));
} 


int main(void)
{
 int i;

    Init_SSI1();  	    
generate_sine_table();
cur=0;
while(1){	
    for(i=0;i<1024;i++) {
 		SSI1_Write(i,0);
		SSI1_Write(sine[i],1);          
		}
	}
}
 
void SSI1_Write(uint16_t data,char channel)
{
    GPIO_PORTF_DATA_R &= ~0x04;  /* assert SS low */
 
    while((SSI1_SR_R & 2) == 0)
        ;                        /* wait until FIFO not full */
    SSI1_DR_R = ((0x9+channel)<<12)|(data<<2);            /* transmit high byte */
    
    while(SSI1_SR_R & 0x10)
        ;                        /* wait until transmit complete */
    GPIO_PORTF_DATA_R |= 0x04;   /* keep SS idle high */
}
 
void Init_SSI1(void)
{
    SYSCTL_RCGCSSI_R |= 2;       /* enable clock to SSI1 */
    SYSCTL_RCGCGPIO_R |= 8;      /* enable clock to GPIOD for SSI1 */
    SYSCTL_RCGCGPIO_R |= 0x20;   /* enable clock to GPIOF for slave select */
 
    /* configure PORTD 3, 1 for SSI1 clock and Tx */
    GPIO_PORTD_AMSEL_R &= ~0x09; /* disable analog for these pins */
    GPIO_PORTD_DEN_R |= 0x09;    /* and make them digital */
    GPIO_PORTD_AFSEL_R |= 0x09;  /* enable alternate function */
    GPIO_PORTD_PCTL_R &= ~0x0000F00F; /* assign pins to SSI1 */
    GPIO_PORTD_PCTL_R |= 0x00002002;  /* assign pins to SSI1 */
 
    /* configure PORTF 2 for slave select */
    GPIO_PORTF_DEN_R |= 0x04;    /* make the pin digital */
    GPIO_PORTF_DIR_R |= 0x04;    /* make the pin output */
    GPIO_PORTF_DATA_R |= 0x04;   /* keep SS idle high */
 
    /* SPI Master, POL = 0, PHA = 0, clock = 4 MHz, 16 bit data */
    SSI1_CR1_R = 0;             /* disable SSI and make it master */
    SSI1_CC_R = 0;              /* use system clock */
    SSI1_CPSR_R = 2;            /* prescaler divided by 2 */
    SSI1_CR0_R = 0xF;        /* 8 MHz SSI clock, SPI mode, 8 bit data */
    SSI1_CR1_R |= 2;            /* enable SSI1 */
}



