#include "canlib_esp_twai.h"

#ifdef CONFIG_IDF_TARGET_ESP32

bool can_hal_TWAI_send(void* interface, void* base_interface, const canFrame_t* frame) {
    //  Get interface
    if(interface == NULL) {
        return false;
    }
    canInterfaceTWAI_t* twai = (canInterfaceTWAI_t*)interface;

    //  Convert message
    twai_message_t cFrame;
    can_hal_convertFrametoTWAI(*frame, &cFrame);

    //  Send frame
    return twai_transmit_v2(twai->twai_handle, &cFrame, 0) == ESP_OK;
}

bool can_hal_TWAI_receive(void* interface, void* base_interface, canFrame_t* frame) {
    //  Get interface
    if(interface == NULL) {
        return false;
    }
    canInterfaceTWAI_t* twai = (canInterfaceTWAI_t*)interface;

    //  Receive frame
    twai_message_t message;
    if (twai_receive_v2(twai->twai_handle, &message, 0) == ESP_OK) {
        can_hal_convertTWAItoFrame(message, frame);
        return true;
    }

    return false;
}

const canImplementation_t can_hal_TWAI = {
    .receive = can_hal_TWAI_receive,
    .transmit = can_hal_TWAI_send
};

esp_err_t can_hal_initTWAI(canInterfaceTWAI_t* can, const twai_timing_config_t* t_config, const twai_filter_config_t* f_config) {
    //  Call base initialization
    can_init_base(&can->base);

    //  Attach TWAI implementation to base interface
    can->base.implementation = (canImplementation_t*)&can_hal_TWAI;
    
    //  Prepare HAL configuration
    twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT(can->pinTx, can->pinRx, TWAI_MODE_NORMAL);

    //  Start interface
    esp_err_t install_res = twai_driver_install_v2(&g_config, t_config, f_config, &can->twai_handle);
    if(install_res != ESP_OK) {
        return install_res;
    }

    //  Start interface
    return twai_start_v2(can->twai_handle);
}

esp_err_t can_hal_deinitTWAI(canInterfaceTWAI_t* can) {
    //  Stop interface
    esp_err_t stop_res = twai_stop_v2(can->twai_handle);
    if(stop_res != ESP_OK) {
        return stop_res;
    }

    //  Uninstall driver
    esp_err_t uninstall_driver_res = twai_driver_uninstall_v2(can->twai_handle);
    if(uninstall_driver_res != ESP_OK) {
        return uninstall_driver_res;
    }

    //  Clear interface handles
    can->twai_handle = NULL;

    return ESP_OK;
}

void can_hal_convertTWAItoFrame(const twai_message_t frame, canFrame_t* ret_frame) {
    ret_frame->id = frame.identifier;
    ret_frame->len = frame.data_length_code;
    ret_frame->flags.isExtended = frame.extd;
    ret_frame->flags.isRemote = frame.rtr;
    ret_frame->flags.reserved = frame.reserved;
    

    memcpy(ret_frame->data.byte, frame.data, frame.data_length_code);
    return;
}

void can_hal_convertFrametoTWAI(const canFrame_t frame, twai_message_t* ret_frame) {
    ret_frame->identifier = frame.id;
    ret_frame->data_length_code = frame.len;
    ret_frame->extd = frame.flags.isExtended;
    ret_frame->rtr = frame.flags.isRemote;
    ret_frame->reserved = frame.flags.reserved;

    memcpy(ret_frame->data, frame.data.byte, frame.len);
    return;
}

#endif