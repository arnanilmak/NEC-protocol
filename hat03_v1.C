//Project: hat03_v1.prj
// Device: MS84Fxx04
// Memory: Flash 4KX14b, EEPROM 256X8b, SRAM 256X8b
// Author: 
//Company: 
//Version:
//   Date: 
//===========================================================
//===========================================================
#include	"SYSCFG.h";
#include	"sensor.h";

#define time (TMR1H<<8 | TMR1L)
//===========================================================
//Variable definition
//===========================================================

int test123;
unsigned char firsttimeflag;
char ir_data,ir_data_cnt;
//===========================================================
//Funtion name：interrupt ISR
//parameters：无
//returned value：无
//===========================================================
void interrupt ISR(void)
{
    static char ir_frame_start_flag=0,cnt1=0,cnt2=0,buf[4];
    
    if(INTF)
	{
        TMR1ON=1;
		if(time<2300)
		{
            if(ir_frame_start_flag)
            {
				if(time<1250)//0 state
				{
					buf[cnt1]&=~(1<<(cnt2%8));
				}
				else   //1 state
				{
					buf[cnt1]|=1<<(cnt2%8);
				}
				cnt2++;
				if(cnt2==32)
                {               
                    ir_frame_start_flag=0;
                    TMR1ON=0;
                    if((buf[0]|buf[1])==0xFF)
                    {
                        ir_data=buf[2];
					}
                 }
				else if((cnt2%8)==0){cnt1++;}
            }
		}
		else if(time<11500)//repeat
		{
            TMR1ON=0;
		}
		else if(time<14000)//start
		{
            ir_data_cnt=0;
			ir_frame_start_flag=1;
			cnt1=0;
            cnt2=0;
		}
		INTF = 0;
		TMR1H=0;
        TMR1L=0;
	}
   
}

void init(void)
{
	OSCCON = 0B01110001;
    TRISA=0;//lc data out
    TRISB=0B1100;//RELAY OUT  FB RELAY IN
    TRISC=0x02;//lcd kathod and buzzer out
    PORTA=0;
    PORTB=0;
    PORTC=0;
    ANSEL=0;
	T1CON = 0B00110000;
    TMR1L=0;
    TMR1H=0;
    INTCON = 0B11010000;  //pc1 external interrupt
	INTEDG=0;
}
//===========================================================
//Funtion name：main
//parameters：无
//returned value：无
//===========================================================
main()
{
    
    init();
    while(1)
    {

		Display(ir_data,1);
		//start_measure();
	}
}
//===========================================================
