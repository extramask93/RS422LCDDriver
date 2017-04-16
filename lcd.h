#define F_CPU 3686400UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
// definicje rozmiarów wyświetlacza
#define LCD_Y 2
#define LCD_X 16
#define USE_RW 0 //rw do masy,USE_RW=1 nie zaimplementowane
//definicje wykorzystywanych portów (data)
#define LCD_D7PORT B
#define LCD_D7 1
#define LCD_D6PORT B
#define LCD_D6 2
#define LCD_D5PORT B
#define LCD_D5 3
#define LCD_D4PORT B
#define LCD_D4 4
#define LCD_D3PORT B
#define LCD_D3 5
#define LCD_D2PORT C
#define LCD_D2 0
#define LCD_D1PORT C
#define LCD_D1 1
#define LCD_D0PORT C
#define LCD_D0 2
//definicje wykorzystywanych portów(cmd)
#define LCD_RSPORT C
#define LCD_RS 4

#define LCD_RWPORT B
#define LCD_RW 2

#define LCD_EPORT C
#define LCD_E 3
//adresy w pamięci DDRAM
#if ( (LCD_Y == 4)&&(LCD_X==20))
#define LCD_LINE1 0x00
#define LCD_LINE2 0x28
#define LCD_LINE3 0x14
#define LCD_LINE4 0x54
#else
#define LCD_LINE1 0x00
#define LCD_LINE2 0x40
#define LCD_LINE3 0x10
#define LCD_LINE4 0x50
#endif

//makra upraszczajace dostep do portów
#define PORT(x) SPORT(x)
#define SPORT(x) (PORT##x)

#define PIN(x) SPIN(x)
#define SPIN(x) (PIN##x)

#define DDR(x) SDDR(x)
#define SDDR(x) (DDR##x)


//makro operacji na sygnalach sterujacych RS, RW, E

#define SET_RS PORT(LCD_RSPORT) |= (1<<LCD_RS)
#define CLR_RS PORT(LCD_RSPORT) &=  ~(1<<LCD_RS)

#define SET_RW PORT(LCD_RWPORT) |= (1<<LCD_RW)
#define CLR_RW PORT(LCD_RWPORT) &=  ~(1<<LCD_RW)

#define SET_E PORT(LCD_EPORT) |= (1<<LCD_E)
#define CLR_E PORT(LCD_EPORT) &=  ~(1<<LCD_E)
// commands
#define MODE_8BIT 0x30
#define LCDC_DISPLAYOFF 0x08
#define LCDC_CLS 0x01
#define LCDC_DISPLAYON 0x0C
#define LCDC_FUNCTIONSET 0x38 //proprer setiing of the display -> D5|8bits|2rows
#define LCDC_ENTRYMODE 0x06 // setting the way the characters are written to 
//display(cursor form L to R, display not shifting)
//funkcja do przesyłu danych na wyswietlacz
#define LCDC_DISPLAYONOFF 0x0C
#define LCDC_CURSORON 0x02
#define LCDC_BLINKON 0x01
#define LCDC_HOME 0x02
#define LCDC_DDRAM 0x80 
static inline void LCDSendData(uint8_t data);
void LCDWriteByte(unsigned char _data);
void LCDWriteCmd(uint8_t cmd);
void LCDWriteData(uint8_t data);
void LCDClr(void);
void LCDHome(void);
void LCDCursorOn(void);
void LCDCursorOff(void);
void LCDBlinkOn(void);
void LCDBlinkOFF(void);
void LCDStr(char * str);
void LCDGotoXY(uint8_t x, uint8_t y);
void LCDInit();
void LCDPrintChar(char data);

