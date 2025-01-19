/*
 * SPDX-FileCopyrightText: 2016-2021 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*=====================================================================================
 * Description:
 *   The Modbus parameter structures used to define Modbus instances that
 *   can be addressed by Modbus protocol. Define these structures per your needs in
 *   your application. Below is just an example of possible parameters.
 *====================================================================================*/
#ifndef _DEVICE_PARAMS
#define _DEVICE_PARAMS

#include <stdint.h>

#include "esp_err.h"

#include "esp_modbus_slave.h"

// This file defines structure of modbus parameters which reflect correspond modbus address space
// for each modbus register type (coils, discreet inputs, holding registers, input registers)
// It also has proper access funtions to access those parameters.

typedef enum
{
    AMBIENT_TEMP_DEGC = 0,
    AMBIENT_HUMI_PCT,
    AMBIENT_PRESSURE_HPA,
    MODBUS_PARAMS_INPUT_REGISTER_FLOAT_COUNT,
} ModbusParams_InReg_Float_t;

esp_err_t init_modbus_params(void *slave_handler);

esp_err_t get_input_register_float_reg_area(ModbusParams_InReg_Float_t index, mb_register_area_descriptor_t *const reg_area);

esp_err_t set_input_register_float(ModbusParams_InReg_Float_t index, float value);

#endif // !defined(_DEVICE_PARAMS)
