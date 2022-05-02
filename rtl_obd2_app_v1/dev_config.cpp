#include "dev_config.hpp"



/*** Global Function Definitions ***/

/**
 * @brief intialize all the device hardwares and Peripherals,
 * 
 * 
 */
void dev_init(void)
{
    UART_LOG.begin(115200);
    UART_COM.begin(460800);

    pinMode(LED_G, OUTPUT);
    pinMode(LED_B, OUTPUT);
}