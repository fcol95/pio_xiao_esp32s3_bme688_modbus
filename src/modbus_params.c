/*
 * SPDX-FileCopyrightText: 2016-2021 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
/*=====================================================================================
 * Description:
 *   C file to define parameter storage instances
 *====================================================================================*/
#include "modbus_params.h"

#include <math.h>

#include "freertos/semphr.h"

// Defines below are used to define register start address for each type of Modbus registers
#define INPUT_REG_PARAMS_FIELD_OFFSET(field) ((uint16_t)(offsetof(input_reg_params_t, field) >> 1))

#define MODBUS_PARAMS_MUTEX_TIMEOUT_MS 100U

// Here are the user defined instances for device parameters packed by 1 byte
// These are keep the values that can be accessed from Modbus master
#pragma pack(push, 1)
typedef struct
{
    float floats[MODBUS_PARAMS_INPUT_REGISTER_FLOAT_COUNT];
} input_reg_params_t;
#pragma pack(pop)

typedef struct
{
    QueueHandle_t floats[MODBUS_PARAMS_INPUT_REGISTER_FLOAT_COUNT];
} input_reg_params_mutexes_t;

input_reg_params_t input_reg_params = {0};
input_reg_params_mutexes_t input_reg_params_mutexes = {0};

// Set register values into known state
static esp_err_t setup_reg_data(void)
{
    // Define initial state of parameters
    for (uint8_t float_ind = 0; float_ind < MODBUS_PARAMS_INPUT_REGISTER_FLOAT_COUNT; float_ind++)
    {
        input_reg_params.floats[float_ind] = NAN;
        input_reg_params_mutexes.floats[float_ind] = xSemaphoreCreateMutex();
        if (input_reg_params_mutexes.floats[float_ind] == NULL)
            return ESP_FAIL;
    }
    return ESP_OK;
}

void *slave_handler_ctx = NULL;

esp_err_t init_modbus_params(void *slave_handler)
{
    if (slave_handler == NULL)
        return ESP_FAIL;

    esp_err_t ret = ESP_OK;

    slave_handler_ctx = slave_handler;

    ret = setup_reg_data();

    return ret;
}

esp_err_t get_input_register_float_reg_area(ModbusParams_InReg_Float_t index, mb_register_area_descriptor_t *const reg_area)
{
    if (index >= MODBUS_PARAMS_INPUT_REGISTER_FLOAT_COUNT)
        return ESP_FAIL;
    if (reg_area == NULL)
        return ESP_FAIL;

    reg_area->type = MB_PARAM_INPUT;
    reg_area->start_offset = (INPUT_REG_PARAMS_FIELD_OFFSET(floats) + index * (sizeof(float) << 2));
    reg_area->address = (void *)&input_reg_params.floats[index];
    reg_area->size = sizeof(float) << 2;
    // reg_area->access = MB_ACCESS_RO;

    return ESP_OK;
}

esp_err_t set_input_register_float(ModbusParams_InReg_Float_t index, float value)
{
    if (index >= MODBUS_PARAMS_INPUT_REGISTER_FLOAT_COUNT)
        return ESP_FAIL;

    esp_err_t ret = ESP_OK;
    ret = mbc_slave_lock(slave_handler_ctx);
    if (ret != ESP_OK)
        return ret;
    // TODO: Is mutex lock really needed if mbc_slave_lock works?
    if (xSemaphoreTake(input_reg_params_mutexes.floats[index], pdMS_TO_TICKS(MODBUS_PARAMS_MUTEX_TIMEOUT_MS)) != pdTRUE)
        return ESP_FAIL;
    input_reg_params.floats[index] = value;
    xSemaphoreGive(input_reg_params_mutexes.floats[index]);
    ret = mbc_slave_unlock(slave_handler_ctx);
    if (ret != ESP_OK)
        return ret;
    return ESP_OK;
}
