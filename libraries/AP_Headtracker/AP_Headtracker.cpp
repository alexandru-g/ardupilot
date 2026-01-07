#include "AP_Headtracker.h"
#include "AP_Headtracker_CADDX.h"
#include <RC_Channel/RC_Channel.h>

#if AP_HEADTRACKER_ENABLED

extern const AP_HAL::HAL& hal;

AP_Headtracker *AP_Headtracker::_singleton;

const AP_Param::GroupInfo AP_Headtracker::var_info[] = {

    // @Group: 1
    // @Path: AP_Headtracker_Params.cpp
    AP_SUBGROUPINFO(_params[0], "1", 1, AP_Headtracker, AP_Headtracker_Params),

#if AP_HEADTRACKER_MAX_INSTANCES > 1
    // @Group: 2
    // @Path: AP_Headtracker_Params.cpp
    AP_SUBGROUPINFO(_params[1], "2", 2, AP_Headtracker, AP_Headtracker_Params),
#endif

    AP_GROUPEND
};

AP_Headtracker::AP_Headtracker()
{
    if (_singleton) {
        AP_HAL::panic("Too many Headtrackers");
    }
    _singleton = this;

    AP_Param::setup_object_defaults(this, var_info);
}

void AP_Headtracker::init()
{
     if (_num_instances != 0) {
        return;
    }

    for (uint8_t i=0; i<AP_HEADTRACKER_MAX_INSTANCES; i++) {
        switch ((Type)_params[i].type.get()) {
            case Type::None:
                break;
            case Type::CADDX:
                _backends[i] = new AP_Headtracker_CADDX(*this, _params[i], i);
                if (_backends[i]) {
                    _num_instances++;
                }
                break;
        }
    }
}

void AP_Headtracker::update()
{
    for (uint8_t i=0; i<_num_instances; i++) {
        if (_backends[i]) {
            _backends[i]->update();
        }
    }
}

void AP_Headtracker::set_enabled(bool enabled)
{
    if (_enabled == enabled) {
        return;
    }
    _enabled = enabled;
    if (!_enabled) {
        // Clear overrides for all configured channels
        for (uint8_t i=0; i<AP_HEADTRACKER_MAX_INSTANCES; i++) {
             if (_params[i].type.get() == (int8_t)Type::None) {
                 continue;
             }
             if (_params[i].yaw_chan > 0) {
                 RC_Channels::rc_channel(_params[i].yaw_chan - 1)->clear_override();
             }
             if (_params[i].pitch_chan > 0) {
                 RC_Channels::rc_channel(_params[i].pitch_chan - 1)->clear_override();
             }
             if (_params[i].roll_chan > 0) {
                 RC_Channels::rc_channel(_params[i].roll_chan - 1)->clear_override();
             }
        }
    }
}

namespace AP {
    AP_Headtracker *headtracker() {
        return AP_Headtracker::get_singleton();
    }
};

#endif // AP_HEADTRACKER_ENABLED
