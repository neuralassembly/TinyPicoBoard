/*
 * File:   tinypicoboard.c
 * Author: kanamaru
 *
 * Created on June 24, 2016, 12:30 PM
 */
#include <xc.h>

#define _XTAL_FREQ 16000000

// Configuration 1
#pragma config FOSC     = INTOSC 
#pragma config WDTE     = OFF
#pragma config PWRTE    = ON
#pragma config MCLRE    = OFF
#pragma config CP       = OFF
#pragma config CPD      = OFF
#pragma config BOREN    = ON
#pragma config CLKOUTEN = OFF
#pragma config IESO     = OFF
#pragma config FCMEN    = OFF

// Configuration 2
#pragma config WRT    = OFF
#pragma config PLLEN  = OFF
#pragma config STVREN = ON
#pragma config BORV   = HI
#pragma config LVP    = OFF

#define SCRATCH_DATA_REQUEST 0x01
#define Firmware_Version 0x04
// Channels
#define ID_Firmware  15
#define ID_Channel_Resitance_D  0
#define ID_Channel_Resitance_C  1
#define ID_Channel_Resitance_B  2
#define ID_Channel_Button       3
#define ID_Channel_Resitance_A  4
#define ID_Channel_Light_sensor 5
#define ID_Channel_Sound_sensor 6
#define ID_Channel_Slider       7

unsigned int resistance_A_value = 0;
unsigned int resistance_B_value = 0;
unsigned int resistance_C_value = 0;
unsigned int resistance_D_value = 0;
unsigned int slider_value = 0;
unsigned int light_value = 0;
unsigned int sound_value = 0;
unsigned int button_value = 0;

unsigned int request = 0;
unsigned char buffer = 0;
char data_packet[2]="";

unsigned int adconv()
{
     unsigned int temp;

     GO_nDONE = 1 ;
     while(GO_nDONE) ;
     temp = ADRESH ;
     temp = ( temp << 8 ) | ADRESL ; 

     return temp ;
}

// Interruption when receiving data
void interrupt InterReceiver( void )
{
     if (RCIF == 1) {
          buffer = RCREG ;
          if(buffer == SCRATCH_DATA_REQUEST){
              request = 1;
          }
          RCIF = 0 ;
     }
}

void buildScratchPacket(char * packet, int channel, int value){
  char upper_data=(char)((value&(unsigned int)0x380)>>7);	//Get the upper 3 bits of the value
  char lower_data=(char)(value&0x7f);	//Get the lower 7 bits of the value
  *packet++=((1<<7)|(channel<<3)|(upper_data));
  *packet++=lower_data;
}

void sendScratchPacket(char * packet){
    while(TXIF==0) ;
    TXREG = packet[0];
    while(TXIF==0) ;
    TXREG = packet[1];
}

void main()
{
     OSCCON = 0b01111010 ;  // 16MHz
     ANSELA = 0b00000111 ;  // Analog input (RA2, RA1, RA0)
     TRISA  = 0b00001111 ;  // Analog input (RA2, RA1, RA0)

     ADCON1 = 0b10010000 ;  // Fosc/8
     ADCON0 = 0b00000001 ;
     __delay_us(10) ;

     RXDTSEL = 1 ;             // RA5 = RX
     TXCKSEL = 1 ;             // RA4 = TX
     TXSTA  = 0b00100100 ;     // Async, 8bit, no parity, BRGH=1
     RCSTA  = 0b10010000 ;
     SPBRG  = 25 ;             // 38400 bps
     RCIF = 0 ;
     RCIE = 1 ;                // Enabling UART interrupt for receiving data
     PEIE = 1 ;                // Enabling peripheral interrupt
     GIE  = 1 ;                // Enabling global interrupt
     
     PORTA  = 0b00000000 ;

     while(1) {
         if(request == 1){
             request = 0;
             // RA0
             ADCON0 = 0b00000001 ;
             __delay_us(10) ;
             resistance_A_value = adconv() ;

             // RA1
             ADCON0 = 0b00000101 ;
             __delay_us(10) ;
             resistance_B_value = adconv() ;

             // RA2
             ADCON0 = 0b00001001 ;
             __delay_us(10) ;
             resistance_C_value = adconv() ;      
             
             
             buildScratchPacket(data_packet, ID_Firmware , Firmware_Version);
             sendScratchPacket(data_packet);
            
             buildScratchPacket(data_packet, ID_Channel_Resitance_D, resistance_D_value);
             sendScratchPacket(data_packet);
                         
             buildScratchPacket(data_packet, ID_Channel_Resitance_C, resistance_C_value);
             sendScratchPacket(data_packet);
                      
             buildScratchPacket(data_packet, ID_Channel_Resitance_B, resistance_B_value);
             sendScratchPacket(data_packet);
                          
             buildScratchPacket(data_packet, ID_Channel_Button, button_value);
             sendScratchPacket(data_packet);
             
             buildScratchPacket(data_packet, ID_Channel_Resitance_A, resistance_A_value);
             sendScratchPacket(data_packet);
                                      
             buildScratchPacket(data_packet, ID_Channel_Light_sensor, light_value);
             sendScratchPacket(data_packet);
                           
             buildScratchPacket(data_packet, ID_Channel_Sound_sensor, sound_value);
             sendScratchPacket(data_packet);   
             
             buildScratchPacket(data_packet, ID_Channel_Slider, slider_value);
             sendScratchPacket(data_packet);    
         }

     }
}