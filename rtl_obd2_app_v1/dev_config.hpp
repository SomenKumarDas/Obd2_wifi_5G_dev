#pragma once
#include <Arduino.h>
#include "WiFi.h"
#include "dev_log.hpp"

/*** Macro Definetions ***/
#define UART_LOG Serial
#define UART_COM Serial1
#define APP_PAYLOAD_MAX_LEN 4100

#define STATUS_FN(func, ret) if((func) == false) {ret}

#define log_v(format, ...) printf(APP_LOG_FORMAT(V, format), ##__VA_ARGS__)
#define log_d(format, ...) printf(APP_LOG_FORMAT(D, format), ##__VA_ARGS__)
#define log_i(format, ...) printf(APP_LOG_FORMAT(I, format), ##__VA_ARGS__)
#define log_w(format, ...) printf(APP_LOG_FORMAT_LINE(W, format), ##__VA_ARGS__)
#define log_e(format, ...) printf(APP_LOG_FORMAT_LINE(E, format), ##__VA_ARGS__)

#define sleep(x) vTaskDelay(pdMS_TO_TICKS(x));

/**
 * @brief intialize all the device hardwares and Peripherals,
 * 
 * 
 */
void dev_init(void);
