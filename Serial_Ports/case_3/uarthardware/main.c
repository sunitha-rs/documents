// ---------------------------------------------------------------------------
// *                        www.apsis-solutions.com
// *               Copyright (c) Apsis Solutions, Bangalore
// *--------------------------------------------------------------------------
// *    Project         : Generic
// *    Module          : Main
// *    Description     : Used to set the clock frequency,Initilise the ADC,UART1 or UART2,LCD_16X2 and 7_segment
// *    Author          : Pankaj Sharma
// *    Workfile        : main.c
// *    Revision        : 1.0
// *    Last changed on : Date: 2012-08-03
// *    Last changed by : Daya Shankar Singh
// *    Last changed on : Date: 2014-10-19
// *    Last changed by : Madhusudhan B L
// *
// *--------------------------------------------------------------------------
/*	
	Good practice
	1. Downsizing Your Variables
	2. Use Unsigned Types
	3. Stay Away from Floating Point
	4. Make Use of bit Variables
	5. Use Locals instead of Globals
	6. Use Internal Memory for Variables - DATA, IDATA, PDATA, XDATA.
	7. Use Macros Instead of Functions - For speed
	   Use functions Instead of Macros - For Code space less
*/


//#pragma config FOSC 	= INTIO7
//#pragma config PLLCFG 	= OFF
//#pragma config PRICLKEN = ON
//#pragma config FCMEN 	= OFF
//#pragma config IESO 	= OFF
//#pragma config PWRTEN 	= ON
//#pragma config BOREN	= ON
//#pragma config BORV 	= 285
//#pragma config WDTEN 	= OFF 
//#pragma config WDTPS	= 32768
//#pragma config CCP2MX 	= PORTB3
//#pragma config PBADEN 	= OFF
//#pragma config CCP3MX 	= PORTE0
//#pragma config HFOFST 	= OFF
//#pragma config T3CMX 	= PORTC0
//#pragma config P2BMX 	= PORTD2
//#pragma config MCLRE 	= EXTMCLR
//#pragma config STVREN 	= OFF
//#pragma config LVP 		= OFF
//#pragma config XINST 	= OFF
//#pragma config DEBUG 	= ON

#pragma config FOSC 	= INTIO67
#pragma config WDTEN 	= OFF
#pragma config LVP 		= OFF
#pragma config BOREN 	= OFF
#pragma config MCLRE    = EXTMCLR



#include <p18f46k22.h>  
#include <stdio.h> 
#include <string.h>
#include "P18F46K22generic.h"
#include "main.h"
#include "clock_setting.h"
#include "ProjectMain.h"
#include "serial_codes.h"
#include "ADC.H" 
#include "LCD.h"
#include "PinAllocation.h"
#include "delay.h"
#include "timers.h"


extern unsigned char guchLCDLine1String[17],guchLCDLine2String[17];
unsigned char guchTimerCount=0;
extern unsigned char guchTimerTimeOutCount;
unsigned char guchTimeOutFlag = 0;
unsigned char i = 0;
unsigned char guchUARTInterruptData;
unsigned char guchUartReceiveData[40];
unsigned char guchClockflag = 0;
unsigned char guchNewDataReceived = FALSE;

#pragma code high_vector=0x08
void interrupt_at_high_vector(void)
{
   _asm goto fnISR_HighVector _endasm 
}
#pragma code

#pragma interrupt fnISR_HighVector
void fnISR_HighVector(void)
{
	if(PIR1bits.RC1IF == 1) // Data received on UART1
	{
		fnUART1InterruptSetting(0);
		guchUARTInterruptData = RCREG1;
		PIR1bits.RC1IF = 0;
		if(guchUARTInterruptData == '*')
		{			
			for(i=0; i<2; i++)
			{
				guchUartReceiveData[i] = uchfnReceive_Serial(UART1);
				//fnTransmit_Serial(guchUartReceiveData[i],UART1); // Enable for Debug Only
			}
			//guchClockflag = 1;
			guchNewDataReceived = TRUE;
			guchUARTInterruptData = 0;
			guchUartReceiveData[i] = 0;
		}			
	}
	fnUART1InterruptSetting(1);
}	



#pragma interrupt timer0_isr
void timer0_isr(void)
{
	unsigned char i;

		
	if(INTCONbits.TMR0IF==1)
	{
		INTCONbits.TMR0IF = 0;
		guchTimerCount++;	
			

		if(guchTimerCount>guchTimerTimeOutCount)
		{
			guchTimerCount = 0;
			T0CONbits.TMR0ON  = 0;
			guchTimerTimeOutCount = 0;
			guchTimeOutFlag=1;
			printf("Timer TimeOut\n\r");
			
		}
		else
		{
			fnoneSecondTimerDelay();
		}
	}
}



void main()
{	

	FnDeviceInitialisation();

	fnMain_Project();
}


void FnDeviceInitialisation()
{	
	fn_Set_Clock_Frequency();
	printf("I am working\n");	
	ANSELA=0x00; 
	ANSELB=0x00;
	ANSELC=0x00;
	ANSELD=0x00;
	ANSELEbits.ANSE0 = 0;
	ANSELEbits.ANSE1 = 0;
	ANSELEbits.ANSE2 = 0;

	
	//pins used in precision farming
	


			
	WPUB=0xc3;
	INTCON2bits.NOT_RBPU = 0;
	fn_Initialize_Serial(UART1);
	fn_Initialize_Serial(UART2);
	fnInitializeADC();
		
	lcd_16_2_Initialisation();
	fnTimer0Initialize();
	delay_in_half_seconds(1);
	strcpypgm2ram(guchLCDLine1String,"APSIS  SOLUTIONS");
	strcpypgm2ram(guchLCDLine2String,"  IOT PLATFORM  ");
	fn_lcd_select_line_and_location(LCD_LINE_1,LOCATION_0);
	fn_Display_String_LCD(guchLCDLine1String);
	fn_lcd_select_line_and_location(LCD_LINE_2,LOCATION_0);
	fn_Display_String_LCD(guchLCDLine2String);	
	delay_in_half_seconds(2);
	fn_clear_display();
	
}

