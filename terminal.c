#define BAUD 115200 //USART speed in bps
#define _UBRR (F_CPU/16/BAUD-1) //calculate UBRR register value for given baud and CPU clock

#include "lcd.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#define DEVICE_ID '1'
#define PACKET_SIZE 36
#define PACKET_DELIMITER 0x04 //end of trnasmission char
#define USART_RX_BUF_SIZE 64 //buffer size
#define USART_RX_BUF_MASK ( USART_RX_BUF_SIZE -1)
#define PRINT 'p'
#define MOVE 'm'
#define COMMAND 'c'
//volatile data
volatile char USART_RxBuf[USART_RX_BUF_SIZE];
volatile uint8_t USART_RxHead;
volatile uint8_t USART_RxTail;
//function declarations
char USARTGetc(void);
void USARTInit(uint32_t _ubrr,uint8_t stopBits);
void EvaluatePosition(char[]);
void EvaluateCommand(char[]);
void Print(char[]);
//
int main()
{
	LCDInit(); // init LCD
	LCDStr("  LCD <-> RS422"); //splash screen
	LCDGotoXY(0,1);
	LCDStr("interface Rev1.0");
	_delay_ms(2000);
	LCDClr();
	LCDGotoXY(0,0);
	USARTInit(_UBRR,2); 
	UCSRB|=(1<<RXCIE); //read interrupt on
	sei(); // global interrupts on
	char command[PACKET_SIZE];
	int i=0;
	int checkCommand=0;
	while(1)
	{
		while(USART_RxHead!=USART_RxTail) //something in the buffer
		{
			command[i]=USARTGetc(); // get that something
			if((command[i] == PACKET_DELIMITER && i!=0) || (i>=PACKET_SIZE-1)) //if 'end of the command character'
			//in the buffer or exceeded packet size
			{
				checkCommand=1; //inform rest of the code that commad awaits 
				break; //escape while loop
				
			}
			i++; //shift to the next empty slot in the command array
		}
		
		if(checkCommand) //evaluate command
		{
			if(command[0]==PACKET_DELIMITER && command[1]==DEVICE_ID) //check whether good formating is used
			//, where @-start_byte, 1 - identifier
			{
				
				switch(command[2])
				{
					case MOVE: EvaluatePosition(command); break;
					case PRINT: Print(command); break; 		
					case COMMAND: EvaluateCommand(command); break;	  
					default: break;
				}
			}
			checkCommand=0; //command evaluated
			i=0; //flush command array by setting index to 1'st member

		}
	}
}

ISR(USART_RXC_vect)
{
	uint8_t tmp_head;
	char data;
	data=UDR;
	tmp_head=(USART_RxHead +1)&USART_RX_BUF_MASK;
	if(tmp_head==USART_RxTail)
	{
          //if buffer overflow then overwrite oldest data
	}
	else
	{
		USART_RxHead=tmp_head;
		USART_RxBuf[tmp_head]=data;
	}
}

void USARTInit(uint32_t _ubrr,uint8_t stopBits)
{
	UBRRH= (uint8_t)(_ubrr>>8); // four MSBs
	UBRRL= (uint8_t) _ubrr; // eight LSBs
	UCSRB=(1<<RXEN); //enable  receiver
	UCSRC=(1<<USBS)|(1<<URSEL)|(3<<UCSZ0); // (two stop bits)(URSEL set to one while
   // writing to UCSRC)(8 bit character size)
	if(stopBits!=2)
	UCSRC&=~(1<USBS);
}

char USARTGetc(void)
{
	if(USART_RxHead==USART_RxTail) return 0; //if no characters awaiting
	USART_RxTail = (USART_RxTail+1) & USART_RX_BUF_MASK;
	return USART_RxBuf[USART_RxTail]; //return one character from the buffer
}
void EvaluatePosition(char command[])
{
	
	uint8_t position=5;
	uint8_t x = atoi(&command[3]);
	if(x>9)
		position=6;
	uint8_t y = atoi(&command[position]);
	LCDGotoXY(x,y);
		
}

void EvaluateCommand(char command[])
{
	switch(command[3])
		{
			case '0': LCDClr(); break;
			case '1': LCDBlinkOn(); break;
			case '2': LCDBlinkOFF(); break;
			case '3': LCDCursorOn(); break;
			case '4': LCDCursorOff(); break;
			default: break;
		}
}
void Print(char command[])
{
	uint8_t z = 3;
	while(command[z]!=PACKET_DELIMITER)
	LCDWriteData(command[z++]);
}

