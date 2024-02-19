
// we want to isolate parts of the driver (see discord) that are relevant to the functions of our LTC 2990.

// Board purposes: Measure the various voltages and currents used by the avionics bay in our rocket.

// Redundant parts of our template driver:

// usable parts of our template driver:

#include "iostream"
#include "Arduino.h"
#include "Wire.h"
#include "LTC2990.hpp"
#include "cstdint"
#include "LT_I2C.h"
#include "Linduino.h"

// Reads a 14-bit adc_code from LTC2990.
int8_t i2c_read_word_data(uint8_t address, uint8_t address1, uint16_t *pInt);

int8_t LTC2990_adc_read(uint8_t i2c_address, uint8_t msb_register_address, int16_t *adc_code, int8_t *data_valid)
{
    int8_t ack = 0;
    uint16_t code;
    ack = i2c_read_word_data(i2c_address, msb_register_address, &code);

    *data_valid = (code >> 15) & 0x01;   // Place Data Valid Bit in *data_valid

    *adc_code = code & 0x7FFF;  // Removes data valid bit to return proper adc_code value

    return(ack);
}







