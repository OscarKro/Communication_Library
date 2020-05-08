#pragma once
#include <hwlib.hpp>
///@file
///\brief
/*This file contains the declaration and definition (header only) of a I2C bus using the hwlib library.*/
///\author Oscar Kromhout
///\date 25-4-2020

///\brief
/*I2C_bus class*/
///\details
//This is a class used with hwlib to construct a very simple to use i2c bus. All the basic steps of an I2C transmission can be done with this class. Starting a transmission, writing
//a chip adress, writing a register adress, than reading from that adress and stop the transmission again for example.
class i2c_bus
{
private:
    hwlib::pin_oc &sda;
    hwlib::pin_oc &scl;

protected:
    uint8_t wait_time = 10;

public:
    ///\brief
    ///Standard constructor for the I2C class.
    ///\details
    /*This is the standard constructor that needs to be used in order to get a working object.*/
    ///@param sda, hwlib::pin_oc &, the sda pin (data pin).
    ///@param scl, hwlib::pin_oc &, the scl (clock) pin.
    i2c_bus(hwlib::pin_oc &sda, hwlib::pin_oc &scl) : sda(sda), scl(scl)
    {
        scl.write(1);
        sda.write(0);
    };

    ///\brief
    //Function to set the wait time between signals.
    ///\details
    //This function is used to set the wait time between certain parts of a transmission. For example the time between writing a start signal, and than writing a byte, or for example
    //between two bytes. The standard wait time is 10 ns.
    ///\param time, the time you want to wait in Ns (nano seconds).
    void setWaitTime(uint8_t time)
    {
        wait_time = time;
    }

    ///\brief
    //Function to write a i2c start signal.
    ///\details
    //This function is used to write a standard i2c start signal to the chip. It sets the clock high and the sda low, then waits for the set wait_time. This function always
    //needs to be called at the start of any transmission.
    virtual void writeStart()
    {
        sda.write(1);
        sda.flush();
        hwlib::wait_ns(wait_time);
        scl.write(1);
        scl.flush();
        hwlib::wait_ns(wait_time);
        sda.write(0);
        sda.flush();
        hwlib::wait_ns(wait_time);
        scl.write(0);
        scl.flush();
        hwlib::wait_ns(wait_time);
    }

    ///\brief
    //Function to write a i2c stop signal.
    ///\details
    //This function is used to write a standard i2c stop signal to the chip. It sets the clock and sda high and then waits for the set wait_time. This function always needs to be
    //called at the end of a i2c transmission.
    void writeStop()
    {
        sda.write(0);
        sda.flush();
        hwlib::wait_ns(wait_time);
        scl.write(1);
        scl.flush();
        hwlib::wait_ns(wait_time);
        sda.write(1);
        sda.flush();
        hwlib::wait_ns(wait_time);
    }

    ///\brief
    //Function to read the data pin for a ack.
    ///\details
    //This function needs to be used by the user at the end of a write transmission to read the 9th bit or ACK bit. If it is high, the data is received and this function returns true.
    ///@return Returns true if an acknowledgement was received.
    bool readAck()
    {
        scl.write(1);
        scl.flush();
        hwlib::wait_ns(wait_time);
        auto ack = !sda.read();
        scl.write(0);
        scl.flush();
        return ack;
    }

    ///\brief
    //Function to write an ack on the data pin
    ///\details
    //This function needs to be used after a read transmission by the user. If a byte has been read using the readByte() function the 9th bit can be send by using
    //this function.
    ///@param ack, bool, the acknowledgement (or not) you want to send, 1 for ack, 0 for no-ack
    void writeAck(bool ack)
    {
        sda.write(!ack);
        sda.flush();
        scl.write(1);
        scl.flush();
        hwlib::wait_ns(wait_time);
        scl.write(0);
        scl.flush();
    }

    ///\brief
    //Function to write a byte over the bus
    ///\details
    //This function is used to write a full byte over the bus. It does NOT start the transmission, read or writes acks, or ends the transmission. It just bit bangs the byte.
    //For most protocols the order of transmission to write a byte is as follows (not auto incremented):
    //write start -> write slave adress -> read ack -> write register adress -> read ack -> write the byte -> read ack -> write next adress, read ack and write byte
    //, read ack or write stop to stop the transmission.
    ///@param byte, const uint8_t, the byte you want to send over the bus.
    virtual void writeByte(const uint8_t byte)
    {
        uint8_t tmpByte = byte;
        hwlib::wait_ns(wait_time);
        for (int i = 0; i < 8; i++)
        {
            bool tmp = tmpByte & 128;
            tmpByte <<= 1;
            sda.write(tmp);
            sda.flush();
            scl.write(1);
            scl.flush();
            hwlib::wait_ns(wait_time);
            scl.write(0);
            scl.flush();
            hwlib::wait_ns(wait_time);
        }
    }

    ///\brief
    //Function to read a byte from the bus
    ///\details
    //This function is used to read a byte from the bus and return this byte. It does NOT start the transmission, read or writes acks, or ends the transmission, it just utilizes
    //bit banging in order to read a byte
    //Notice that for most I2C busses reading is done in this order:
    //start transmission -> Write slave adress -> read ack -> write register adress -> read ack -> repeat start (and sometimes stop as well) -> write slave adress + 1 -> read ack -> read byte -> write ack
    //for more data or write no ack and write stop transmission to stop.
    ///@return retursn a uint8_t containing the byte it read.
    virtual uint8_t readByte()
    {
        uint8_t message = 0;
        hwlib::wait_ns(wait_time);
        for (int i = 0; i < 8; i++)
        {
            message <<= 1;
            scl.write(1);
            scl.flush();
            hwlib::wait_ns(wait_time);
            message |= sda.read();
            scl.write(0);
            scl.flush();
            hwlib::wait_ns(wait_time);
        }
        return message;
    }
};