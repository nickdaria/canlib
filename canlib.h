#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/**
 * Simple typedef to make it clear this is only using 29 bits
 */
typedef uint32_t canID_t;

/**
 * Struct containing canFrame flags
 */
typedef struct {
    bool isExtended;
    bool isRemote;
    bool reserved;
} canFrameFlags_t;

/**
 * Convenient union for interacting with CAN frames easily
 */
typedef union {
    uint64_t u64[8];        // 64-bit unsigned integers
    int64_t  i64[8];        // 64-bit signed integers

    uint32_t u32[16];       // 32-bit unsigned integers
    int32_t  i32[16];       // 32-bit signed integers

    uint16_t u16[32];       // 16-bit unsigned integers
    int16_t  i16[32];       // 16-bit signed integers

    uint8_t  byte[64];      // Raw byte array access
    uint8_t  u8[64];        // 8-bit unsigned integers
    int8_t   i8[64];        // 8-bit signed integers
} canDataUnion_t;

/**
 * An instance of a CAN frame
 */
typedef struct {
    uint32_t id;
    canDataUnion_t data;
    size_t len;
    canFrameFlags_t flags;
} canFrame_t;

/**
 * Callback for any received message
 */
typedef void (*canFrameCallback_t)(
    void* interface,
    void* base_interface,
    const uint8_t result,
    const canFrame_t* frame,
    void* usrParameter
);

/**
 * HAL/Chip specific implementation references for executing send/receieve commands
 */
typedef struct {
    bool (*transmit)(void* interface, void* base_interface, const canFrame_t *frame);
    bool (*receive)(void* interface, void* base_interface, canFrame_t *frame);
} canImplementation_t;

/**
 * Base struct for all implementations
 */
typedef struct {
    uint64_t rxCount;
    uint64_t txCount;
    uint64_t errorCount;

    //canFrameCallback_t rxCallback;
    //canFrameCallback_t txCallback;

    canImplementation_t *implementation;
} canInterface_t;

/**
 * Initialize the base class, called by init function for HAL specific implementations
 */
void can_init_base(canInterface_t *interface);

/**
 * Call the receive function for this interface
 */
bool can_receive(void* interface, canInterface_t* base, canFrame_t *ret_frame);

/**
 * Call the transmit function for this interface
 */
bool can_transmit(void* interface, canInterface_t* base, const canFrame_t frame);