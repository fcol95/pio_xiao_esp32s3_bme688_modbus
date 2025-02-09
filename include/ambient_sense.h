#ifndef AMBIENT_SENSE__H__
#define AMBIENT_SENSE__H__

#include "esp_err.h"

esp_err_t ambient_sense_init(void);
void ambient_sense_task(void *pvParameter);

#endif // AMBIENT_SENSE__H__