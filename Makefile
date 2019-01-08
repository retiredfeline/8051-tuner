#
# By default SDCC generates for the small model which is what we want
# RAM is internal, ROM is from 0 to 2kB
#

CC=sdcc
CFLAGS=-mmcs51 -Ipt-1.4

default:	tuner.ibn

tuner.ihx:	tuner.rel lcd1602.rel i2c.rel
		$(CC) $(CFLAGS) -o $@ $^

tuner.rel:	tuner.c tables.h
		$(CC) $(CFLAGS) -c tuner.c

tables.h:	gentab.py
		./gentab.py > tables.h

%.asm:		%.c
		$(CC) $(CFLAGS) -S $<

%.rel:		%.c
		$(CC) $(CFLAGS) -c $<

%.ihx:		%.rel
		$(CC) $(CFLAGS) -o $@ $<

%.ibn:		%.ihx
		hex2bin -e ibn $<

clean:
		rm -f *.{asm,sym,lst,rel,rst,sym,lk,map,mem,ihx,ibn}
