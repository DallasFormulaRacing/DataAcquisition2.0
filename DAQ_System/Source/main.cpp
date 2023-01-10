#include "mbed.h"
#include "SDBlockDevice.h"

// Instantiate the SDBlockDevice by specifying the SPI pins connected to the SDCard
// socket. The PINS are:
//     MOSI (Master Out Slave In)
//     MISO (Master In Slave Out)
//     SCLK (Serial Clock)
//     CS (Chip Select)
SDBlockDevice sd(MBED_CONF_SD_SPI_MOSI, MBED_CONF_SD_SPI_MISO, MBED_CONF_SD_SPI_CLK, MBED_CONF_SD_SPI_CS);
uint8_t block[512] = "Hello World!\n";

int main()
{
    // Call the SDBlockDevice instance initialisation method
    if (0 != sd.init()) {
        printf("Init failed \n");
        return -1;
    }
    printf("sd size: %llu\n",         sd.size());
    printf("sd read size: %llu\n",    sd.get_read_size());
    printf("sd program size: %llu\n", sd.get_program_size());
    printf("sd erase size: %llu\n",   sd.get_erase_size());

    // Set the frequency
    if (0 != sd.frequency(5000000)) {
        printf("Error setting frequency \n");
    }

    if (0 != sd.erase(0, sd.get_erase_size())) {
        printf("Error Erasing block \n");
    }

    // Write data block to the device
    if (0 == sd.program(block, 0, 512)) {
        // Read the data block from the device
        if (0 == sd.read(block, 0, 512)) {
            // Print the contents of the block
            printf("%s", block);
        }
    }

    // Call the SDBlockDevice instance de-initialisation method
    sd.deinit();
}
