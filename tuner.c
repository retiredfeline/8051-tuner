//
// Generate 3 octaves of equal tempered tones
//

#include <8051.h>

#include "lcd1602.h"
#include "tables.h"

#include "pt.h"

#define	COUNT		4000			// 4 ms TICK
#define SCANDIV		(65536 - COUNT)
#define	TICK		4			// ms
#define	TICKSINHALFSEC	(500/TICK)
#define	DEPMIN		(100 / TICK)		// debounce period
#define	RPTTHRESH	((400 / TICK) + 1)	// repeat threshold after debounce
#define	RPTPERIOD	(250 / TICK)		// repeat period

// the dec and inc buttons are connected to port 3.4 and 3.5
#define	DECBUTTON	0x10
#define	INCBUTTON	0x20
#define	SWMASK		(DECBUTTON|INCBUTTON)

#define	LED		P1_4
#define	NOTEMIN		0
#define	NOTEMAX		(sizeof(divisors)/sizeof(divisors[0])-1)
#define	SOUND		P1_5

unsigned char swstate = SWMASK;
unsigned char swtent = SWMASK;
unsigned char swmin = DEPMIN;
unsigned char swrepeat = RPTTHRESH;
struct pt pt;
unsigned char textchanged = 1;
unsigned char noteindex = NOTEMIN;
unsigned int divisor = divisors[NOTEMIN];
__code char *description = descriptions[NOTEMIN];

#ifdef	BLINKPROG

// assume 12 MHz clock so timer runs at 1 MHz
void delay4ms() __naked
{
	TR0 = 0;	// turn off T0
	TH0 = SCANDIV >> 8;
	TL0 = SCANDIV & 0xff;
	TR0 = 1;	// start T0
	while (TF0 == 0)
		;	// wait for overflow
	TF0 = 0;	// clear overflow
	__asm__("ret");
}

void delayms(unsigned int ms)
{
	unsigned int	i = ms / 4;

	for ( ; i > 0; i--)
		delay4ms();
}

void main()
{
	TMOD = T0_M0;	// mode 1
	for (;;) {
		LED = 0;
		delayms(500);
		LED = 1;
		delayms(500);
	}
}

#else

void timer1_isr (void) __interrupt (3) __using(1)
{
	TR1 = 0;	// turn off T1
	TH1 = divisor >> 8;
	TL1 = divisor & 0xff;
	TR1 = 1;	// start T1 again
	SOUND ^= 1;
}

inline void delaystart(unsigned int count)
{
	TR0 = 0;	// turn off T0
	TH0 = count >> 8;
	TL0 = count & 0xff;
	TR0 = 1;	// start T0
}

inline void delaywait()
{
	while (TF0 == 0)
		;
	TF0 = 0;
}

static void setdisplay()
{
	divisor = divisors[noteindex];
	description = descriptions[noteindex];
	textchanged = 1;
}

static void switchaction()
{
	switch(~swstate & SWMASK) {
	case DECBUTTON:
		if (noteindex == 0)
			return;
		noteindex--;
		setdisplay();
		break;
	case INCBUTTON:
		if (noteindex == NOTEMAX)
			return;
		noteindex++;
		setdisplay();
		break;
	}
}

static inline void reinitstate()
{
	swmin = DEPMIN;
	swrepeat = RPTTHRESH;
	swtent = swstate;
}

static
PT_THREAD(switchhandler(struct pt *pt))
{
	PT_BEGIN(pt);
	PT_WAIT_UNTIL(pt, swstate != swtent);
	swtent = swstate;
	PT_WAIT_UNTIL(pt, --swmin <= 0 || swstate != swtent);
	if (swstate != swtent) {		// changed, restart
		reinitstate();
		PT_RESTART(pt);
	}
	switchaction();
	PT_WAIT_UNTIL(pt, --swrepeat <= 0 || swstate != swtent);
	if (swstate != swtent) {		// changed, restart
		reinitstate();
		PT_RESTART(pt);
	}
	switchaction();
	for (;;) {
		swrepeat = RPTPERIOD;
		PT_WAIT_UNTIL(pt, --swrepeat <= 0 || swstate == SWMASK);
		if (swstate == SWMASK) {	// released, restart
			reinitstate();
			PT_RESTART(pt);
		}
		switchaction();
	}
	PT_END(pt);
}

static void updatedisplay()
{
	__code char *p;
	int j;

	if (textchanged) {
		textchanged = 0;
		lcd1602setcursor(0, 0);
		for (p = description, j = 0; *p; p++, j++)
			lcd1602write(*p);
		for ( ; j < 16; j++)
			lcd1602write(' ');
	}
}

void main()
{
	unsigned char	ledcounter = TICKSINHALFSEC;

	TMOD = T0_M0|T1_M0;		// mode 1 on both timers
	TR0 = 0;			// turn off T0
	TH0 = SCANDIV >> 8;
	TL0 = SCANDIV & 0xff;
	TR1 = 0;			// turn off T1
	TH1 = divisor >> 8;
	TL1 = divisor & 0xff;
	ET1 = 1;			// enable timer 1 interrupt
	EA = 1;				// enable global interrupts
	TR1 = 1;			// start T1
	P1 = 0xff;
	P3 = 0xff;
	lcd1602init();
	lcd1602clear();
	for (;;) {
		delaystart(SCANDIV);
		updatedisplay();
		swstate = P3 & SWMASK;
		PT_SCHEDULE(switchhandler(&pt));
		if (ledcounter-- == 0) {
			ledcounter = TICKSINHALFSEC;
			LED ^= 1;
		}
		delaywait();
	}
}

#endif		// !BLINKPROG
