#include <stdint.h>
#include <stdio.h>
#include "inc/tm4c123gh6pm.h"

int result;

void EnableInterrupts(void)
{
    __asm  ("CPSIE  I\n");
}

void WaitForInterrupt(void)
{
    __asm  ("WFI\n");
}

void DisableInterrupts(void)
{
    __asm ("CPSID  I\n");
}


volatile uint32_t ADC0,ADC1;

void ADC0Seq3_Handler(void){
  ADC0_ISC_R = 0x08;          // acknowledge ADC sequence 3 completion
  ADC0 = ADC0_SSFIFO3_R;  // 12-bit result
}

void ADC1Seq3_Handler(void){
  ADC1_ISC_R = 0x08;          // acknowledge ADC sequence 3 completion
  ADC1 = ADC1_SSFIFO3_R;  // 12-bit result
}

void timer_init(uint32_t period)
{
  SYSCTL_RCGCTIMER_R |= 0x01;   // 4) activate timer0 
 
  TIMER0_CTL_R = 0x00000000;    // disable timer0A during setup
  TIMER0_CTL_R |= 0x00000020;   // enable timer0A trigger to ADC
  TIMER0_CFG_R = 0;             // configure for 32-bit timer mode
  TIMER0_TAMR_R = 0x00000002;   // configure for periodic mode, default down-count settings
  TIMER0_TAPR_R = 0;            // prescale value for trigger
  TIMER0_TAILR_R = period-1;    // start value for trigger
  TIMER0_IMR_R = 0x00000000;    // disable all interrupts
  TIMER0_CTL_R |= 0x00000001;   // enable timer0A 32-b, periodic, no interrupts

}

void ADC0_InitTimer0ATriggerSeq3PD3(){
  
  SYSCTL_RCGCADC_R |= 0x01;     // 1) activate ADC0 
  SYSCTL_RCGCGPIO_R |= 0x08;    // Port D clock
  GPIO_PORTD_DIR_R &= ~0x08;    // make PD3 input
  GPIO_PORTD_AFSEL_R |= 0x08;   // enable alternate function on PD3
  GPIO_PORTD_DEN_R &= ~0x08;    // disable digital I/O on PD3
  GPIO_PORTD_AMSEL_R |= 0x08;   // enable analog functionality on PD3
  
  ADC0_PC_R = 0x01;             // 2) configure for 125K samples/sec
  ADC0_SSPRI_R = 0x3210;        // 3) sequencer 0 is highest, sequencer 3 is lowest
  ADC0_ACTSS_R &= ~0x08;        // 5) disable sample sequencer 3
  ADC0_EMUX_R = (ADC0_EMUX_R&0xFFFF0FFF)+0x5000; // 6) timer trigger event
  ADC0_SSMUX3_R = 4;            // 7) PD4 is channel 4
  ADC0_SSCTL3_R = 0x06;         // 8) set flag and end                       
  ADC0_IM_R |= 0x08;            // 9) enable SS3 interrupts
  ADC0_ACTSS_R |= 0x08;         // 10) enable sample sequencer 3
  NVIC_PRI4_R = (NVIC_PRI4_R&0xFFFF00FF)|0x00004000; // 11)priority 2
  NVIC_EN0_R = 1<<17;           // 12) enable interrupt 17 in NVIC

}

void ADC1_InitTimer0ATriggerSeq3PD3(){
  
  SYSCTL_RCGCADC_R |= 0x02;     // 1) activate ADC0 
  SYSCTL_RCGCGPIO_R |= 0x08;    // Port D clock
  GPIO_PORTD_DIR_R &= ~0x04;    // make PD3 input
  GPIO_PORTD_AFSEL_R |= 0x04;   // enable alternate function on PD3
  GPIO_PORTD_DEN_R &= ~0x04;    // disable digital I/O on PD3
  GPIO_PORTD_AMSEL_R |= 0x04;   // enable analog functionality on PD3
  

  ADC1_PC_R = 0x01;             // 2) configure for 125K samples/sec
  ADC1_SSPRI_R = 0x3210;        // 3) sequencer 0 is highest, sequencer 3 is lowest	
  ADC1_ACTSS_R &= ~0x08;        // 5) disable sample sequencer 3
  ADC1_EMUX_R = (ADC0_EMUX_R&0xFFFF0FFF)+0x5000; // 6) timer trigger event
  ADC1_SSMUX3_R = 5;            // 7) PD4 is channel 4
  ADC1_SSCTL3_R = 0x06;         // 8) set flag and end                       
  ADC1_IM_R |= 0x08;            // 9) enable SS3 interrupts
  ADC1_ACTSS_R |= 0x08;         // 10) enable sample sequencer 3
  NVIC_PRI12_R = (NVIC_PRI12_R & ~0x80000000)|0x60000000; // 11)priority 2
  NVIC_EN1_R = 1<<19;           // 12) enable interrupt 17 in NVIC

}


int main(void)
{
    DisableInterrupts();	
    timer_init(16000);	
    ADC0_InitTimer0ATriggerSeq3PD3();	
    ADC1_InitTimer0ATriggerSeq3PD3();
    EnableInterrupts();
while(1){
	printf("%ld %ld\n",ADC0,ADC1);		
}
}