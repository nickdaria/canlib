#include "canlib.h"

void can_init_base(canInterface_t *interface) {
    interface->rxCount = 0;
    interface->txCount = 0;
    interface->errorCount = 0;
    interface->implementation = NULL;
}

bool can_receive(void* interface, canInterface_t* base, canFrame_t *ret_frame) {
    if(interface == NULL || base->implementation == NULL || base->implementation->receive == NULL) {
        return false;
    }

    bool result = base->implementation->receive(interface, base, ret_frame);
    if(result) {
        base->rxCount++;
    }

    return result;
}

bool can_transmit(void* interface, canInterface_t* base, const canFrame_t frame) {
    if(interface == NULL || base->implementation == NULL || base->implementation->transmit == NULL) {
        return false;
    }

    bool result = base->implementation->transmit(interface, base, &frame);
    if(result) {
        base->txCount++;
    }

    return result;
}
