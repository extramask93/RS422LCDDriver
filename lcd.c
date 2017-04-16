#include "lcd.h"

static inline void LCDSendData(uint8_t data)
{
	if(data&(1<<0)) PORT(LCD_D0PORT) |=(1<<LCD_D0);
	else PORT(LCD_D0PORT) &=~(1<<LCD_D0);
	if(data&(1<<1)) PORT(LCD_D1PORT) |=(1<<LCD_D1);
	else PORT(LCD_D1PORT) &=~(1<<LCD_D1);
	if(data&(1<<2)) PORT(LCD_D2PORT) |=(1<<LCD_D2);
	else PORT(LCD_D2PORT) &=~(1<<LCD_D2);
	if(data&(1<<3)) PORT(LCD_D3PORT) |=(1<<LCD_D3);
	else PORT(LCD_D3PORT) &=~(1<<LCD_D3);
	if(data&(1<<4)) PORT(LCD_D4PORT) |=(1<<LCD_D4);
	else PORT(LCD_D4PORT) &=~(1<<LCD_D4);
	if(data&(1<<5)) PORT(LCD_D5PORT) |=(1<<LCD_D5);
	else PORT(LCD_D5PORT) &=~(1<<LCD_D5);
	if(data&(1<<6)) PORT(LCD_D6PORT) |=(1<<LCD_D6);
	else PORT(LCD_D6PORT) &=~(1<<LCD_D6);
	if(data&(1<<7)) PORT(LCD_D7PORT) |=(1<<LCD_D7);
	else PORT(LCD_D7PORT) &=~(1<<LCD_D7);
}
void LCDWriteByte(unsigned char _data)
{
	
#if USE_RW ==1
	CLR_RW;
#endif
	SET_E;
	LCDSendData(_data);
	CLR_E;
#if USE_RW ==1
	//not implemented
#else
	_delay_us(120);
#endif
}
void LCDWriteCmd(uint8_t cmd)
{
	CLR_RS;
	LCDWriteByte(cmd);
}

void LCDWriteData(uint8_t data)
{
	SET_RS;
	LCDWriteByte(data);
}
void LCDClr(void)
{
	LCDWriteCmd(LCDC_CLS);
	#if USE_RW == 0
		_delay_ms(4.9);
	#endif
}
void LCDHome(void)
{
	LCDWriteCmd(LCDC_CLS|LCDC_HOME);
	#if USE_RW==0
		_delay_ms(4.9);
	#endif
}

void LCDCursorOn(void)
{
	LCDWriteCmd(LCDC_DISPLAYON|LCDC_CURSORON);
}
void LCDCursorOff(void)
{
	LCDWriteCmd(LCDC_DISPLAYON);
}

void LCDBlinkOn(void)
{
	LCDWriteCmd(LCDC_DISPLAYON|LCDC_CURSORON|LCDC_BLINKON);
}
void LCDBlinkOFF(void)
{
	LCDWriteCmd(LCDC_DISPLAYON);
}

void LCDStr(char * str)
{
	while(*str) LCDWriteData(*str++);
}
void LCDPrintChar(char data)
{
	LCDWriteData(data);
}
void LCDGotoXY(uint8_t x, uint8_t y)
{
	uint8_t DDRAMAddr;
	// remap lines into proper order
	switch(y)
	{
	case 0: DDRAMAddr = LCD_LINE1+x; break;
	case 1: DDRAMAddr = LCD_LINE2+x; break;
	case 2: DDRAMAddr = LCD_LINE3+x; break;
	case 3: DDRAMAddr = LCD_LINE4+x; break;
	default: DDRAMAddr = LCD_LINE1+x;
	}
	// set data address
	LCDWriteCmd(LCDC_DDRAM | DDRAMAddr);
}

void LCDInit()
{
	/*
	Setting Mega ports to output
	*/
	DDR(LCD_D7PORT) |= (1<<LCD_D7);
	DDR(LCD_D6PORT) |= (1<<LCD_D6);
	DDR(LCD_D5PORT) |= (1<<LCD_D5);
	DDR(LCD_D4PORT) |= (1<<LCD_D4);
	DDR(LCD_D3PORT) |= (1<<LCD_D3);
	DDR(LCD_D2PORT) |= (1<<LCD_D2);
	DDR(LCD_D1PORT) |= (1<<LCD_D1);
	DDR(LCD_D0PORT) |= (1<<LCD_D0);
	DDR(LCD_RSPORT) |= (1<<LCD_RS);
	DDR(LCD_EPORT) |= (1<<LCD_E);
	#if USE_RW==1
	DDR(LCD_RWPORT) |=(1<<LCD_RW);
	#endif


	//8bit mode inint according to datasheet
	_delay_ms(120);
	LCDWriteCmd(MODE_8BIT);
	_delay_ms(5);
	LCDWriteCmd(MODE_8BIT);
	_delay_ms(1);
	LCDWriteCmd(MODE_8BIT);
	_delay_ms(1);
	LCDWriteCmd(LCDC_FUNCTIONSET); //0011NF**
	LCDWriteCmd(LCDC_DISPLAYOFF); //turn off the display
	LCDClr(); //clear
	LCDWriteCmd(LCDC_ENTRYMODE);
	LCDWriteCmd(LCDC_DISPLAYON|LCDC_BLINKON|LCDC_CURSORON);
}



































