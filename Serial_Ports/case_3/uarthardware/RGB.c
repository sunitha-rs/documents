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
#include "RGB.h"



typedef struct {
    unsigned char   r;
    unsigned char   b;
    unsigned char   g;
} ws2812_ptr;

ws2812_ptr          led;
void ws2812_send(ws2812_ptr* led);
// unsigned long int bitflip(unsigned char b);
// void fnRGBRedcolor();
// void fnRGBGreenColor();
// void fnRGBYellowColor();
// void ws2812_send(ws2812_ptr* led);

// #define      OUTPUT          0
//Dir_PIN = OUTPUT;
//struct stMainFlags Flags;

    
        
//typedef struct {
//   unsigned char   r;
 //   unsigned char   b;
 //   unsigned char   g;
//} ws2812_ptr;

//ws2812_ptr          led;

// transmit a single WS2812


void fnRGBRedcolor(void)
{
        printf("LED_RED\n\r");
        Dir_PIN = OUTPUT;
        led.r = 255;
        led.g = 0;
        led.b = 0;

        ws2812_send(&led);
        
        
}
void fnRGBGreenColor(void)
{
        printf("LED_GREEN\n\r");
        Dir_PIN = OUTPUT;
        led.r = 0;
        led.g = 255;
        led.b = 0;

        ws2812_send(&led);
        
        
}

void fnRGBYellowColor(void)
{
        printf("LED_YELLOW\n\r");
        Dir_PIN = OUTPUT;
        led.r = 255;
        led.g = 255;
        led.b = 0;

        ws2812_send(&led);
        
        
}

void fnRGBBlueColor(void)
{
        printf("LED_BLUE\n\r");
        Dir_PIN = OUTPUT;
        led.r = 0;
        led.g = 0;
        led.b = 255;

        ws2812_send(&led);
        
        
}

void fnRGBWhiteColor(void)
{
        printf("LED_WHITE\n\r");
        Dir_PIN = OUTPUT;
        led.r = 255;
        led.g = 255;
        led.b = 255;

        ws2812_send(&led);
        
        
}




void ws2812_send(ws2812_ptr* led) 
{
   // printf("Rgb_working_Fine");
    int j, n;
    long int val, temp;
    // the WS2812 wants bits in the order of:
    // GGGGGGGGRRRRRRRRBBBBBBBB
    // but I want to work in the opposite order. so i'm going to flip
    // the bits around and do some shifting so my order is
    // BBBBBBBBRRRRRRRRGGGGGGGG
    // with the most significant bit on the far right. so the RGB value
    // of 128 64 32, which normally would be:
    // R : 0b10000000
    // G : 0b01000000
    // B : 0b00100000
    // will become:
    // BBBBBBBBRRRRRRRRGGGGGGGG
    // 000001000000000100000010
    
   
    val = (bitflip(led->b) << 16) + (bitflip(led->r) << 8) + (bitflip(led->g)); 
    temp = val;
    for(n=0; n<=2; n++)
    {
        val = temp;

        // now begin shifting them over one at a time
        for(j = 0; j < 24; j++) 
        {
            // depending on if the currently viewed bit is 1 or 0
            // the pin will stay high for different times

            if (val & 1 == 1) 
            {
                // if it is a 1, let it stay higher a bit longer
                PIN = 1;
                Nop();
                Nop();
                Nop();
                PIN = 0;
            } else {
                // but a 0 should go high and then low as fast as possible
                PIN = 1;
                PIN = 0;
            }
            
            // and then right shift to get the next bit
            val = val >> (unsigned char)1;
        }
    }
}

// reverse the bits in a char

unsigned long int bitflip(unsigned char b) 
{
   b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
   b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
   b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
   return (unsigned long int)b;
}