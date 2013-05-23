4bit-LCD-C
==========
#include "p18f4520.h" 

#include "delays.h" 

#include "xlcd.h" 




#pragma config OSC = INTIO67   // Internal osc, RA6=CLKO, RA7=I/O
#pragma config FCMEN = OFF  	// Fail-Safe Clock Monitor disabled 
#pragma config IESO = OFF		// Oscillator Switchover mode disabled 
#pragma config WDT = OFF        // WDT disabled (control through SWDTEN bit)
#pragma config PWRT = OFF       // racmod  -> PWRT disabled
#pragma config MCLRE = ON       // MCLR pin enabled; RG5 input pin disabled
#pragma config XINST = OFF      // Instruction set extension disabled
#pragma config BOREN = OFF      // Brown-out controlled by software
#pragma config BORV = 3       // Brown-out voltage set for 2.0V, nominal
#pragma config STVREN = OFF		// Stack full/underflow will not cause Reset
//#pragma config CP0 = OFF, CP1 = OFF, CP2 = OFF, CP3 = OFF 			// Program memory block not code-protected


//LCD Control pins
 #define rs   LATDbits.LATD3   		/* PORT for RS */
 #define TRIS_rs  TRISDbits.TRISD3    	/* TRIS for RS */
 #define en    LATDbits.LATD2  		/* PORT for E  */
 #define TRIS_en   TRISDbits.TRISD2    	/* TRIS for E  */

//LCD Data pins
#define lcdport LATD

void Delay_ms(int d)
{
int i = 0;

	for ( ; d; --d) 
		for (i = 100;  i;  --i) ;
}


void lcd_ini();
void dis_cmd(unsigned char);
void dis_data(unsigned char);
void lcdcmd(unsigned char);
void lcddata(unsigned char);
void cleardt();
void line(unsigned char pos);

void main(void)
{
	unsigned char data0[]="CaxtronsFMradios";
	unsigned char data1[]="    Boombox.    ";
	unsigned char data2[]="By Austin,Graham";
	unsigned char data3[]=" Jamie, Yiannos ";
	unsigned int i=0;
	TRISD=0;			// Configure Port B as output port
	LATD=0;
	lcd_ini();			// LCD initialization
	while(data0[i]!='\0')
	{
		dis_data(data0[i]);
		Delay_ms(5);
		i++;	
	}
	 i=0;
	line(0xc0);
	while(data1[i]!='\0')
	{
		dis_data(data1[i]);
		Delay_ms(5);
		i++;	
	}
	 i=0;
	line(0x90);
	while(data2[i]!='\0')
	{
		dis_data(data2[i]);
		Delay_ms(5);
		i++;	
	}
	 i=0;
	line(0xd0);
	while(data3[i]!='\0')
	{
		dis_data(data3[i]);
		Delay_ms(5);
		i++;	
	}
}
void lcd_ini(void)	                    
{
	dis_cmd(0x02);		// To initialize LCD in 4-bit mode.
//	dis_cmd(0x28);		// To initialize LCD in 2 lines, 5x7 dots and 4bit mode.
	dis_cmd(0x0C);
	dis_cmd(0x06);
	dis_cmd(0x80);
}

void dis_cmd(unsigned char cmd_value)
{
	unsigned char cmd_value1;
	cmd_value1 = (cmd_value & 0xF0);	// Mask lower nibble because RB4-RB7 pins are being used
	lcdcmd(cmd_value1);			// Send to LCD
	cmd_value1 = ((cmd_value<<4) & 0xF0);	// Shift 4-bit and mask
	lcdcmd(cmd_value1);			// Send to LCD
}


void dis_data(unsigned char data_value)
{
	unsigned char data_value1;
	data_value1=(data_value&0xF0);
	lcddata(data_value1);
	data_value1=((data_value<<4)&0xF0);
	lcddata(data_value1);
}

void lcdcmd(unsigned char cmdout)
{
	lcdport=cmdout;		//Send command to lcdport=PORTD
	rs=0;						
	en=1;
	Delay_ms(10);
	en=0;
}

void lcddata(unsigned char dataout)
{
	lcdport=dataout;	//Send data to lcdport=PORTB
	rs=1;
	en=1;
	Delay_ms(10);
	en=0;
}

void cleardt(void)
{
	unsigned char dt=0x1;
	dis_cmd(dt);
	Delay_ms(2);
}

void line(unsigned char pos)
{
	dis_cmd(pos);
}
