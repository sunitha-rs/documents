// ---------------------------------------------------------------------------
// *                        www.apsis-solutions.com
// *               Copyright (c) Apsis Solutions, Bangalore
// *--------------------------------------------------------------------------
// *    Project         : Generic
// *    Module          : Project Main
// *    Description     : Project Starts Here
// *					  	  
// *    Author          : Ajith Kirlaya
// *    Workfile        : Projectmain.c
// *    Revision        : 2.0
// *    Last changed on : Date: 2017-10-08
// *                     
// *    Last changed by : Ajith Kirlaya
// *
// *--------------------------------------------------------------------------

#include <p18f46k22.h>    
#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
#include "ProjectMain.h"  
#include "serial_codes.h"
#include "ADC.H" 
#include "PinAllocation.h"
#include "LCD.h"
#include "delay.h"
#include "timers.h"
#include "wifimodule.h" 

unsigned char guchLCDLine1String[17],guchLCDLine2String[17];
unsigned char uchBuffDisplay[] = "APSIS SOLUTIONS\n\r";
#pragma udata udata3
unsigned char uchRecieve150Ch[150];
unsigned int guchDataTowebsite;
extern unsigned int gunField1Value,gunField2Value,gunField3Value,gunField4Value,gunField5Value,gunField6Value,gunField7Value,gunField8Value;
unsigned long ulVoltageInVolts;
extern unsigned char guchField5Value;
extern unsigned int gunDataFromFields;
unsigned char guchSendingData[10]= "TESTED OK";
extern unsigned char guchUartReceiveData[];
extern unsigned char guchClockflag;
unsigned int guchData;

extern unsigned char guchNewDataReceived;






void fnDisableGlobalInterrupts(void)
{
	INTCONbits.GIE 	= 0;
	INTCONbits.PEIE = 0;
}
void fnEnableGlobalInterrupts(void)
{
	INTCONbits.GIE 	= 1;
	INTCONbits.PEIE = 1;
}
// Requirement was to receive some thing on interupt on U2
void fnUART1InterruptSetting (unsigned char uchEn_Dis)
{
	// Data from PIC18F46K22 Datasheet page number 265
	if(uchEn_Dis == 1)
	{
		PIE1bits.RC1IE	= 1;
		IPR1bits.RC1IP	= 1; // High Interrupt Priority
		fnEnableGlobalInterrupts();
	}
	else
	{
		PIE1bits.RC1IE	= 0;
		IPR1bits.RC1IP	= 1; // High Interrupt Priority
		fnDisableGlobalInterrupts();
	}
	
}






