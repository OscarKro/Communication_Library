# Communication_Library
Small communication Library for use with HWLIB. It is mostly for my own use (or other students if need be).

## Author
Made by Oscar Kromhout, Gorinchem, The Netherlands

## Explanation
I've found that the standard I2C and SPI bus within the [Hardware Library](https://github.com/wovo/hwlib) from Wovo can be a bit confusing and will sometimes not work because of other manufacturer implementations of a bus. So I've decided to write my own low level busses to use. This way anyone can use a bus however they want.

## I2C explained
I've found that some manufactures of chips uphold their own way of implementing small functionalities of I2C. For example the BMP280 does not need a start/stop signal between writing and reading. But the OV7670 does. So read the datasheet of the chip you are trying to use to maintain the right order of using the functions in this library. The functions do work on every I2C device so you do not need to worry about that.

### writing with I2C
For most chips, the writing with the I2C bus functions is something like this:
writeStart() -> writeByte(slave adress) -> readAck() -> writeByte(register adress) -> readAck() -> writeByte(data) -> readAck() -> repeat writeByte(register Adress) etc or writeStop()

### reading with I2C
For most chips, the reading with the I2C bus functions is something like this:
writeStart() -> writeByte(slave adress) -> readAck() -> writeByte(register adress) -> readAck() -> writeStart() (and sometimes also writeStop()) -> writeByte(slave adress with read bit set to 1) -> readAck() -> readByte() -> repeat or writeStop()
But the protocol for reading with I2C can sometimes differ from manufacturer to manufacturer so be sure to read the datasheet.

#### use of some functions

- readAck() will give back a bool which is 1 when an ack was read, and 0 when an ack was not read (i2c is actually inverted).

- writeAck(bool) will need a 1 if you want to ack, and a 0 if you do not (i2c is actually inverted). 

- Calling writeByte() with a slave adress does not do anything with a read or write bit. So if the adress of your chip is 110110 (7 bits) the eight bit needs to be set to 1 if you want to read and you will have to add this yourself. So if you want to use that adress with the writeByte() to write you would have to do: writeByte(1101100). And if you want to use it to read you would have to do writeByte(1101101). This is done on purpose. I wanted to keep this library as low level as possible.

## SPI explained
Todo

