#!/usr/bin/env python3

# If MCU is clocked at 12 MHz then the counter input is 1 MHz
# and the divisor required for each note is:
#
# divisor = 500000 / frequency
#
# half because we need to flip the bit twice per period

CLOCK = 500000


def main():
    octave = []
    for note in range(0, 12):
        octave.append(2.0 ** (note / 12.0))
    notes = ["A", "A#/Bb", "B", "C", "C#/Db", "D",
             "D#/Eb", "E", "F", "F#/Gb", "G", "G#/Ab", ]
    freq = []
    divisor = []
    errppm = []
    desc = []
    for base in [220, 440, 880]:
        for note in range(0, 12):
            f = base * octave[note]
            if f < 130:
                onum = 2
            if f < 261:
                onum = 3
            elif f < 523:
                onum = 4
            elif f < 1046:
                onum = 5
            else:
                onum = 6
            d = CLOCK / f
            intdiv = round(d)
            e = abs(1 - (d/intdiv)) * 1000000.0
            freq.append(f)
            divisor.append(intdiv)
            errppm.append(e)
            desc.append("%0.1f %s%d" % (f, notes[note], onum))
    print("__code unsigned const short divisors[] = {")
    for i, intdiv in enumerate(divisor):
        print("\t%d,\t// d=%d, f=%0.1f, e=%0.1f ppm" %
              (65536 - intdiv, intdiv, freq[i], errppm[i]))
    print("};\n")
    print("__code const char *const descriptions[] = {")
    for i, intdiv in enumerate(divisor):
        print("\t\"%s\"," % (desc[i]))
    print("};\n")


if __name__ == '__main__':
    main()
