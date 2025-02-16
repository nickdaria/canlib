#include "candelegate.h"

bool can_delegateFrame(
    void*                   interface,
    canInterface_t*         base,
    const canFrame_t*       frame_rx,
    canFrameCallback_t      cb_allFrames,
    canFrameCallback_t      cb_unhandled,
    const canCallback_t*    callback_table,
    const size_t            callback_len,
    void*                   usrParameter
    )
{
    //  Safety
    if(interface == NULL || base == NULL || frame_rx == NULL) {
        return false;
    }

    //  Call cb_allFrames if provided
    if (cb_allFrames != NULL) {
        cb_allFrames(interface, base, 0, frame_rx, usrParameter);
    }

    //  Check the callback_table for any matching entries
    bool handled = false;
    if (callback_table != NULL && callback_len > 0) {
        for (size_t i = 0; i < callback_len; i++) {
            const canCallback_t* entry = &callback_table[i];
            if(frame_rx->id == entry->id_match) {
                if (entry->callback != NULL) { entry->callback(interface, base, 0, frame_rx, usrParameter); }
                handled = true;

                #if CANLIB_DELEGATE_BREAK_ON_MATCH == true
                break;
                #endif
            }
        }
    }

    //  If not handled by any callback_table entry, call cb_unhandled
    if (!handled && cb_unhandled) {
        cb_unhandled(interface, base, 0, frame_rx, usrParameter);
    }

    return true;
}
