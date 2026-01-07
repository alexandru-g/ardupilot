#include "AP_Headtracker_Params.h"

const AP_Param::GroupInfo AP_Headtracker_Params::var_info[] = {
    // @Param: _TYPE
    // @DisplayName: Headtracker Type
    // @Description: Type of headtracker attached
    // @Values: 0:None, 1:CADDX
    // @User: Standard
    AP_GROUPINFO_FLAGS("_TYPE", 1, AP_Headtracker_Params, type, 0, AP_PARAM_FLAG_ENABLE),

    // @Param: _YAW_CHAN
    // @DisplayName: Yaw Channel
    // @Description: Output RC channel for Yaw
    // @User: Standard
    AP_GROUPINFO("_YAW_CHAN", 2, AP_Headtracker_Params, yaw_chan, 0),

    // @Param: _PITCH_CHAN
    // @DisplayName: Pitch Channel
    // @Description: Output RC channel for Pitch
    // @User: Standard
    AP_GROUPINFO("_PITCH_CHAN", 3, AP_Headtracker_Params, pitch_chan, 0),

    // @Param: _ROLL_CHAN
    // @DisplayName: Roll Channel
    // @Description: Output RC channel for Roll
    // @User: Standard
    AP_GROUPINFO("_ROLL_CHAN", 4, AP_Headtracker_Params, roll_chan, 0),

    // @Param: _SHARE_PORT
    // @DisplayName: Share Port with Mount
    // @Description: Allow a Mount to use the same Serial Port as this Headtracker
    // @Values: 0:Disabled, 1:Enabled
    // @User: Standard
    AP_GROUPINFO("_SHARE_PORT", 5, AP_Headtracker_Params, share_port, 0),

    AP_GROUPEND
};

AP_Headtracker_Params::AP_Headtracker_Params(void)
{
    AP_Param::setup_object_defaults(this, var_info);
}
