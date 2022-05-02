#pragma once
#include <Arduino.h>
#include "WiFi.h"

/*** Macro Definetions ***/
#define UART_LOG Serial
#define UART_COM Serial1
#define APP_PAYLOAD_MAX_LEN 4100

#define STATUS_FN(func, ret) if((func) == false) {ret}



/**
 * @brief intialize all the device hardwares and Peripherals,
 * 
 * 
 */
void dev_init(void);
