#pragma once

/** Macro includes and definations **/
#include "dev_config.hpp"

/*** Global external variables ***/
extern TaskHandle_t AppSoc_TH;

/**
 * @brief Task to handle TCP Socket Server Activity
 *        Application Data Entry point.
 * 
 * @param args NULL ptr
 */
void app_srv_task(void *args);