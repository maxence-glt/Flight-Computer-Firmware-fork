
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

/* Reads a 14-bit adc_code from LTC2990.
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
*/

// Reads a 14-bit adc_code from the LTC2990 but enforces a maximum timeout.
// Similar to LTC2990_adc_read except it repeats until the data_valid bit is set, it fails to receive an I2C acknowledge, or the timeout (in milliseconds)
// expires. It keeps trying to read from the LTC2990 every millisecond until the data_valid bit is set (indicating new data since the previous
// time this register was read) or until it fails to receive an I2C acknowledge (indicating an error on the I2C bus).
int8_t LTC2990_adc_read_timeout(uint8_t i2c_address, uint8_t msb_register_address, int16_t *adc_code, int8_t *data_valid, uint16_t timeout, uint8_t status_bit)
{
    int8_t ack = 0;
    uint8_t reg_data;
    uint16_t timer_count;  // Timer count for data_valid
    *data_valid = 0;

    for (timer_count = 0; timer_count < timeout; timer_count++)
    {

        ack |=  LTC2990_register_read(i2c_address, LTC2990_STATUS_REG, &reg_data); //! 1)Read status register until correct data valid bit is set

        if ((ack) || (((reg_data>>status_bit)&0x1)==1))
        {
            break;
        }
        delay(1);
    }
    ack |= LTC2990_adc_read(i2c_address, msb_register_address, &(*adc_code), &(*data_valid));   //! 2) It's either valid or it's timed out, we read anyways
    if (*data_valid  !=1)
    {
        return (1);
    }
    return(ack);
}









