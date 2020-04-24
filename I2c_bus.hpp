#pragma once
#include "./rtos/rtos.hpp"
#include "./hwlib/library/hwlib.hpp"

class i2c_bus{
private:
    hwlib::pin_oc &sda;
    hwlib::pin_oc &scl;
    uint8_t wait_time = 10;

public:
    i2c_bus(hwlib::pin_oc &sda, hwlib::pin_oc &scl): sda(sda), scl(scl) {
        scl.write(1);
        sda.write(0);
    };

    void writeStart() {
        scl.write(1);
        scl.flush();
        sda.write(0);
        sda.flush();
        hwlib::wait_ns(wait_time);
    }

    void writeStop() {
        scl.write(1);
        sda.write(1);
        scl.flush();
        sda.flush();
        hwlib::wait_ns(wait_time);
    }

    bool readAck() {
        scl.write(1);
        scl.flush();
        auto ack = sda.read();
        hwlib::wait_ns(wait_time);
        scl.write(0);
        scl.flush();
        return ack;
    }

    void writeAck(bool ack)
    {
        sda.write(ack);
        sda.flush();
        scl.write(1);
        scl.flush();
        hwlib::wait_ns(wait_time);
        scl.write(0);
        scl.flush();
        sda.write(0);
        sda.flush();
    }

    void writeByte(const uint8_t byte) {
        uint8_t tmpByte = byte;
        scl.write(0);
        scl.flush();
        hwlib::wait_ns(wait_time);
        for (int i = 0; i < 8; i++) {
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
        sda.write(0);
        sda.flush();
    }
    uint8_t readByte() {
        scl.write(0);
        scl.flush();
        uint8_t message = 0;
        for (int i = 0; i < 8; i++) {
            message <<= 1;
            scl.write(1);
            scl.flush();
            hwlib::wait_ns(wait_time);
            message |= sda.read();
            scl.write(0);
            scl.read();
            hwlib::wait_ns(wait_time);
        }
        return message;
    }
};
