#include<stdint.h>
#include"inc/tm4c123gh6pm.h"
#include"i2c_lcd.h"
 
#define SLAVE_ADDR 0x68            
#define LCD_DISPLAY 1

void lcd_display_event(char num)
{
				 LCD_OutChar(num);
				 if(events==2 || events==4) LCD_OutChar('/');
				 else if (events==8) 	{LCD_OutCmd(0xC0);write_lcd("Time=");}
				 else if(events==10 || events==12) LCD_OutChar(':');
				 if(events==14) LCD_OutCmd(0xC);
}

static int I2C_wait_till_done(void)
{
    while(I2C3_MCS_R & 1)
        ;                           /* wait until I2C master is not busy */
    return I2C3_MCS_R & 0xE;        /* return I2C error code */
}
 
/* Write one byte only */
/* byte write: S-(saddr+w)-ACK-maddr-ACK-data-ACK-P */
char I2C3_byteWrite(int slaveAddr, char memAddr, char data)
{
    char error;
 
    /* send slave address and starting address */
    I2C3_MSA_R = slaveAddr << 1;
    I2C3_MDR_R = memAddr;
    I2C3_MCS_R = 3;                 /* S-(saddr+w)-ACK-maddr-ACK */
    error = I2C_wait_till_done();   /* wait until write is complete */
    if(error) return error;

    /* send data */
    I2C3_MDR_R = data;
    I2C3_MCS_R = 5;                 /* -data-ACK-P */
    error = I2C_wait_till_done();   /* wait until write is complete */
    while( I2C3_MCS_R & 0x40 )
        ;                           /* wait until bus is not busy */
    error = I2C3_MCS_R & 0xE;
    if(error) return error;
 
    return 0;                       /* no error */
}
 
/* Read memory */
/* read: S-(saddr+w)-ACK-maddr-ACK-R-(saddr+r)-ACK-data-ACK-data-ACK-...-data-NACK-P */
char I2C3_read(int slaveAddr, char memAddr, int byteCount, char* data)
{
    char error;
 
    if (byteCount <= 0)
        return -1;                  /* no read was performed */
 
    /* send slave address and starting address */
    I2C3_MSA_R = slaveAddr << 1;
    I2C3_MDR_R = memAddr;
    I2C3_MCS_R = 3;                 /* S-(saddr+w)-ACK-maddr-ACK */
    error = I2C_wait_till_done();
    if(error)
        return error;

    /* to change bus from write to read, send restart with slave addr */
    I2C3_MSA_R = (slaveAddr << 1) + 1;   /* restart: -R-(saddr+r)-ACK */
 
    if( byteCount == 1 )            /* if last byte, don't ack */
        I2C3_MCS_R = 7;             /* -data-NACK-P */
    else                            /* else ack */
        I2C3_MCS_R = 0xB;           /* -data-ACK- */
    error = I2C_wait_till_done();
    if(error) return error;
 
    *data++ = I2C3_MDR_R;           /* store the data received */
 
    if( --byteCount == 0 ) {        /* if single byte read, done */
        while( I2C3_MCS_R & 0x40 )
            ;                       /* wait until bus is not busy */
        return 0;                   /* no error */
    }
 
    /* read the rest of the bytes */
    while( byteCount > 1 ) {
        I2C3_MCS_R = 9;              /* -data-ACK- */
        error = I2C_wait_till_done();
        if(error) return error;
        byteCount--;
        *data++ = I2C3_MDR_R;        /* store data received */
    }
 
    I2C3_MCS_R = 5;                 /* -data-NACK-P */
    error = I2C_wait_till_done();
    *data = I2C3_MDR_R;             /* store data received */
    while( I2C3_MCS_R & 0x40 )
        ;                           /* wait until bus is not busy */
 
    return 0;                       /* no error */
}
 
void read_cur_time()
{
	char err,d[3],t[3];
	err = I2C3_read(SLAVE_ADDR, 0, 3, t);
	err = I2C3_read(SLAVE_ADDR, 4, 3, d);
	cur_date[0]=(d[0]>>4)+'0';cur_date[1]=(d[0] & 0xF)+'0';
	cur_date[3]=(d[1]>>4)+'0';cur_date[4]=(d[1] & 0xF)+'0';
	cur_date[8]=(d[2]>>4)+'0';cur_date[9]=(d[2] & 0xF)+'0';
	cur_time[0]=(t[2]>>4)+'0';cur_time[1]=(t[2] & 0xF)+'0';
	cur_time[3]=(t[1]>>4)+'0';cur_time[4]=(t[1] & 0xF)+'0';
	cur_time[6]=(t[0]>>4)+'0';cur_time[7]=(t[0] & 0xF)+'0';
}

void write_date_time()
{
	char err;int i;	
	date[0]=(event[0]<<4)|event[1];
	date[1]=(event[2]<<4)|event[3];
	century[0]=event[4]+'0';
	century[1]=event[5]+'0';
	date[2]=(event[6]<<4)|event[7];
	time[2]=(event[8]<<4)|event[9];
	time[1]=(event[10]<<4)|event[11];
	time[0]=(event[12]<<4)|event[13];
	for(i=6;i>=4;i--) err = I2C3_byteWrite(SLAVE_ADDR, i, date[i-4]);	
	for(i=2;i>=0;i--) err = I2C3_byteWrite(SLAVE_ADDR, i, time[i]);
}

void set_format_date_time()
{
	
cur_date[2]=cur_date[5]='/';cur_date[6]=century[0];cur_date[7]=century[1];
cur_time[2]=cur_time[5]=':';cur_time[8]=cur_date[10]=0;
}

void lcd_display_date_time()
{
	LCD_Clear();
	write_lcd(cur_date);
	LCD_OutCmd(0xC0);
	write_lcd(cur_time);
}

void SysTick_Handler(void)
{
    if(events==14)
    {	
		read_cur_time();
		lcd_display_date_time();
		return;	
    }		
    int row,col,value,num;
    GPIO_PORTE_DATA_R &=0xF0;
    if((GPIO_PORTC_DATA_R>>4)==0xF) {key=0;return;}
    if(key) return;
    GPIO_PORTE_DATA_R |=0x0F;		
    for(row=0;row<4;row++)
        {
	GPIO_PORTE_DATA_R = ~(1<<row);
    value=GPIO_PORTC_DATA_R>>4;
    if(value==0xF) continue;
    	for(col=0;col<4;col++)
    	{ 	if(!(value&1)) 
			{
			key = 1; num=key_decoder(row,col); 
			if(num) 
				{
				 event[events]=num-'0';	events++;
					lcd_display_event(num);	
				}
			return;
			}
    	      	value=value>>1;
    	 }
        }
}

int main(void)
{ 
DisableInterrupts();
init_keypad();
init_systick_key_debounce();
init_lcd();
write_lcd("Date=");
EnableInterrupts();
while(events<14)
	WaitForInterrupt();
DisableInterrupts();
I2C3_init();
write_date_time();
set_format_date_time();
init_systick_time_update();
EnableInterrupts();
while(1) {
WaitForInterrupt();
}
return 0;
} 
