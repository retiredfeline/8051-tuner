#define	DATA	P1_7
#define	CLOCK	P1_6

extern void delay5us();
extern void startXfer();
extern void stopXfer();
extern unsigned char writeByte(unsigned char);
extern void setBrightness(unsigned char);
