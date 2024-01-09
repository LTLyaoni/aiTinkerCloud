

/*----------------------------------include-----------------------------------*/
#include "aipi_device.h"

#include "FreeRTOS.h"
#include "task.h"

#include "aipi_cloud.h"

#include "cJSON.h"
/*-----------------------------------macro------------------------------------*/
static aipi_dev_t device;

static xTaskHandle sys_state_ctl;

static aipi_state_t sys_state;
/*----------------------------------typedef-----------------------------------*/

/*----------------------------------variable----------------------------------*/

/*-------------------------------------os-------------------------------------*/
void aipi_cloud_parse_control_data(char *control_data);
/*----------------------------------function----------------------------------*/
void flash_erase_set(char *key, char *value)
{
    size_t len = 0;
    int value_len = strlen(value);
    ef_set_and_save_env(key, value);
    // bflb_flash_read(key, flash_data, strlen(value));
    // printf("writer data:%s\r\n", flash_data);
    memset(value, 0, strlen(value));
    ef_get_env_blob(key, value, value_len, &len);
}

void aipi_flash_erase_set(wifi_sta_t *value)
{
    
    // flash_erase_set(FLASH_SSID_KEY, value->ssid);
    // flash_erase_set(FLASH_PWD_KEY, value->passworld);

    aiio_wifi_config_t conf;
    aiio_wifi_get_config(AIIO_WIFI_IF_STA, &conf);
    value->ssid = conf.sta.ssid;
    value->passworld = conf.sta.password;
    // printf("\r\n存储%s\r\n", value->ssid);
    flash_erase_set(FLASH_SSID_KEY, value->ssid);
    flash_erase_set(FLASH_PWD_KEY, value->passworld);

    flash_erase_set(FLASH_MQTT_IP, value->mqttip);
    flash_erase_set(FLASH_MQTT_TOKEN, value->token);
    char charPort[8];
    sprintf(charPort, "%hu", value->port);

    flash_erase_set(FLASH_MQTT_PORT, charPort);

    // ef_get_env_blob(FLASH_MQTT_PORT, value->port, sizeof(value->port), NULL);
}

void flash_erase_data(char *key)
{
    ef_del_and_save_env(key);
}

void flash_del(void)
{

    printf("\r\n删除flash\r\n");
    flash_erase_set(FLASH_SSID_KEY, "");
    flash_erase_set(FLASH_PWD_KEY, "");
    ef_env_set_default();
}
char *flash_get_data(char *key, int len)
{
    static char *flash_data;
    flash_data = pvPortMalloc(len);
    memset(flash_data, 0, len);

    ef_get_env_blob(key, flash_data, len, (size_t)&len);

    return flash_data;
}

/**
 * @brief
 *
 * @param key
 * @return char*
 */
wifi_sta_t *aipi_flash_get_data()
{
    // static char *flash_data;
    // flash_data = pvPortMalloc(len);
    // memset(flash_data, 0, len);

    // ef_get_env_blob(key, flash_data, len, (size_t)&len);
    static wifi_sta_t wifi_config_data = {0};

    // ef_get_env_blob(key, &wifi_config_data, sizeof(wifi_sta_t), NULL);
    // printf("读取到的长度 %d\r\n", sizeof(wifi_config_data));
    uint8_t len = 32;
    wifi_config_data.ssid = flash_get_data(FLASH_SSID_KEY, 32);
    wifi_config_data.passworld = flash_get_data(FLASH_PWD_KEY, 64);

    wifi_config_data.mqttip = flash_get_data(FLASH_MQTT_IP, 200);
    wifi_config_data.token = flash_get_data(FLASH_MQTT_TOKEN, 65);

    wifi_config_data.port = atoi(flash_get_data(FLASH_MQTT_PORT, 8)) ;

    return &wifi_config_data;
}

