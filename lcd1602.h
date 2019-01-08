// commands
#define	LCD1602_CLEARDISPLAY	0x01
#define	LCD1602_RETURNHOME	0x02
#define	LCD1602_ENTRYMODESET	0x04
#define	LCD1602_DISPLAYCONTROL	0x08
#define	LCD1602_CURSORSHIFT	0x10
#define	LCD1602_FUNCTIONSET	0x20
#define	LCD1602_SETCGRAMADDR	0x40
#define	LCD1602_SETDDRAMADDR	0x80

// flags for display entry mode
#define	LCD1602_ENTRYRIGHT	0x00
#define	LCD1602_ENTRYLEFT	0x02
#define	LCD1602_ENTRYSHIFTINCREMENT	0x01
#define	LCD1602_ENTRYSHIFTDECREMENT	0x00

// flags for display on/off control
#define	LCD1602_DISPLAYON	0x04
#define	LCD1602_DISPLAYOFF	0x00
#define	LCD1602_CURSORON	0x02
#define	LCD1602_CURSOROFF	0x00
#define	LCD1602_BLINKON	0x01
#define	LCD1602_BLINKOFF	0x00

// flags for display/cursor shift
#define	LCD1602_DISPLAYMOVE	0x08
#define	LCD1602_CURSORMOVE	0x00
#define	LCD1602_MOVERIGHT	0x04
#define	LCD1602_MOVELEFT	0x00

// flags for function set
#define	LCD1602_8BITMODE	0x10
#define	LCD1602_4BITMODE	0x00
#define	LCD1602_2LINE		0x08
#define	LCD1602_1LINE		0x00
#define	LCD1602_5x10DOTS	0x04
#define	LCD1602_5x8DOTS		0x00

// flags for backlight control
#define	LCD1602_BACKLIGHT	0x08
#define	LCD1602_NOBACKLIGHT	0x00

#define	En 			0x04	// Enable bit
#define	Rw			0x02	// Read/Write bit
#define	Rs			0x01	// Register select bit

extern void lcd1602init();
extern void lcd1602clear();
extern void lcd1602home();
extern void lcd1602setcursor(unsigned char col, unsigned char row);
extern void lcd1602displayon();
extern void lcd1602displayoff();
extern void lcd1602cursoron();
extern void lcd1602cursoroff();
extern void lcd1602write(unsigned char c);
