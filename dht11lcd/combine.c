#include <wiringPi.h>
#include <pcf8574.h>
#include <lcd.h>
#include <wiringPiI2C.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>

#include <stdint.h>

#define MAXTIMS	85

#define DHTPIN	7

int dht11_dat[5] = { 0, 0, 0, 0, 0 };
#define BASE 64
#define        RS                (BASE + 0)
#define        RW                (BASE + 1)
#define        E                (BASE + 2)
#define LED      (BASE + 3)

#define        DB4                (BASE + 4)
#define        DB5                (BASE + 5)
#define        DB6                (BASE + 6)
#define        DB7                (BASE + 7)
static int lcdHandle;//wiringPi LCD的控制要素

void read_dht11_dat()//dht11測試func

{

	uint8_t laststate	= HIGH;

	uint8_t counter		= 0;

	uint8_t j		= 0, i;

	float	f; 

 

	dht11_dat[0] = dht11_dat[1] = dht11_dat[2] = dht11_dat[3] = dht11_dat[4] = 0;

 

	pinMode( DHTPIN, OUTPUT );

	digitalWrite( DHTPIN, LOW );

	delay( 18 );

	digitalWrite( DHTPIN, HIGH );

	delayMicroseconds( 40 );

	pinMode( DHTPIN, INPUT );

 

	for ( i = 0; i < MAXTIMS; i++ )

	{

		counter = 0;

		while ( digitalRead( DHTPIN ) == laststate )

		{

			counter++;

			delayMicroseconds( 2 );

			if ( counter == 255 )

			{

				break;

			}

		}

		laststate = digitalRead( DHTPIN );

 

		if ( counter == 255 )

			break;

 

		if ( (i >= 4) && (i % 2 == 0) )

		{

			dht11_dat[j / 8] <<= 1;

			if ( counter > 16 )

				dht11_dat[j / 8] |= 1;

			j++;

		}

	}

 

	if ( (j >= 40) &&

	     (dht11_dat[4] == ( (dht11_dat[0] + dht11_dat[1] + dht11_dat[2] + dht11_dat[3]) & 0xFF) ) )

	{

		f = dht11_dat[2] * 9. / 5. + 32;
		printf( "Humidity = %d.%d %% Temperature = %d.%d C\n",

			dht11_dat[0], dht11_dat[1], dht11_dat[2], dht11_dat[3]);
		lcdPosition(lcdHandle,0,0);
		lcdPrintf(lcdHandle,"Humid: %d.%d %%",dht11_dat[0], dht11_dat[1]);
		lcdPosition(lcdHandle,0,1);
                lcdPrintf(lcdHandle,"Temp: %d.%d C",dht11_dat[2], dht11_dat[3]);
	}else  {

		printf( "Data not good, skip\n" );

	}

}


int main(void)
{   
    int i;
    int count;
    //wiringPi初始化
    wiringPiSetup();

    pcf8574Setup(BASE,0x27);//初始化LCD PCF8754 addr 0x27
   
    lcdHandle = lcdInit (2, 16, 4, RS, E, DB4,DB5,DB6,DB7, 0,0,0,0) ;
   
    if (lcdHandle < 0)
    {
        fprintf (stderr, "lcdInit failed\n") ;
        exit (1) ;
    }
   
    for(i=0;i<8;i++){
          pinMode(BASE+i,OUTPUT);  //把IO接口轉為output
	}
    digitalWrite(LED,1);         //把LED轉高電位把LCD打開
    digitalWrite(RW,0);          //將RW模式轉為低電位，設定為寫入
   
    printf ("Raspberry Pi - PCF8754 Test\n");
    lcdHome(lcdHandle);   //將LCD歸定位並清除
    lcdClear(lcdHandle);
    while(1){
	read_dht11_dat();//讀取DHT11資料
	delay(1000);
	}
    return 0;
}
