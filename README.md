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

## SPI explained
Todo