void fnMain_Project()
{	
	unsigned int unTemperatureValue; 
	unsigned int unADCvalueSmokeSensor =0;
	unsigned int unDataFromThingspeakforDeviceControl = 0;
	unsigned int unDataFromField1,unDataFromField2;
	unsigned char uchCommandForRobot;
	unsigned char a=10,b=5,c=0;
	unsigned char uchData;
	unsigned char uchData1;
	
 	unsigned char uchRecieve150Ch[10],i = 0;
 	unsigned char uchtemp;
 	
 	unsigned char uchTempvalue[];
 	unsigned int  unADCvalue1=0;
 	unsigned int  unADCvalue2=0;
 	unsigned int  unADCvalue3=0;
 	unsigned int  unADCvalue4=0;
 	unsigned int unIterationCount;
 	unsigned int unarrtemp[10];
 	unsigned char uchLightStatus   =  '0';
	unsigned char uchFanStatus     =  '0';
	unsigned char uchColourStatus  =  'W';
	unsigned char uchBlindStatus   =  '0';
	unsigned char uchAlarmStatus   =  '0';

	
 	ANSELA = 0X00;
 	ANSELB = 0X00;
	Dir_DEVICE_0   = SET_OUTPUT;
	Dir_DEVICE_1   = SET_OUTPUT;
	Dir_DEVICE_2   = SET_OUTPUT;
	Dir_DEVICE_3   = SET_OUTPUT;
	Dir_DEVICE_4   = SET_OUTPUT;
	Dir_DEVICE_5   = SET_OUTPUT;

  
  


	printf("program begins...\n\r");
	   fn_clear_display();

	strcpypgm2ram(guchLCDLine1String,"PROGRAM STARTS");
	fn_lcd_select_line_and_location(LCD_LINE_1,LOCATION_0);
	fn_Display_String_LCD(guchLCDLine1String);


	Delay_in_ms(1000);


	fnUART1InterruptSetting(1);




	while(1)
	{
		printf("\n\r Waiting for Commands.... \n\r");
		while(guchNewDataReceived != TRUE); // While New Data not present remain here
		guchNewDataReceived = FALSE;

		//printf("guchUartReceiveData = %s", guchUartReceiveData);
		
		fnTransmit_Serial(guchUartReceiveData[0], UART2);
		fnTransmit_Serial(guchUartReceiveData[1], UART2);




		if(guchUartReceiveData[0]=='B')
		{
		   if(guchUartReceiveData[1]=='U')
		   {
		   	   strcpypgm2ram(guchLCDLine1String,"     BED UP     ");
		   	   fn_lcd_select_line_and_location(LCD_LINE_1,LOCATION_0);
			   fn_Display_String_LCD(guchLCDLine1String);  
			    delay_in_seconds(2);
			      printf("BEDUP command recognised\n\r");
		   }
		   else if(guchUartReceiveData[1]=='D')
		   {
		   	   strcpypgm2ram(guchLCDLine1String,"    BED DOWN     ");
		   	   fn_lcd_select_line_and_location(LCD_LINE_1,LOCATION_0);
			   fn_Display_String_LCD(guchLCDLine1String); 
			   delay_in_seconds(2);
			    printf("BED DOWN command recognised\n\r");

		   }	
		   else
		   {
		   printf("bed command not found\n\r");
		   }
		}
//		//Blinds
		else if(guchUartReceiveData[0]=='D')
		{
		   if(guchUartReceiveData[1]=='O')
		   { 
		       strcpypgm2ram(guchLCDLine1String,"  BLINDS OPEN   ");
		   	   fn_lcd_select_line_and_location(LCD_LINE_1,LOCATION_0);
			   fn_Display_String_LCD(guchLCDLine1String);
			   delay_in_seconds(2);
			    printf(" BLINDS OPEN command recognised\n\r");

		   }

		   else if(guchUartReceiveData[1]=='C')
		   {   
		   	   strcpypgm2ram(guchLCDLine1String,"  BLINDS CLOSE  ");
		   	   fn_lcd_select_line_and_location(LCD_LINE_1,LOCATION_0);
			   fn_Display_String_LCD(guchLCDLine1String); 
			   	delay_in_seconds(2);
			   	  printf(" BLINDS CLOSE command recognised\n\r");
		   }
	
		   else
		   {
	        printf("blinds command not found\n\r");
		   }
	}

	
		else if(guchUartReceiveData[0]=='L')
		{
		   if(guchUartReceiveData[1]=='N')
		   {  
		   	   strcpypgm2ram(guchLCDLine1String,"    LIGHT ON    ");
		   	   fn_lcd_select_line_and_location(LCD_LINE_1,LOCATION_0);
			   fn_Display_String_LCD(guchLCDLine1String); 
			   delay_in_seconds(2);
			   printf(" LIGHT ON command recognised\n\r");
		   }

		 
		   else if(guchUartReceiveData[1]=='F')
		   { 
		   	   strcpypgm2ram(guchLCDLine1String,"    LIGHT OFF   ");
		   	   fn_lcd_select_line_and_location(LCD_LINE_1,LOCATION_0);
			   fn_Display_String_LCD(guchLCDLine1String);
			   	delay_in_seconds(2);
                printf(" LIGHT OFF command recognised\n\r");

		   }	
		   else
		   {
            printf("light command not found\n\r");
		   }
		}
		
		//Light Mode
		else if(guchUartReceiveData[0]=='M')
		{
		   if(guchUartReceiveData[1]=='R')
		   {   
		   	   strcpypgm2ram(guchLCDLine1String,"    MOOD RED    ");
		   	   fn_lcd_select_line_and_location(LCD_LINE_1,LOCATION_0);
			   fn_Display_String_LCD(guchLCDLine1String);
			   	delay_in_seconds(2);
			   	printf(" MOOD RED command recognised\n\r");

		   }
		   else if(guchUartReceiveData[1]=='G')
		   {  
		   	   strcpypgm2ram(guchLCDLine1String,"   MOOD GREEN   ");
		   	   fn_lcd_select_line_and_location(LCD_LINE_1,LOCATION_0);
			   fn_Display_String_LCD(guchLCDLine1String);
			   delay_in_seconds(2);
			   printf(" MOOD GREEN command recognised\n\r");

		   }
		   else if(guchUartReceiveData[1]=='B')
		   { 
		   	   strcpypgm2ram(guchLCDLine1String,"    MOOD BLUE    ");
		   	   fn_lcd_select_line_and_location(LCD_LINE_1,LOCATION_0);
			   fn_Display_String_LCD(guchLCDLine1String);
			   delay_in_seconds(2);
			   printf(" MOOD BLUE command recognised\n\r");

		   }
		   else if(guchUartReceiveData[1]=='Y')
		   { 
		   	  strcpypgm2ram(guchLCDLine1String,"   MOOD YELLOW  ");
		   	  fn_lcd_select_line_and_location(LCD_LINE_1,LOCATION_0);
			  fn_Display_String_LCD(guchLCDLine1String);
			  delay_in_seconds(2);
			  printf(" MOOD YELLOW command recognised\n\r");
		   }
		   	
		   else if(guchUartReceiveData[1]=='W')
		   {   
		   	  strcpypgm2ram(guchLCDLine1String,"   MOOD WHITE   ");
		   	  fn_lcd_select_line_and_location(LCD_LINE_1,LOCATION_0);
			  fn_Display_String_LCD(guchLCDLine1String);
			 delay_in_seconds(2);
			 printf(" MOOD WHITE  command recognised\n\r");
		   }
		   else
		   {
            printf(" moods lights command not found\n\r");
		   }
		}
		//Fan
		else if(guchUartReceiveData[0]=='F')
		{
		   if(guchUartReceiveData[1]=='N')
		   { 
		   	  strcpypgm2ram(guchLCDLine1String,"     FAN ON     ");
		   	  fn_lcd_select_line_and_location(LCD_LINE_1,LOCATION_0);
			  fn_Display_String_LCD(guchLCDLine1String);
		   	  delay_in_seconds(2);
		   	  	 printf(" FAN ON   command recognised\n\r");

		   }
		   else if(guchUartReceiveData[1]=='F')
		   {  
		   	  strcpypgm2ram(guchLCDLine1String,"    FAN OFF     ");
		   	  fn_lcd_select_line_and_location(LCD_LINE_1,LOCATION_0);
			  fn_Display_String_LCD(guchLCDLine1String);
			  delay_in_seconds(2);
			  printf(" FAN OFF   command recognised\n\r");

		   }	
		   else
		   {  
  			printf("fan command not found\n\r");
		   }
		}
		//Alaram
		if(guchUartReceiveData[0]=='A')
		{
		   if(guchUartReceiveData[1]=='N')
		   { 
		       strcpypgm2ram(guchLCDLine1String,"     ALARM ON    ");
		       fn_lcd_select_line_and_location(LCD_LINE_1,LOCATION_0);
			   fn_Display_String_LCD(guchLCDLine1String);  
			  delay_in_seconds(2);
			  printf(" ALARM ON   command recognised\n\r");
			   
		   	  
		   }
		   else if(guchUartReceiveData[1]=='F')
		   { 
		      strcpypgm2ram(guchLCDLine1String,"    ALARM OFF   ");
		   	  fn_lcd_select_line_and_location(LCD_LINE_1,LOCATION_0);
			  fn_Display_String_LCD(guchLCDLine1String);
			  delay_in_seconds(2);
			  printf(" ALARM OFF  command recognised\n\r");
		   }	
		   else
		   {  
	  		printf("alarm command not found\n\r");
		   }
		}
     }
	
}




