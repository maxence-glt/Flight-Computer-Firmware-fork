
// we want to isolate parts of the driver (see discord) that are relevant to the functions of our LTC 2990.

// Board purposes: Measure the various voltages and currents used by the avionics bay in our rocket.

// Redundant parts of our template driver:

// usable parts of our template driver:

#include "iostream"
#include "Arduino.h"
#include "Wire.h"
#include "LTC2990.hpp"
#include "cstdint"

bool _initialized {false};
uint8_t _addr;
uint8_t _mode;
uint32_t _lastErrorCode;



uint8_t LTC2990_setMode(uint8_t tempFormatIsCelsius = 0, uint8_t single = 0, uint8_t limit = LTC2990_MEASURE_ALL, uint8_t measureMode = LTC2990_MEASURE_MODE_V1_V2_V3_V4) {
    return measureMode + (limit<<3) + (single<<LTC2990_REPEAT_BIT) + (tempFormatIsCelsius<<LTC2990_TEMPFORMAT_BIT);
}

void LTC2990_config(uint8_t address, uint8_t tempFormatIsCelsius, uint8_t single, uint8_t limit, uint8_t measureMode) {
    _initialized = false;
    _addr = address;
    _mode = LTC2990_setMode(tempFormatIsCelsius, single, limit, measureMode);
}

void LTC2990_begin() {
    Wire.begin();
    _initialized = true;

    // add error block that checks if Init failed, set _initialized to false
}

uint8_t LTC2990_writeRegister(uint8_t r, uint8_t val) {
    noInterrupts();
    Wire.beginTransmission(_addr);
    Wire.write(r);
    Wire.write(val);
    uint8_t status = Wire.endTransmission(true);
    if (status != 0) {
        _lastErrorCode = LTC2990_ERROR_WRITE;
    } else {
        _lastErrorCode = LTC2990_SUCCESS;
    }
    delayMicroseconds(200); //TODO: remove / work around
    interrupts();
#ifdef LTC2990_DEBUG
    Serial.println(status);
#endif
    return status;
}

uint8_t LTC2990_readRegister(uint8_t r) {
    noInterrupts();
    Wire.beginTransmission(_addr);
    Wire.write(r);
    uint8_t status = Wire.endTransmission(true);
    delayMicroseconds(200);
    if(status == 0) {
        if (Wire.requestFrom((int)_addr, 1, 1) == 1) {
            uint8_t result = Wire.read();
            interrupts();
            _lastErrorCode = LTC2990_SUCCESS;
            return result;
        } else {
            _lastErrorCode = LTC2990_ERROR_READ;
        }
    } else {
        _lastErrorCode = LTC2990_ERROR_WRITE;
    }
    interrupts();
    return 0;
}

uint32_t LTC2990_getLastErrorCode() {
    return _lastErrorCode;
}

uint32_t LTC2990_readVoltage(uint8_t m_register, bool differential_toggle) {
    uint8_t m = LTC2990_readRegister(m_register);
    uint8_t l = 0;
    uint8_t m_error = _lastErrorCode;
    uint8_t l_error = LTC2990_SUCCESS;

#ifndef LTC2990_LOW_PRECISION
    l = LTC2990_readRegister(m_register + 1);
    l_error = _lastErrorCode;
#endif

    if((m&0x80) == 0 || m == 0) {
        if(m!=0) {
            _lastErrorCode = LTC2990_ERROR_NOT_READY; //No new value is available at this point
        }
        return 0;
    }
    if(l_error != LTC2990_SUCCESS) {
        _lastErrorCode = l_error;
        return 0;
    }
    if((m&0x40) == 0x40) {
        _lastErrorCode = LTC2990_ERROR_NEGATIVE;
        return 0; //negative
    }
    m &= 0x3F;
    _lastErrorCode = LTC2990_SUCCESS;
    if (differential_toggle)
        return ((m<<8)+l) / 1000.0 * LTC2990_VOLTAGE_DIFFERENTIAL_COEF;
    return ((m<<8)+l) / 1000.0 * LTC2990_VOLTAGE_SINGLE_END_COEF;
}

uint32_t LTC2990_readTemperature(uint8_t m_register) {
    uint8_t m = LTC2990_readRegister(m_register);
    uint8_t l = 0;
    uint8_t m_error = _lastErrorCode;
    uint8_t l_error = LTC2990_SUCCESS;

#ifndef LTC2990_LOW_PRECISION
    l = LTC2990_readRegister(m_register + 1);
    l_error = _lastErrorCode;
#endif

    if((m&0x80) == 0 || m == 0) {
        if(m!=0) {
            _lastErrorCode = LTC2990_ERROR_NOT_READY; //No new value is available at this point
        }
        return 0;
    }
    if(l_error != LTC2990_SUCCESS) {
        _lastErrorCode = l_error;
        return 0;
    }
    m &= 0x1F;
    _lastErrorCode = LTC2990_SUCCESS;
    return ((m<<8) + l) * LTC2990_TEMP_COEF;
}

uint32_t LTC2990_readV1() {
    return LTC2990_readVoltage(LTC2990_REGISTER_V1_M, false);
}

uint32_t LTC2990_readV2() {
    return LTC2990_readVoltage(LTC2990_REGISTER_V2_M, false);
}

uint32_t LTC2990_readV3() {
    return LTC2990_readVoltage(LTC2990_REGISTER_V3_M, false);
}

uint32_t LTC2990_readV4() {
    return LTC2990_readVoltage(LTC2990_REGISTER_V4_M, false);
}

uint32_t LTC2990_readVCC() {
    uint32_t v = LTC2990_readVoltage(LTC2990_REGISTER_VCC_M, false);
    if(v!=0) {
        return v+2500;
    }
    return 0;
}

uint32_t LTC2990_readV1V2() {
    return LTC2990_readVoltage(LTC2990_REGISTER_V1_M, true);
}

uint32_t LTC2990_readV3V4() {
    return LTC2990_readVoltage(LTC2990_REGISTER_V3_M, true);
}

uint32_t LTC2990::readTR1() {
    return readTemperature(LTC2990_REGISTER_V1_M);
}
uint32_t LTC2990::readTR2() {
    return readTemperature(LTC2990_REGISTER_V3_M);
}
uint32_t LTC2990::readTINT() {
    return readTemperature(LTC2990_REGISTER_TINT_M);
}