#include "app_soc.hpp"

/* Application OS TASK Global Vars */

TaskHandle_t AppSoc_TH;
SemaphoreHandle_t WIFI_TCPSocComplete_Sem;

/* App Local Vars */

WiFiClient wClient;
WiFiServer SocSrv(6889);
static wl_status_t status = WL_IDLE_STATUS;
uint8_t RxBuff[APP_PAYLOAD_MAX_LEN], TxBuff[APP_PAYLOAD_MAX_LEN];
int TcpRxLen = 0, UartRxLen = 0;

/* Local Functions Protoypes */
static bool wifi_init(void);

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
    STATUS_FN(wifi_init(), goto exit;)

    SocSrv.begin();

    for (;;)
    {
        wClient = SocSrv.available();
        if (wClient.connected() == true)
        {
            UART_LOG.println("[INFO] [New client Connected]");
            digitalWrite(LED_B, LOW);
            digitalWrite(LED_G, HIGH);
            while (wClient.connected())
            {
                if ((TcpRxLen = wClient.read(RxBuff, 4100)) > 0)
                {
                    UART_LOG.print("[D] [TcpRxLen: "); UART_LOG.println(TcpRxLen);
                    UART_COM.write(RxBuff, TcpRxLen);
                }

                if ((UartRxLen = UART_COM.available()) > 0)
                {
                    for(uint16_t idx = 0; idx < UartRxLen; idx++)
                    {
                        TxBuff[idx] = UART_COM.read();
                    }

                    if(wClient.write(TxBuff, UartRxLen) <= 0)
                    {
                        UART_LOG.print("[D] [break by write");
                        break;
                    }
                }

                vTaskDelay(pdMS_TO_TICKS(100));
            }
            UART_LOG.println("[INFO] [Client Disconnected]");
            wClient.stop();
            digitalWrite(LED_G, LOW);
            digitalWrite(LED_B, HIGH);
        }

        vTaskDelay(pdMS_TO_TICKS(100));
    }

exit:
    UART_LOG.println("[ERROR] [TASK EXIT]");
    vTaskDelete(NULL);
}

static bool wifi_init(void)
{
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

    return false;
}