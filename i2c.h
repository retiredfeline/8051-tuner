#define	ADDR	0x27
#define	SDA	P1_7
#define	SCL	P1_6

extern void i2cinit();
extern void i2cstart();
extern void i2crestart();
extern void i2cstop();
extern void i2cack();
extern void i2cnakk();
extern unsigned char i2csendaddr();
extern unsigned char i2csend(unsigned char);
extern unsigned char i2cread();
