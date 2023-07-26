#include "mbed.h"
#include "Hx711.h"

void Hx711::set_gain(uint8_t gain) {
    switch (gain) {
        case 128:       // channel A, gain factor 128
            gain_ = 1;
            break;
        case 64:        // channel A, gain factor 64
            gain_ = 3;
            break;
        case 32:        // channel B, gain factor 32
            gain_ = 2;
            break;
    }

    sck_.write(LOW);
    read();
}

uint32_t Hx711::readRaw() {
    // wait for the chip to become ready
    // TODO: this is not ideal; the programm will hang if the chip never
    // becomes ready...
    while (!is_ready());

    uint32_t value = 0;
    uint8_t data[3] = { 0 };
    uint8_t filler = 0x00;

    // pulse the clock pin 24 times to read the data
    data[2] = shiftInMsbFirst();
    data[1] = shiftInMsbFirst();
    data[0] = shiftInMsbFirst();

    // set the channel and the gain factor for the next reading using the clock pin
    for (unsigned int i = 0; i < gain_; i++) {
        sck_.write(HIGH);
        sck_.write(LOW);
    }

    // Datasheet indicates the value is returned as a two's complement value
    // Flip all the bits
    data[2] = ~data[2];
    data[1] = ~data[1];
    data[0] = ~data[0];

    // Replicate the most significant bit to pad out a 32-bit signed integer
    if ( data[2] & 0x80 ) {
        filler = 0xFF;
    } else if ((0x7F == data[2]) && (0xFF == data[1]) && (0xFF == data[0])) {
        filler = 0xFF;
    } else {
        filler = 0x00;
    }

    // Construct a 32-bit signed integer
    value = ( static_cast<uint32_t>(filler)  << 24
            | static_cast<uint32_t>(data[2]) << 16
            | static_cast<uint32_t>(data[1]) << 8
            | static_cast<uint32_t>(data[0]) );

    // ... and add 1
    return static_cast<int>(++value);
}


uint8_t Hx711::shiftInMsbFirst() {
    uint8_t value = 0;

    for (uint8_t i = 0; i < 8; ++i) {
        sck_.write(HIGH);
        value |= dt_.read() << (7 - i);
        sck_.write(LOW);
    }
    return value;
}
