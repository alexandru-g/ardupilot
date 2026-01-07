#include "AP_Headtracker_Backend.h"

AP_Headtracker_Backend::AP_Headtracker_Backend(AP_Headtracker &frontend, AP_Headtracker_Params &params, uint8_t instance) :
    _frontend(frontend),
    _params(params),
    _instance(instance)
{
}
