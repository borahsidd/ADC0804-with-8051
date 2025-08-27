#include <reg51.h>
#define adc_data P2

///////////////////////////////Define variables
int x, digit0,digit1,digit2;
float y,adc_value;
float temp;

///////////////////////////////////Define pins for ADC0804
sbit rd=P1^5;
sbit wr=P1^6;
sbit intr=P1^7;
////////////////////////////////////Message

unsigned char mesg1[]="Temperature is=";

/////////////////////////////////// Function for creating delay in milliseconds.
void delay(unsigned int time)  
{
    unsigned i,j ;
    for(i=0;i<time;i++)    
    for(j=0;j<1275;j++);
}
///////////////////////////////////// Function to initialize serial communication
void initialize_serial()	
	{ 
	TMOD=0x20;  //use Timer 1, 8 bit ,auto reload
	TH1=0xFD;   //9600 baudrate
	SCON=0x50;
	TR1=1;     //start timer
	TI=1;
	}
////////////////////////////////////Function to transmit data_
	
void transmit(unsigned char cmd)
	{
	 while(TI==0);
	 SBUF=cmd;
	 TI=0;
	}
//////////////////////////////Function to initialize ADC
	
	void init_adc()
	{
		adc_data=0xFF;					/////////initialised adc pin as input	
		rd=0; 									////////initialised as output
    wr=0; 									/////////initialised as output
    intr=1; 								////////initialised as input							
	}
	
////////////////////////////////////Function to read ADC value
unsigned char read_adc()
  {    
		unsigned char value=0;
		rd=1;
    wr=0;     //////start conversion
		delay(7);
		wr=1; 	
		while(intr==1);  ////wait untill end of conversion
		rd=0;
		value=adc_data; 
		return value;								 
	}
							 
/////////////////////////////////////Convert ASCII to to hex function
	
void convert_ascii(float val)
 {
 unsigned int r,g;
 g=val;
 digit2=g/100;
 digit2=digit2|0x30;
 r=g%100;
 digit1=r/10;
 digit1=digit1|0x30;
 digit0=r%10;
 digit0=digit0|0x30;
 }
 
///////////////////////////////////*********************Main function*****************************//////////////////////////
 
void main()
{
	unsigned char x;
	initialize_serial();
	init_adc();
	while(1)
	{
		init_adc();
		initialize_serial();
		adc_value=0;			
		adc_value=read_adc(); 						/////Read adc value
		adc_value=(19.53*adc_value); 	/////Conversion of bit value to voltage (mV)
		adc_value=adc_value/10;						/////conversion formula-Voltage to temperature
		for(x=0;x<=15;x++)
		{
			transmit(mesg1[x]);
		}
		convert_ascii(adc_value);			///////conversion of ADC value to ASCII
		transmit(digit2); 
		transmit(digit1);
		transmit(digit0);
		transmit('C');
		transmit(0x0D);								///////To start new line 
		delay(100);
	}
}