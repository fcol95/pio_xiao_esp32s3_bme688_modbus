#ifndef MODBUS_PARAMS__H__
#define MODBUS_PARAMS__H__

#include <stdint.h>

#include "esp_err.h"

#include "esp_modbus_slave.h"

// This file defines structure of modbus parameters which reflect correspond
// modbus address space for each modbus register type (coils, discreet inputs,
// holding registers, input registers) It also has proper access funtions to
// access those parameters.

typedef enum
{
    AMBIENT_TEMP_DEGC = 0, //< Input Registers Address 1 - Length 2
    AMBIENT_HUMI_PCT,      //< Input Registers Address 3 - Length 2
    AMBIENT_PRESSURE_HPA,  //< Input Registers Address 5 - Length 2
    MODBUS_PARAMS_INPUT_REGISTER_FLOAT_COUNT,
} ModbusParams_InReg_Float_t;

typedef enum
{
    AMBIENT_SENSE_PERIOD_MS = 0, // Input Registers Address 1 - Length 1
    MODBUS_PARAMS_HOLDING_REGISTER_UINT_COUNT,
} ModbusParams_HoldReg_UInt_t;

esp_err_t modbus_params_init(void *slave_handler);

esp_err_t modbus_params_get_input_register_float_reg_area(
    ModbusParams_InReg_Float_t index,
    mb_register_area_descriptor_t *const reg_area);
esp_err_t modbus_params_get_holding_register_uint_reg_area(
    ModbusParams_HoldReg_UInt_t index,
    mb_register_area_descriptor_t *const reg_area);

esp_err_t
modbus_params_set_input_register_float(ModbusParams_InReg_Float_t index,
                                       float value);

esp_err_t
modbus_params_set_holding_register_uint(ModbusParams_HoldReg_UInt_t index,
                                        uint16_t value);
esp_err_t
modbus_params_get_holding_register_uint(ModbusParams_HoldReg_UInt_t index,
                                        uint16_t *const value);

#endif // MODBUS_PARAMS__H__
