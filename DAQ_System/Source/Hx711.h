#ifndef _HX711_H_
#define _HX711_H_
#include "mbed.h"
/**
 * Class for communication with the HX711 24-Bit Analog-to-Digital 
 * Converter (ADC) for Weigh Scales by AVIA Semiconductor.
 * This library is a port of the Arduino library at 
 * https://github.com/bogde/HX711
 * It works with the FRDM K22F.
 */
class Hx711 {

public:

    /**
     * Create an Hx711 ADC object
     * @param pin_sck PinName of the clock pin (digital output)
     * @param pin_dt PinName of the data pin (digital input)
     * @param offset offset for sensor values
     * @param scale scale factor to obtain real values
     * @param gain channel selection is made by passing the appropriate gain: 
     *      128 or 64 for channel A, 32 for channel B
     */
    Hx711(PinName pin_sck, PinName pin_dt, int offset, float scale, uint8_t gain = 128) :
        sck_(pin_sck),
        dt_(pin_dt) {
        set_offset(offset);
        set_scale(scale);
        set_gain(gain);
    }
    
    /**
     * Create an Hx711 ADC object with zero offset and unit scaling
     * @param pin_sck PinName of the clock pin (digital output)
     * @param pin_dt PinName of the data pin (digital input)
     * @param gain channel selection is made by passing the appropriate gain: 
     *      128 or 64 for channel A, 32 for channel B
     * TODO: constructor overloading is not allowed?
     */
    Hx711(PinName pin_sck, PinName pin_dt, uint8_t gain = 128) :
        sck_(pin_sck),
        dt_(pin_dt) {
        set_offset(0);
        set_scale(1.0f);
        set_gain(gain);
    }

    /**
     * Check if the sensor is ready
     * from the datasheet: When output data is not ready for retrieval, 
     * digital output pin DOUT is high. Serial clock input PD_SCK should be low. 
     * When DOUT goes to low, it indicates data is ready for retrieval.
     * @return true if dt_.read() == LOW
     * TODO: this is not ideal; the programm will hang if the chip never
     * becomes ready...
     */
    bool is_ready() {
        return dt_.read() == LOW;
    }
    
    /**
     * Waits for the chip to be ready and returns a raw int reading
     * @return int sensor output value
     */
    uint32_t readRaw();
    
    /**
     * Obtain offset and scaled sensor output; i.e. a real value
     * @return float
     */
    float read() {
        return convert_to_real(readRaw());
    }
    
    /**
     * Convert integer value from chip to offset and scaled real value
     * @param val integer value
     * @return (val - get_offset()) * get_scale()
     */
    float convert_to_real(int val) {
        return ((float)(val - get_offset())) * get_scale();
    }
    
    /**
     * Puts the chip into power down mode
     */
    void power_down() {
        sck_.write(LOW);
        sck_.write(HIGH);
    }

    /**
     * Wakes up the chip after power down mode
     */
    void power_up() {
        sck_.write(LOW);
    }

    /**
     * Set the gain factor; takes effect only after a call to read()
     * channel A can be set for a 128 or 64 gain; channel B has a fixed 32 gain
     * depending on the parameter, the channel is also set to either A or B
     * Ensures that gain_ = 128, 64 or 32
     * @param gain 128, 64 or 32
     */
    void set_gain(uint8_t gain = 128);
    
    /**
     * Obtain current gain
     * @return gain_
     */
    uint8_t get_gain() {
        return gain_;
    }

    /**
     * Set the scale factor
     * @param scale desired scale
     */
    void set_scale(float scale = 1.0f) { 
        scale_ = scale; 
    };

    /**
     * Get sensor scale factor
     * @return scale_
     */
    float get_scale() { 
        return scale_; 
    }

    /**
     * Set the sensor offset
     * @param offset the desired offset
     */
    void set_offset(int offset = 0) { 
        offset_ = offset; 
    }

    /**
     * Get current sensor offset
     * @return offset_
     */
    int get_offset() { return offset_; }
    

private:

    static const uint8_t LOW      = 0; // digital low
    static const uint8_t HIGH     = 1; // digital high

    DigitalOut sck_;    // clock line
    DigitalIn dt_;      // data line 

    uint8_t gain_;      // amplification factor at chip
    int offset_;        // offset chip value
    float scale_;       // scale output after offset

    /**
     * Port of the Arduino shiftIn function; shifts a byte one bit at a time
     * @return incoming but
     */
    uint8_t shiftInMsbFirst();
};

#endif