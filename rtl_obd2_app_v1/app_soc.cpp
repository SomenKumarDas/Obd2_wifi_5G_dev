#include "app_soc.hpp"

/* Application OS TASK Global Vars */
TaskHandle_t AppSoc_TH, SocTx_TH;
SemaphoreHandle_t SocComplete_Sem;

/* Local Functions Protoypes */
static bool wifi_init(void);
static void socTx_subTask(void *args);

/****************************
 * Global Function Definitions
 ****************************/

/**
 * @brief Task to handle TCP Socket Server Activity
 *        Application Data Entry point.
 *
 * @param args NULL ptr
 */
void app_srv_task(void *args)
{
    WiFiClient wClient;
    WiFiServer SocSrv(6889);
    uint8_t RxBuff[APP_PAYLOAD_MAX_LEN];
    int SocRxLen = 0;

    STATUS_FN(wifi_init(), goto exit;)
    SocSrv.begin();

    for (;;)
    {
        wClient = SocSrv.available();
        if (wClient.connected() == true)
        {
            log_i("[New client Connected]");
            digitalWrite(LED_B, LOW);
            digitalWrite(LED_G, HIGH);
            xTaskCreate(socTx_subTask, "socTx_subTask", 1024 * 5, (void*)&wClient, 2, &SocTx_TH);
            while (wClient.connected())
            {
                if ((SocRxLen = wClient.read(RxBuff, sizeof(RxBuff))) > 0)
                {
                    UART_COM.write(RxBuff, SocRxLen);
                    log_d("[SocRxLen: %d]", SocRxLen);
                }

                sleep(5);
            }
            log_i("[Client Disconnected]");
            vTaskDelete(SocTx_TH);
            wClient.stop();
            digitalWrite(LED_G, LOW);
            digitalWrite(LED_B, HIGH);
        }

        sleep(100);
    }

exit:
    log_e("[APP] [TASK EXIT]");
    vTaskDelete(NULL);
}

static void socTx_subTask(void *args)
{
    log_i("[socTx_subTask start]");
    uint8_t TxBuff[APP_PAYLOAD_MAX_LEN];
    WiFiClient * SocCl = static_cast<WiFiClient*>(args);
    int comRxLen = 0, SocTxLen = 0;
    uint8_t *dPtr = NULL;
    for (;;)
    {

        if ((comRxLen = UART_COM.available()) > 0)
        {
            dPtr = &TxBuff[0];
            SocTxLen = 0;
            while (comRxLen != UART_COM.available())
            {
                sleep(100);
                comRxLen = UART_COM.available();
            }

            while (comRxLen--)
            {
                *dPtr++ = UART_COM.read();
                SocTxLen++;
            }

            if (SocCl->write(&TxBuff[0], SocTxLen))
            {
                log_d("[SocTxLen: %d]", SocTxLen);
            }
        }
        // while (UART_COM.available())
        // {
        //     wClient.write(UART_COM.read());
        // }

        sleep(10);
    }
    log_w("[APP] [socTx_subTask stop]");
    SocCl->stop();
    vTaskDelete(NULL);
}

static bool wifi_init(void)
{
    static wl_status_t status = WL_IDLE_STATUS;
    MDNSService service("obd5gz", "_tcp", "local", 80);

    if (WiFi.status() == WL_NO_SHIELD)
    {
        UART_LOG.println("[ERROR] [WL_NO_SHIELD]");
        return false;
    }

    // Need to write handler for connection
    while (status != WL_CONNECTED)
    {
        digitalWrite(LED_B, !digitalRead(LED_B));
        status = (wl_status_t)WiFi.begin("Transient", "ToxiCViruS");
        vTaskDelay(pdMS_TO_TICKS(500));
    }

    if (WiFi.status() == WL_CONNECTED)
    {
        digitalWrite(LED_B, HIGH);
        return true;
    }

    MDNS.begin();
    MDNS.registerService(service);

    return false;
}