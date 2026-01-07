#pragma once

#include "AP_Headtracker_config.h"

#if AP_HEADTRACKER_ENABLED

#include <AP_Common/AP_Common.h>
#include <AP_Param/AP_Param.h>
#include "AP_Headtracker_Params.h"

#define AP_HEADTRACKER_MAX_INSTANCES 1

class AP_Headtracker_Backend;

class AP_Headtracker {
    friend class AP_Headtracker_Backend;

public:
    AP_Headtracker();

    /* Do not allow copies */
    CLASS_NO_COPY(AP_Headtracker);

    // get singleton instance
    static AP_Headtracker *get_singleton() {
        return _singleton;
    }

    // Enums
    enum class Type {
        None = 0,
        CADDX = 1,
    };

    // init - detect and initialise all headtrackers
    void init();

    // update - clear debug buffer and call update on all headtrackers
    void update();

    void set_enabled(bool enabled);
    bool enabled() const { return _enabled; }

    bool is_port_sharing_allowed(uint8_t instance) const {
        return instance < AP_HEADTRACKER_MAX_INSTANCES && _params[instance].share_port;
    }

    // parameter var table
    static const struct AP_Param::GroupInfo var_info[];

protected:
    // parameters for backends
    AP_Headtracker_Params _params[AP_HEADTRACKER_MAX_INSTANCES];

    // backends
    AP_Headtracker_Backend *_backends[AP_HEADTRACKER_MAX_INSTANCES];
    uint8_t _num_instances;
    bool _enabled = true;

private:
    static AP_Headtracker *_singleton;
};

namespace AP {
    AP_Headtracker *headtracker();
};

#endif // AP_HEADTRACKER_ENABLED
