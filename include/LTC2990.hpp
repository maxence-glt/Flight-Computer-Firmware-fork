#ifndef LTC2990_h
#define LTC2990_h
#include <inttypes.h>

/** 
 * Uncomment to enable serial debugging output. Serial starts on LTC2990 begin method with 9600 speed
 * */
//#define LTC2990_DEBUG

/** 
 * Uncomment to enable low precision mode which ignores lower byte. For voltage this drops precision to 0.077V steps
 * */
//#define LTC2990_LOW_PRECISION

#define LTC2990_MEASURE_MODE_V1_V2_TR2 0x0
#define LTC2990_MEASURE_MODE_V1_MINUS_V2_TR2 0x1
#define LTC2990_MEASURE_MODE_V1_MINUS_V2_V3_V4 0x2
#define LTC2990_MEASURE_MODE_TR1_V3_V4 0x3
#define LTC2990_MEASURE_MODE_TR1_V3_MINUS_V4 0x4
#define LTC2990_MEASURE_MODE_TR1_TR2 0x5
#define LTC2990_MEASURE_MODE_V1_MINUS_V2_V3_MINUS_V4 0x6
#define LTC2990_MEASURE_MODE_V1_V2_V3_V4 0x7

#define LTC2990_MEASURE_ONLY_INT 0x0
#define LTC2990_MEASURE_ONLY_V1_TR1 0x1
#define LTC2990_MEASURE_ONLY_V3_TR2 0x2
#define LTC2990_MEASURE_ALL 0x3

#define LTC2990_REPEAT_BIT 0x6
#define LTC2990_TEMPFORMAT_BIT 0x7

#define LTC2990_STATUS_BUSY_BIT 0x0
#define LTC2990_STATUS_READY_TINT_BIT 0x1
#define LTC2990_STATUS_READY_V1_TR1_BIT 0x2
#define LTC2990_STATUS_READY_V2_BIT 0x3
#define LTC2990_STATUS_READY_V3_TR2_BIT 0x4
#define LTC2990_STATUS_READY_V4_BIT 0x5
#define LTC2990_STATUS_READY_VCC_BIT 0x6
#define LTC2990_STATUS_RESERVED_BIT 0x7

#define LTC2990_REGISTER_STATUS 0x0
#define LTC2990_REGISTER_CONTROL 0x1
#define LTC2990_REGISTER_TRIGGER 0x2
#define LTC2990_REGISTER_NA 0x3
#define LTC2990_REGISTER_TINT_M 0x4
#define LTC2990_REGISTER_TINT_L 0x5
#define LTC2990_REGISTER_V1_M 0x6
#define LTC2990_REGISTER_V1_L 0x7
#define LTC2990_REGISTER_V2_M 0x8
#define LTC2990_REGISTER_V2_L 0x9
#define LTC2990_REGISTER_V3_M 0xA
#define LTC2990_REGISTER_V3_L 0xB
#define LTC2990_REGISTER_V4_M 0xC
#define LTC2990_REGISTER_V4_L 0xD
#define LTC2990_REGISTER_VCC_M 0xE
#define LTC2990_REGISTER_VCC_L 0xF

#define LTC2990_BASE_ADDRESS 0x98
#define LTC2990_GLOBAL_SYNC_ADDRESS 0xEE

#define LTC2990_VOLTAGE_SINGLE_END_COEF 305.180
#define LTC2990_VOLTAGE_DIFFERENTIAL_COEF 19.420
#define LTC2990_TEMP_COEF 0.0625

#define LTC2990_SUCCESS 0
#define LTC2990_ERROR_WRITE 1
#define LTC2990_ERROR_READ 2
#define LTC2990_ERROR_NEGATIVE 3
#define LTC2990_ERROR_NOT_READY 4

#include <inttypes.h>

class LTC2990 {
public:

    LTC2990();
    LTC2990(uint8_t relative_address);
    LTC2990(uint8_t relative_address, uint8_t tempFormatIsCelsius, uint8_t single, uint8_t limit, uint8_t measureMode);

    void begin();

    uint8_t setMode(uint8_t tempFormatIsCelsius, uint8_t single, uint8_t limit, uint8_t measureMode);
    uint32_t readV1();
    uint32_t readV2();
    uint32_t readV3();
    uint32_t readV4();
    uint32_t readVCC();
    uint32_t readV1V2();
    uint32_t readV3V4();
    uint32_t readTR1();
    uint32_t readTR2();
    uint32_t readTINT();
    uint32_t getLastErrorCode();
    bool initialized();

    void triggerConvertion();

    uint8_t readRegister(uint8_t reg);

private:

    void config(uint8_t relative_address, uint8_t tempFormatIsCelsius, uint8_t single, uint8_t limit, uint8_t measureMode);


    uint8_t writeRegister(uint8_t reg, uint8_t value);

    uint32_t readVoltage(uint8_t reg);
    uint32_t readVoltageDifferential(uint8_t reg);
    uint32_t readTemperature(uint8_t reg);

    uint8_t _addr;
    uint8_t _mode;
    uint8_t _shadow;
    uint8_t _initialised;
    uint8_t _lastErrorCode;
};

#endif