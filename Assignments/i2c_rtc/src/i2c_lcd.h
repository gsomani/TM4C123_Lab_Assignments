char key=0;int events=0;char event[14];char century[2],date[3],time[3],cur_date[11],cur_time[9];

void I2C3_init(void);
void DisableInterrupts(void);
void WaitForInterrupt(void);
void EnableInterrupts(void);
void init_systick_key_debounce();
void init_systick_time_update();
void init_keypad();
void LCD_OutCmd(unsigned char command);
void LCD_Clear(void);
void LCD_OutChar(char data);
void init_lcd();
void write_lcd(char text[]);
