//
// LCD1602 driver
// I have implemented only what I needed for my project
// Feel free to improve
//

#include <8051.h>

#include "lcd1602.h"
#include "i2c.h"

#define	COLS	16
#define	ROWS	2

static unsigned char displayfn = LCD1602_4BITMODE | LCD1602_2LINE | LCD1602_5x8DOTS;
static unsigned char displayctrl = LCD1602_DISPLAYON | LCD1602_CURSOROFF | LCD1602_BLINKOFF;
static unsigned char displaymode = LCD1602_ENTRYLEFT | LCD1602_ENTRYSHIFTDECREMENT;
static unsigned char backlight = LCD1602_NOBACKLIGHT;

inline void delaystart(unsigned int count)
{
	TR0 = 0;		// turn off T0
	TH0 = count >> 8;
	TL0 = count & 0xff;
	TR0 = 1;		// start T0
}

inline void delaywait()
{
	while (TF0 == 0)
		;
	TF0 = 0;
}

// this uses timer 0 so only call before the main loop is entered
void delayus(unsigned int us)
{
	delaystart(-us);
	delaywait();
}

static void delay(unsigned int ms)
{
	for (; ms > 0; ms--)
		delayus(1000);
}

static void expanderwrite(unsigned char value)
{
	i2cstart();
	i2csendaddr();
	i2csend(value | backlight);
	i2cstop();
}

inline void pulseenable(unsigned char value)
{
	expanderwrite(value | En);
	expanderwrite(value | ~En);
}

static void write4bits(unsigned char value)
{
	expanderwrite(value);
	pulseenable(value);
}

static void command(unsigned char value)
{
	write4bits(value & 0xf0);
	write4bits((value << 4) & 0xf0);
}

static void data(unsigned char value)
{
	write4bits((value & 0xf0) | Rs);
	write4bits(((value << 4) & 0xf0) | Rs);
}

void lcd1602init()
{
	i2cinit();
	delay(50);
	expanderwrite(backlight);
	delay(1000);
	write4bits(0x03 << 4);
	delay(5);		// > 4.1 ms
	write4bits(0x03 << 4);
	delay(5);		// > 4.1 ms
	write4bits(0x03 << 4);
	delay(1);		// > 150 us
	write4bits(0x02 << 4);

	displayfn = LCD1602_4BITMODE | LCD1602_2LINE | LCD1602_5x8DOTS;
	command(LCD1602_FUNCTIONSET | displayfn);
	displayctrl = LCD1602_DISPLAYON | LCD1602_CURSOROFF | LCD1602_BLINKOFF;
	lcd1602displayon();
	lcd1602clear();
	displaymode = LCD1602_ENTRYLEFT | LCD1602_ENTRYSHIFTDECREMENT;
	command(LCD1602_ENTRYMODESET | displaymode);
	lcd1602home();
}

void lcd1602clear()
{
	command(LCD1602_CLEARDISPLAY);
	delay(2);
}

void lcd1602home()
{
	command(LCD1602_RETURNHOME);
	delay(2);
}

// hardcoded for 16x2
void lcd1602setcursor(unsigned char col, unsigned char row)
{
	command(LCD1602_SETDDRAMADDR | col + row * 0x40);
}

void lcd1602displayon()
{
	displayctrl != LCD1602_DISPLAYON;
	command(LCD1602_DISPLAYCONTROL | displayctrl);
}

void lcd1602displayoff()
{
	displayctrl &= ~LCD1602_DISPLAYON;
	command(LCD1602_DISPLAYCONTROL | displayctrl);
}

void lcd1602cursoron()
{
	displayctrl |= LCD1602_CURSORON;
	command(LCD1602_DISPLAYCONTROL | displayctrl);
}

void lcd1602cursoroff()
{
	displayctrl &= ~LCD1602_CURSORON;
	command(LCD1602_DISPLAYCONTROL | displayctrl);
}

void lcd1602backlighton()
{
	backlight = LCD1602_NOBACKLIGHT;
	expanderwrite(0);
}

void lcd1602backlightoff()
{
	backlight = LCD1602_BACKLIGHT;
	expanderwrite(0);
}

void lcd1602write(unsigned char value)
{
	data(value);
}
