#include "AP_Headtracker_CADDX.h"
#include <RC_Channel/RC_Channel.h>
#include <AP_HAL/AP_HAL.h>
#include <AP_SerialManager/AP_SerialManager.h>

extern const AP_HAL::HAL& hal;

AP_Headtracker_CADDX::AP_Headtracker_CADDX(AP_Headtracker &frontend, AP_Headtracker_Params &params, uint8_t instance) :
    AP_Headtracker_Backend(frontend, params, instance)
{
    AP_SerialManager &serial_manager = AP::serialmanager();
    // Search for SerialProtocol_Headtracker (50)
    const AP_SerialManager::SerialProtocol prot = AP_SerialManager::SerialProtocol_Headtracker;
    port = serial_manager.find_serial(prot, 0);

    if (port) {
        // Baudrate is handled by SerialManager (default 115200 usually)
        port->begin(serial_manager.find_baudrate(prot, 0));
    }
}

void AP_Headtracker_CADDX::update()
{
    if (!port) {
        return;
    }

    uint32_t n = port->available();
    while (n--) {
        process_byte(port->read());
    }
}

void AP_Headtracker_CADDX::process_byte(uint8_t byte)
{
    // Shift Window
    memmove(&window[0], &window[1], 9);
    window[9] = byte;
    
    // Check for Sync (A5 5A) at start
    if (window[0] == 0xA5 && window[1] == 0x5A) {
        
        // CRC Check (CCITT)
        uint16_t calc_crc = crc16_ccitt(window, 8, 0); 
        uint16_t recv_crc = (window[8] << 8) | window[9];
        
        if (calc_crc == recv_crc) {
             memcpy(payload, &window[2], 6);
             process_packet();
             memset(window, 0, 10);
        }
    }
}

void AP_Headtracker_CADDX::process_packet()
{
    int16_t roll = ((payload[1] >> 4) & 0x0F) | ((uint16_t)payload[2] << 4);
    if (roll & 0x800) roll |= 0xF000;
    
    int16_t tilt = payload[3] | ((payload[4] & 0x0F) << 8);
    if (tilt & 0x800) tilt |= 0xF000;
    
    int16_t pan = ((payload[4] >> 4) & 0x0F) | ((uint16_t)payload[5] << 4);
    if (pan & 0x800) pan |= 0xF000;

    // Map -2048..2048 -> 1000..2000
    auto map_val = [](int16_t val) -> uint16_t {
         int32_t pwm = 1500 + (val * 500 / 2048);
         return constrain_int32(pwm, 1000, 2000);
    };
    
    // Inject RC Overrides
    if (_frontend.enabled()) {
        if (_params.yaw_chan > 0) {
            RC_Channels::set_override(_params.yaw_chan - 1, map_val(pan));
        }
        if (_params.pitch_chan > 0) {
            RC_Channels::set_override(_params.pitch_chan - 1, map_val(tilt));
        }
        if (_params.roll_chan > 0) {
            RC_Channels::set_override(_params.roll_chan - 1, map_val(roll));
        }
    }
}
