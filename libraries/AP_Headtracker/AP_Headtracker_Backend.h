#pragma once

#include "AP_Headtracker.h"

class AP_Headtracker_Backend {
public:
    AP_Headtracker_Backend(AP_Headtracker &frontend, AP_Headtracker_Params &params, uint8_t instance);

    // update - read from headtrackers
    virtual void update() = 0;

protected:
    AP_Headtracker &_frontend;
    AP_Headtracker_Params &_params;
    uint8_t _instance;
};
