#pragma once
#include <hwlib.hpp>
///@file
///\brief
//This file contains the declaration and definition of the SPI_BUS
///\author Oscar Kromhout
///\date 15-3-2020

///\brief
//The SPI_BUS class containing a basic SPI implementation
///\details
//This class contains several functions to make a basic SPI bus. Read the documentation of all functions to learn what everything does.
class SPI_BUS
{
private:
	hwlib::pin_out &sclk;
	hwlib::pin_out &csn;
	hwlib::pin_out &mosi;
	hwlib::pin_in &miso;

protected:
	///\brief
	/*Time to wait between setting the SCLK pin high and low*/
	int spiWaitTimeNs;

public:
	///\brief
	///The basic constructor that is needed to function properly
	///\details
	//This constructor needs to be used to build a valid SPI_BUS object
	///@param SCLK A hwlib pin_out object for the clock of the SPI bus
	///@param CSN A hwlib pin_out object for the slave select (CSN) of the SPI bus
	///@param MOSI A hwlib pin_out object for the master out, slave in of the SPI bus
	///@param MISO A hwlib pin_in object for the master in, slave out of the SPI bus
	///@param SPI_REST_TIME A uint_fast8_t number which is the time in nano seconds that the object should wait between setting the sclk pin high or low.
	//This can deviate for different chips. The default is set to 50 nano seconds, so this parameter is optional.
	SPI_BUS(hwlib::pin_out &SCLK, hwlib::pin_out &CSN, hwlib::pin_out &MOSI, hwlib::pin_in &MISO, int SPI_REST_TIME = 50) : sclk(SCLK), csn(CSN), mosi(MOSI), miso(MISO), spiWaitTimeNs(SPI_REST_TIME){};

	///\brief
	///Function to set the slave select pin high or low
	///\details
	//This function sets the slave select pin high or low and flushes the buffer
	///@param b A bool that sets the pin
	void setSS(bool b)
	{
		csn.write(b);
		csn.flush();
	}

	///\brief
	///Function to set the master out slave in pin high or low
	///\details
	//This function sets the MOSI pin high or low and flushes the buffer
	///@param b A bool that sets the pin
	void setMosi(bool b);
	{
		mosi.write(b);
		mosi.flush();
	}

	///\brief
	//Function to set the clock pin high or low
	///\details
	//This function sets the SCLK pin high or low and flushes the buffer
	///@param b A bool that sets the pin
	void setSCLK(bool b);
	{
		sclk.write(b);
		sclk.flush();
	}

	///\brief
	//Function that reads the master in, slave out pin
	///@return Returns a bool that is high when the MISO pin is high, else it returns low
	bool readMiso();
	{
		return miso.read();
	}

	///\brief
	//Function to set the SPI wait time
	///\details
	//This function sets the wait time for the spi data transmissions. In between all clock cycles in a spi bus is waiting time.
	//By using this function you are able to change that time to a minimum of 10 nano seconds. You can also change the clock time
	//in the constructor of the object. The default value for the constructor is 50 nano seconds.
	///@params newTimeInNs, a integer containing the new spi clock time you want to use.
	void setSpiClockTime(int newTimeInNs)
	{
		if (newTimeInNs >= 20)
		{
			spiWaitTimeNs = newTimeInNs;
		}
	}

	///\brief
	//Function to transmit a byte over the SPI bus
	///\details
	//This function transmits a full byte over the SPI bus using the sclk and mosi pin and utilizing bit banging. Take note that most SPI busses require
	//some time between setting the sclk pin high and low. This time can be set in the constructor of this object or by calling the setSpiClockTime. The default time is 50 nano seconds.
	//Also keep in mind that this function does NOT pull the SS pin high or low. This needs to be done manually because some SPI chips are SS active high
	//and most are active low. The function is virtual so can be overwritten with another implementation.
	///@param byte A uint8_t (unsigned char) containing the byte that is transmitted over the bus
	virtual void writeByte(uint8_t byte)
	{
		for (int i = 0; i < 8; i++)
		{
			bool tmp = byte & 128;
			byte <<= 1;
			setMosi(tmp);
			setSCLK(1);
			hwlib::wait_ns(spiWaitTimeNs);
			setSCLK(0);
			hwlib::wait_ns(spiWaitTimeNs);
		}
	}

	///\brief
	//Function to read a byte over the SPI bus
	///\details
	//This function reads a full byte using the MISO and SCLK pins and utilizing bit banging. It reads the most significant bit first because this is
	//the standard. Take note that most SPI busses require some time between setting the sclk pin high and low. This time can be set in the constructor
	// of this object or by calling the setSpiClockTime. The default time is 50 nano seconds. The function is virtual so can be overwritten with another implementation.
	///@return Returns a uint8_t (unsigned char) containing the byte that was written over the bus
	virtual uint8_t readByte()
	{
		uint8_t message = 0;
		for (int i = 0; i < 8; i++)
		{
			message <<= 1;
			setSCLK(1);
			hwlib::wait_ns(spiWaitTimeNs);
			message |= readMiso();
			setSCLK(0);
			hwlib::wait_ns(spiWaitTimeNs);
		}
		return message;
	}
};
