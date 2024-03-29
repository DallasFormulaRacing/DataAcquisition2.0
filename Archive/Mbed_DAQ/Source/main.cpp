// /*
// * DataAcquisition2.0 — 2023 Vehicle
// * (c) 2022 Dallas Formula Racing - Embedded Firmware Team
// * Formula SAE International Collegiate Chapter
// * GPL-3.0 License
// */

// DFR Custom Dependancies
#include "Application/DAQ/daq.hpp"

using AutoReloadTimer = LowPowerTicker;

static InterruptIn button(PC_13);
static bool data_logging_enable = false;
static bool logging = false;

static void logging_signal() {
    logging = true;
}

// interrupt routine activated by a falling edge of button input
void ToggleDataLogging(void) {
    data_logging_enable = !data_logging_enable;
}

int main() {
    // Init components
    application::DAQ daq;
    daq.Init();
    
    // Start timer
    constexpr uint8_t kLoggingRate = 3; // seconds
    AutoReloadTimer timer;
    timer.attach(&logging_signal, std::chrono::seconds(kLoggingRate));

    // use internal pullup for pushbutton 
    button.mode(PullUp);
    wait_ns(100);
    button.fall(&ToggleDataLogging);

    uint8_t open_file = 0;
    float timestamp = 0.0f;


    printf("Entering Super Loop\n");
    // Operate
    while (true) {
        daq.Read();

        // TODO: De-couple this condition. It should not take 3 to close the file
        if (logging) {
            // Operate: Writing
            timestamp += kLoggingRate;
            daq.Write(timestamp, data_logging_enable);
            logging = false;
        }
    }
}
