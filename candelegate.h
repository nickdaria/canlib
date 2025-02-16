#pragma once

#include "canlib.h"

#define CANLIB_DELEGATE_BREAK_ON_MATCH false

typedef struct {
    canID_t            id_match;
    canFrameCallback_t callback;
} canCallback_t;

/**
 * Process one received frame (if available) and invoke callbacks accordingly.
 *
 * @param interface         Pointer to child struct (HAL-specific)
 * @param base              Pointer to base CAN interface struct
 * @param frame_rx          Pointer to the frame we got
 * @param cb_allFrames      Called for every received frame (optional, can be NULL)
 * @param cb_unhandled      Called if the frame doesn't match any entry in callback_table
 * @param callback_table    Array of callbacks, each with an ID mask/match
 * @param callback_len      Number of elements in callback_table
 * @param usrParameter      Pointer to a user-defined struct that can be used by the callback with application context
 *
 * @return True if a frame was processed, false if no frame was received
 */
bool can_delegateFrame(
    void*                  interface,
    canInterface_t*        base,
    const canFrame_t*      frame_rx,
    canFrameCallback_t     cb_allFrames,
    canFrameCallback_t     cb_unhandled,
    const canCallback_t*   callback_table,
    const size_t           callback_len,
    void*                  usrParameter
);