static void aipi_devstate_start(void *arg)
{
    aiio_rev_queue_t rev_queue = {0};

    while (1)
    {
        // 接收通知	 FreeRTOS的
        xTaskNotifyWait(0, 0xffffffff, &sys_state, portMAX_DELAY);

        switch (sys_state)
        {
        case AIPI_STATE_SYS_START:
        {
            aiio_log_d("从Flash 读取设备信息");
            // 从Flash 读取设备信息
            // uint8_t len = 32;
            // char *flash_ssid = flash_get_data(FLASH_SSID_KEY); // = "";  //
            // // 读不到SSID，开启配网
            // if (strlen(flash_ssid) == 0)
            // {
            //     aiio_log_d("读不到SSID，开启配网");
            //     // rev_queue.common_event = REV_CONFIG_START_EVENT;
            //     // if (xQueueSendToBack(cloud_rev_queue_handle, &rev_queue, 100) != pdPASS)
            //     // {
            //     //     aiio_log_e("queue send failed\r\n");
            //     // }
            //     // memset(&rev_queue, 0, sizeof(aiio_rev_queue_t));
            //     start_ble_config();

            //     break;
            // }
            // len = 64;
            // char *flash_pass = flash_get_data(FLASH_PWD_KEY, len);

            // device.wifi_sta.mqttip = flash_get_data(FLASH_MQTT_IP, 200);
            // device.wifi_sta.token = flash_get_data(FLASH_MQTT_TOKEN, 65);
            // // char *ports = flash_get_data(FLASH_MQTT_PORT, 8);
            // // device.wifi_sta.port = 1883;
            // device.wifi_sta.port = flash_get_data(FLASH_MQTT_PORT, 8);
            // // aiio_log_i("ip:%s,token:%s,port:%d", r2_device->mqttip, r2_device->token, r2_device->port);

            // aiio_log_d("从Flash 读取设备SSID信息==>%s\r\n",flash_ssid);
            // aiio_log_d("从Flash 读取设备pass信息==>%s\r\n",flash_pass);
            // aiio_log_d("从Flash 读取设备mqttip信息==>%s\r\n",device.mqttip);
            // aiio_log_d("从Flash 读取设备tk信息==>%s\r\n",device.token);
            // aiio_log_d("从Flash 读取设备ports信息==>%hu\r\n",ports);

            // aiio_log_d("开始联网\r\n");
            // iot_connect_wifi(flash_ssid, flash_pass);

            // vPortFree(flash_pass);
            // vPortFree(flash_ssid);
            // // vPortFree(mqttip);
            // // vPortFree(token);
            // vPortFree(ports);

            wifi_sta_t *wifi_sta = aipi_flash_get_data(); // = "";  //
            aiio_log_d("数据测试\r\n");
            vTaskDelay(pdMS_TO_TICKS(100));
            printf("ssid======= %d \r\n", wifi_sta->ssid);
            vTaskDelay(pdMS_TO_TICKS(100));
            printf("passworld======= %s \r\n", wifi_sta->passworld);
            vTaskDelay(pdMS_TO_TICKS(100));
            printf("mqttip======= %s \r\n", wifi_sta->mqttip);
            vTaskDelay(pdMS_TO_TICKS(100));
            printf("token======= %s \r\n", wifi_sta->token);
            vTaskDelay(pdMS_TO_TICKS(100));
            printf("port======= %d \r\n", wifi_sta->port);

            if (wifi_sta->ssid == NULL || !(strlen(wifi_sta->ssid) > 0))
            {
                aiio_log_d("读不到SSID，开启配网");
                // rev_queue.common_event = REV_CONFIG_START_EVENT;
                // if (xQueueSendToBack(cloud_rev_queue_handle, &rev_queue, 100) != pdPASS)
                // {
                //     aiio_log_e("queue send failed\r\n");
                // }
                // memset(&rev_queue, 0, sizeof(aiio_rev_queue_t));
                start_ble_config();
                break;
            }
            aiio_log_d("开始联网\r\n");
            device.wifi_sta = wifi_sta;

            iot_connect_wifi(device.wifi_sta->ssid, device.wifi_sta->passworld);
        }
        break;
        case AIPI_STATE_WIFI_CONNECT_OK: // WiFi 连接成功
        {
            aiio_log_d("WiFi 连接成功\r\n");
            // aiio_wifi_config_t conf;
            // aiio_wifi_get_config(AIIO_WIFI_IF_STA, &conf);
            // device.wifi_sta.ssid = conf.sta.ssid;
            // device.wifi_sta.passworld = conf.sta.password;
            // flash_erase_set(FLASH_SSID_KEY, device.wifi_sta.ssid);
            // flash_erase_set(FLASH_PWD_KEY, device.wifi_sta.passworld);
        }
        break;
        case AIPI_STATE_SERVER_CONNECT_OK: // 服务器连接成功
        {
            aiio_log_d("服务器连接成功\r\n");
            // 通知上线
            // 保存信息
            // flash_erase_set(FLASH_MQTT_IP, device.mqttip);
            // flash_erase_set(FLASH_MQTT_TOKEN, device.token);
            // flash_erase_set(FLASH_MQTT_PORT, device.port);
            // aiio_service_report_attribute(NULL, NULL, "{ \"1\":false}");
            // aiio_service_report_attribute(NULL, NULL, "{ \"4\":false}");
            // aiio_service_report_attribute(NULL, NULL, "{ \"5\":false}");
            // lv_label_set_text(r2_device->ui->config_label_ip, r2_device->wifi_sta.ip_addrs);
            aipi_flash_erase_set(device.wifi_sta);
        }
        break;
        case AIPI_STATE_CONTRL_DATA_EVENT:
        {
            aiio_log_d("收到云控制");
            // r2_device->relay_sw1->on(r2_device->relay_sw1);
            // aiio_service_report_attribute(NULL, NULL, "{ \"101\":true}");
            // if (r2_device->relay_sw1->gpio_state == true && r2_device->relay_sw2->gpio_state == true && r2_device->relay_sw3->gpio_state == true)
            //     aiio_service_report_attribute(NULL, NULL, "{ \"1\":true}");

            aiio_log_d("接受到控制 DATA=%s \r\n", device.mq_data);

            aipi_cloud_parse_control_data(device.mq_data);
        }
        break;
        case AIPI_STATE_CONFIG_START:
        {

            rev_queue.common_event = REV_CLOUD_DISCONNECTED;
            if (xQueueSendToBack(cloud_rev_queue_handle, &rev_queue, 100) != pdPASS)
            {
                aiio_log_i("queue send failed\r\n");
            }
            memset(&rev_queue, 0, sizeof(aiio_rev_queue_t));
            vTaskDelay(pdMS_TO_TICKS(100));
            rev_queue.common_event = REV_CONFIG_START_EVENT;
            if (xQueueSendToBack(cloud_rev_queue_handle, &rev_queue, 100) != pdPASS)
            {
                aiio_log_i("queue send failed\r\n");
            }
            memset(&rev_queue, 0, sizeof(aiio_rev_queue_t));
        }
        break;
        default:
            break;
        }
    }
}
void aipi_dev_state_update(aipi_state_t new_state)
{
    // aiio_log_d("发送通知");
    if (sys_state_ctl == NULL)
    {
        aiio_log_e("sys_state stask is not Create OK!");
        return;
    }
    xTaskNotify(sys_state_ctl, new_state, eSetValueWithoutOverwrite);
}
aipi_dev_t *aipi_device_init(void)
{

    bflb_mtd_init();
    easyflash_init();

    relays_device_init(&device);

    device.state_update_cb = aipi_dev_state_update;

    xTaskCreate(aipi_devstate_start, "sys_state", 1024, NULL, 2, &sys_state_ctl);

    return &device;
}

