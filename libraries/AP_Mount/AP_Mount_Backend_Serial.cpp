#include "AP_Mount_config.h"

#if HAL_MOUNT_ENABLED


#include "AP_Mount_Backend_Serial.h"

#include <AP_SerialManager/AP_SerialManager.h>
#include <AP_Headtracker/AP_Headtracker.h>

// Default init function for every mount
void AP_Mount_Backend_Serial::init()
{
    const AP_SerialManager& serial_manager = AP::serialmanager();

    // search for serial port.  hild classes should check that uart is not nullptr
    _uart = serial_manager.find_serial(AP_SerialManager::SerialProtocol_Gimbal, _serial_instance);

    // fallback to headtracker port if no gimbal port found
#if AP_HEADTRACKER_ENABLED
    if (_uart == nullptr) {
        // search for any headtracker instance that allows sharing
        const auto *headtracker = AP_Headtracker::get_singleton();
        if (headtracker) {
            for (uint8_t i = 0; i < AP_HEADTRACKER_MAX_INSTANCES; i++) {
                if (headtracker->is_port_sharing_allowed(i)) {
                    _uart = serial_manager.find_serial(AP_SerialManager::SerialProtocol_Headtracker, i);
                    if (_uart != nullptr) {
                        break;
                    }
                }
            }
        }
    }
#endif

    if (_uart == nullptr) {
        return;
    }

    // initialised successfully if uart is found
    _initialised = true;

    // call the parent class init
    AP_Mount_Backend::init();
}

#endif // HAL_MOUNT_ENABLED
