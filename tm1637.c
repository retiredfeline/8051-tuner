//
// Routines for driving TM1637 display
//

#include <8051.h>

#include "tm1637.h"

// call and ret take 2 us each, add a nop for 5 us
// change if crystal is > 12 MHz
void delay5us() __naked
{
	__asm__("nop");
	__asm__("ret");
}

void startXfer() __naked
{
	CLOCK = 1;
	DATA = 1;
	delay5us();
	DATA = 0;
	CLOCK = 0;
	delay5us();
	__asm__("ret");
}

void stopXfer() __naked
{
	CLOCK = 0;
	DATA = 0;
	delay5us();
	CLOCK = 1;
	DATA = 1;
	delay5us();
	__asm__("ret");
}

unsigned char writeByte(unsigned char value)
{
	unsigned char i;

	for (i = 0; i < 8; i++) {
		CLOCK = 0;
		delay5us();
		if (value & 0x01)
			DATA = 1;
		else
			DATA = 0;
		delay5us();
		CLOCK = 1;
		delay5us();
		value >>= 1;
	}
	CLOCK = 0;
	delay5us();
	DATA = 1;
	CLOCK = 1;
	delay5us();
	return DATA;
}

void setBrightness(unsigned char val)
{
	startXfer();
	writeByte(val);
	stopXfer();
}

#ifdef	MAIN
// assume 12 MHz clock so timer runs at 1 MHz

#define	COUNT	4000
#define	TVAL	(65536-COUNT)

void delay4ms() __naked
{
	TR0 = 0;
	// let's see how good SDCC is at optimising this
	TH0 = TVAL >> 8;
	TL0 = TVAL & 0xFF;
	// pretty good according to generated assembler, just a spurious warning
	TR0 = 1;	// turn on T0
	while (TF0 == 0)
		;	// wait for overflow
	TF0 = 0;	// clear overflow
	__asm__("ret");
}

void delay(unsigned int ms)
{
	unsigned int	i = ms / 4;

	for ( ; i > 0; i--)
		delay4ms();
}

void writedigits(unsigned char colon)
{
	startXfer();
	writeByte(0x40);
	stopXfer();
	startXfer();
	writeByte(0xc0);
	writeByte(0x06);	// 1
	writeByte(0x5b|colon);	// 2:
	writeByte(0x4f);	// 3
	writeByte(0x66);	// 4
	stopXfer();
}

// 12:34 with blinking colon at 1 Hz
void main()
{
	TMOD = 0x1;
	setBrightness(0x8f);
	for (;;) {
		writedigits(0);
		delay(500);
		writedigits(0x80);
		delay(500);
	}
}
#endif