/*------------------------------------control------------------------------------*/
void analyzeJson(cJSON *root)
{
    for (int j = 0; j < cJSON_GetArraySize(root); j++)
    {
        cJSON *item = cJSON_GetArrayItem(root, j);
        if (item->type == cJSON_Object)
        { // 数组
            if (!cJSON_IsNull(item))
            {
                // if (item->string)//可能存在对象名为NULL{"key":"value"}对象名为NULL   )
                // cout << "cJSON_Object "<< j << " :"<< item->string << "    cJSON_GetArraySize(Object): " << cJSON_GetArraySize(item) << endl;
                analyzeJson(item);
            }
        }
        else if (item->type == cJSON_Array)
        {
            if (!cJSON_IsNull(item))
            { // 对象
                // if(item->string)
                // cout << "cJSON_Array " << j << " :" << item->string << "    cJSON_GetArraySize(Array): " << cJSON_GetArraySize(item) << endl;
                analyzeJson(item);
            }
        }
        else
        {
            if (!cJSON_IsNull(item) && device.control_cb)
            { // 键值对，注意键类型
                if (item->type == cJSON_String)
                {
                    device.control_cb(item->string, item->valuestring, 0);
                }
                else if (item->type == cJSON_Number)
                {
                    device.control_cb(item->string, NULL, item->valuedouble);
                }
                else if (item->type == cJSON_False || item->type == cJSON_True)
                {
                    device.control_cb(item->string, NULL, item->type == cJSON_True);
                }
                // cout << "key(number) " << j << " : " << item->string << ";   value:" << item->valueint << endl;
            }
        }
    }
}

/// @brief
/// @param control_data
void aipi_cloud_parse_control_data(char *control_data)
{

    cJSON *cjson_root = NULL;
    cJSON *cjson_control = NULL;
    uint16_t cmd_num = 0;
    cJSON *cjson_value = NULL;
    char str[10] = {0};

    if (control_data == NULL)
    {
        aiio_log_e("param err \r\n");
        return;
    }
    //  aiio_os_tick_dealy(aiio_os_ms2tick(5000));
    aiio_log_d("control_data = %s \r\n", control_data);
    // aiio_os_tick_dealy(aiio_os_ms2tick(5000));
    cjson_root = cJSON_Parse(control_data);
    // aiio_os_tick_dealy(aiio_os_ms2tick(5000));
    if (cjson_root == NULL)
    {
        aiio_log_e("json parse err \r\n");
        return;
    }
    // aiio_os_tick_dealy(aiio_os_ms2tick(5000));
    cJSON *control = cJSON_GetObjectItem(cjson_root, STR_NAME_CONTROL);

    analyzeJson(control);

    cJSON_Delete(cjson_root);
}