#pragma once

#include <AP_Param/AP_Param.h>

class AP_Headtracker_Params {
public:
    AP_Headtracker_Params(void);

    /* Do not allow copies */
    CLASS_NO_COPY(AP_Headtracker_Params);

    static const struct AP_Param::GroupInfo var_info[];

    AP_Int8 type;
    AP_Int8 yaw_chan;
    AP_Int8 pitch_chan;
    AP_Int8 roll_chan;
    AP_Int8 share_port;
};
