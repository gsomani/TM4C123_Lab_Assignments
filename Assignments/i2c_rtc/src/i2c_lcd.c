#include<stdint.h>
#include"inc/tm4c123gh6pm.h"
#define     LCD_RS 0x40     /* PORTA BIT6 mask */
#define     LCD_EN 0x80     /* PORTA BIT7 mask */

#define LCD_DATA_PORT    (*((volatile uint32_t *)0x400053FC))    /* PORTB7 - PORTB0 */
#define LCD_CMD_PORT     (*((volatile uint32_t *)0x40004300))    /* PA7-PA6 */

void I2C3_init(void)
{
    SYSCTL_RCGCI2C_R |= 0x08;       /* enable clock to I2C3 */
    SYSCTL_RCGCGPIO_R |= 0x08;      /* enable clock to GPIOD */
 
    /* PORTA 7, 6 for I2C3 */
    GPIO_PORTD_AFSEL_R |= 0x03;     /* PORTA 0, 1 for I2C3 */
    GPIO_PORTD_PCTL_R &= ~0xFF; /* PORTA 0, 1 for I2C3 */
    GPIO_PORTD_PCTL_R |= 0x33;
    GPIO_PORTD_DEN_R |= 0x03;       /* PORTA 7, 6 as digital pins */
    GPIO_PORTD_ODR_R |= 0x02;       /* PORTA 7 as open drain */
 
    I2C3_MCR_R = 0x10;              /* master mode */
    I2C3_MTPR_R = 7;                /* 100 kHz @ 16 MHz */
}

void DisableInterrupts(void)
{
    __asm ("CPSID  I\n");
}

void WaitForInterrupt(void)
{
    __asm  ("WFI\n");
}


void EnableInterrupts(void)
{
    __asm  ("CPSIE  I\n");
}

void init_systick_key_debounce(){
   NVIC_ST_RELOAD_R = 800000-1;  /* reload with number of clocks per second */
    NVIC_ST_CTRL_R = 7;             /* enable SysTick interrupt, use system clock */
}

void init_systick_time_update(){
   NVIC_ST_RELOAD_R = 16000000-1;  /* reload with number of clocks per second */
    NVIC_ST_CTRL_R = 7;             /* enable SysTick interrupt, use system clock */
 
}

void init_keypad()
{
        SYSCTL_RCGCGPIO_R |= 0x00000014; // GPIO clock on PORT C and PORT E
        GPIO_PORTE_DIR_R |= 0x0F; // set PORT E (E0-E3) as output pins (by setting them as 1)
        GPIO_PORTE_ODR_R |= 0x0F; // set PORT E (E0-E3) as open drain output
        GPIO_PORTE_DEN_R |= 0x0F; // digital enable set for PORT E(E0-E3)
        GPIO_PORTC_DIR_R &= 0x0F; // set PORT C (C4-C7) as input pins (by setting them as 0)
        GPIO_PORTC_PUR_R |= 0xF0; // set PORT C (C4-C7) as pull-up
        GPIO_PORTC_DEN_R |= 0xF0; // // digital enable set for PORT C(C4-C7)
}

char key_decoder(int row,int col)
{
	if(col==3) return 0;
	if(row==3){
		if(col==1) return '0';
		else return 0;
	}
	return row*3+col+1+'0';
}

void delayMs(int n)
{
int i, j;
for(i = 0 ; i < n; i++)
for(j = 0; j < 3180; j++) {}
}

void delayUs(int n)
{
    int i, j;

    for( i = 0 ; i < n; i++ )
        for( j = 0; j < 3; j++ ) {} /* do nothing for 1 us */
}

void LCD_OutCmd(unsigned char command)
{
    LCD_DATA_PORT = command;
    LCD_CMD_PORT = 0;           /* E=0, R/W=0, RS=0 */
    delayUs(6);                 /* wait 6us */
    LCD_CMD_PORT = LCD_EN;      /* E=1, R/W=0, RS=0 */
    delayUs(6);                 /* wait 6us */
    LCD_CMD_PORT = 0;           /* E=0, R/W=0, RS=0 */
    delayUs(40);                /* wait 40us */
}

/* Clear the LCD */
void LCD_Clear(void)
{
    LCD_OutCmd(0x01);           /* Clear Display */
    delayUs(1600);              /* wait 1.6ms */
    LCD_OutCmd(0x02);           /* Cursor to home */
    delayUs(1600);              /* wait 1.6ms */
}

void LCD_OutChar(char data)
{
    LCD_DATA_PORT = data;
    LCD_CMD_PORT = LCD_RS;      /* E=0, R/W=0, RS=1 */
    delayUs(6);                 /* wait 6us */
    LCD_CMD_PORT = LCD_EN | LCD_RS; /* E=1, R/W=0, RS=1 */
    delayUs(6);                 /* wait 6us */
    LCD_CMD_PORT = LCD_RS;      /* E=0, R/W=0, RS=1 */
    delayUs(40);                /* wait 40us */
}

void init_lcd()
{
    	SYSCTL_RCGCGPIO_R |= 0x00000003;
    	GPIO_PORTB_DIR_R |= 0xFF;
    	GPIO_PORTB_DEN_R |= 0xFF;
    	GPIO_PORTA_DIR_R |= 0xC0;
    	GPIO_PORTA_DEN_R |= 0xC0;
    	delayMs(60); 
	LCD_OutCmd(0x38);
	delayMs(5);		
	LCD_OutCmd(0x38);
	delayUs(100);
	LCD_OutCmd(0x38);       
        LCD_OutCmd(0x8);
	LCD_Clear();        
	LCD_OutCmd(0xE);
        LCD_OutCmd(0xF);	
}

void write_lcd(char text[])
{
	for (int i=0;text[i] && i<16;i++)
		LCD_OutChar(text[i]);
}
