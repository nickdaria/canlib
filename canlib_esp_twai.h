#pragma once

#if defined(ESP_PLATFORM)

#include "canlib.h"
#include <driver/gpio.h>
#include <driver/twai.h>
#include <string.h>

typedef struct {
    canInterface_t base;
    twai_handle_t twai_handle;
    gpio_num_t pinTx;
    gpio_num_t pinRx;
} canInterfaceTWAI_t;

esp_err_t can_hal_initTWAI(canInterfaceTWAI_t* can, const twai_timing_config_t* t_config, const twai_filter_config_t* f_config);

esp_err_t can_hal_deinitTWAI(canInterfaceTWAI_t* can);

void can_hal_convertTWAItoFrame(const twai_message_t frame, canFrame_t* ret_frame);
void can_hal_convertFrametoTWAI(const canFrame_t frame, twai_message_t* ret_frame);

#endif