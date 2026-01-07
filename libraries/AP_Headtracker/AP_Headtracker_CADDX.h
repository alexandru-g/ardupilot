#pragma once

#include "AP_Headtracker_Backend.h"
#include <AP_HAL/AP_HAL.h>

class AP_Headtracker_CADDX : public AP_Headtracker_Backend {
public:
    AP_Headtracker_CADDX(AP_Headtracker &frontend, AP_Headtracker_Params &params, uint8_t instance);

    void update() override;

private:
    AP_HAL::UARTDriver *port;
    
    // Parser State
    uint8_t window[10];
    uint8_t payload[6];

    void process_byte(uint8_t byte);
    void process_packet();
};
