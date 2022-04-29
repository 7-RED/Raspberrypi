#include <wiringPi.h>

#include <stdio.h>

#include <sys/time.h>

#include <wiringPiI2C.h>

#include <string.h>

int LCDAddr = 0x27;

int BLEN = 1;

int ad;

void write_word(int data){

	int temp = data;

	if(BLEN== 1)

		temp |= 0x08;

	else

		temp &= 0xF7;

	wiringPiI2CWrite(ad,temp);

}

void send_command(int comm){

	int buf;

	buf= comm & 0xF0;

	buf|= 0x04;

	write_word(buf);

	delay(2);

	buf&= 0xFB;

	write_word(buf);

	buf= (comm & 0x0F) << 4;

	buf|= 0x04;

	write_word(buf);

	delay(2);

	buf&= 0xFB;

	write_word(buf);

}

void send_data(int data){

	int buf;

	buf= data & 0xF0;

	buf|= 0x05;

	write_word(buf);

	delay(2);

	buf&= 0xFB;

	write_word(buf);

	buf= (data & 0x0F) << 4;

	buf|= 0x05;

	write_word(buf);

	delay(2);

	buf&= 0xFB;

	write_word(buf);

}

void init(){

	send_command(0x33);

	delay(5);

	send_command(0x32);

	delay(5);

	send_command(0x28);

	delay(5);

	send_command(0x0C);

	delay(5);

	send_command(0x01);

	wiringPiI2CWrite(ad,0x08);

}

void clear(){

	send_command(0x01);

}

void write(int x,int y, char data[]){

	int addr,i;

	int tmp;

	if(x<0)
		x=0;

	if(x>15)
		x=15;

	if(y<0)
		y=0;

	if(y>1)
		y=1;

	addr= 0x80 + 0x40 * y +x;

	send_command(addr);

	tmp= strlen(data);

	for(i= 0; i < tmp; i++){

		send_data(data[i]);

	}

}

int main(void){

	ad= wiringPiI2CSetup(LCDAddr);

	init();

	write(0,0,"Ni ho");

	write(1,1,"rasp");

	delay(2000);

}
