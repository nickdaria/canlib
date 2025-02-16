# canlib
Extendable common CAN structures and patterns for C applications that utilize CAN 2.0 & CAN-FD

## Structure
canlib includes a set of types, structures, and functions that can be implemented for any CAN peripheral. Using these structures allows for your code to accept frames from any peripheral in a single format. 

### Functions
- `can_receive(...)` read a CAN message
- `can_transmit(...)` send a CAN message

### Types
- `canFrame_t` struct containing ID, data, length, and flags
- `canDataUnion_t` 64 byte union representing CAN/CAN-FD frame data
- `canFrameCallback_t` function callback signature for passing frames around a program

## Peripheral API
New types of peripherals simply need to write functions for common operations and define an implementation struct that points to these functions. This "map" tells the library how to handle common user functions.
- `canInterface_t` per-instance structure containing common information such as TX/RX/Error counts
- `canImplementation_t` per-peripheral type structure containing function pointers to code for common functions like transmit and receive

### Included Peripherals
- Espressif TWAI v2
    - Multiple interfaces

## CAN Delegate
A simple handler that allows you to direct CAN traffic to appropriate functions. Simply pass receieved frames into `can_delegateFrame(...)` and the appropriate callbacks will be run.
- Accepts a list of `canCallback_t` (ID & function)
- Accepts callback for all frames
- Accepts callback for unhandled frames (not found in callback list)