# 8051 tuner

This is the firmware for a [3 octave tone generator project](https://hackaday.io/project/162267-8051-tuner) published at Hackaday.

It drives a 2 line by 16 character LCD display with an I2C interface that can be purchased in eBay for a few dollars.

The LCD1602 high level driver and the I2C low level driver are not complete implementations, I only implemented enough for this application.

For LCD1602 some parameters are hard coded for this size display and many of the LCD commands are not implemented. These are not hard to add.

The I2C driver doesn't do clock stretching and other behaviour required by the standard.

Feel free to improve on the code if you reuse it.

It uses the [Protothreads library](http://dunkels.com/adam/pt/) by Adam Dunkels. You should adjust the -I option to the sdcc compiler to where the include files are installed.

tm1637.c is an unrelated program I used to test my 8051 MCU after breadboard assembly.

## Versioning

First release January 2019

## Authors

* **Ken Yap**

## License

See the [LICENSE](LICENSE.md) file for license rights and limitations (MIT).
