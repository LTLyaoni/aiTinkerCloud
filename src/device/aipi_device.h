

/*----------------------------------include-----------------------------------*/
#include "bflb_gpio.h"
/*-----------------------------------macro------------------------------------*/

#define FLASH_WIFI_CONFIG_KEY "WIFICONFIG"

#define FLASH_SSID_KEY "SSID"
#define FLASH_PWD_KEY "PASS"
#define FLASH_MQTT_IP "mqip"
#define FLASH_MQTT_TOKEN "token"
#define FLASH_MQTT_PORT "port"

/*----------------------------------typedef-----------------------------------*/
typedef enum {
    AIPI_STATE_SYS_START = 0,
    AIPI_STATE_CONFIG_START,
    AIPI_STATE_CONFIG_OK,
    AIPI_STATE_CONFIG_ERR,
    AIPI_STATE_WIFI_CONNECT_OK,
    AIPI_STATE_WIFI_CONFIG_ERR,
    AIPI_STATE_SERVER_CONNECT_OK,
    AIPI_STATE_CONTRL_DATA_EVENT,
}aipi_state_t;


typedef struct device_wifi {
    char* ssid;
    char* passworld;
    // char* ip_addrs;
    char* mqttip;
    char* token;
    uint16_t port;
    bool state;
}wifi_sta_t;

typedef struct device_time_date {
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t wday;
    uint8_t hour;
    uint8_t min;
    uint8_t sec;
}time_date_t;

// typedef struct relays_dev {
//     char* relays_name;
//     uint8_t gpio_pin;
//     bool gpio_state;
//     void (*on)(struct relays_dev*);
//     void (*off)(struct relays_dev*);
// }relays_dev_t;


typedef void(*dev_state_update_cb)(void* event);
typedef void(*dev_cloud_control_cb)(char* pid,char* valuestring, double valuedouble);

typedef struct device_t {
    // lv_ui* ui;
    wifi_sta_t* wifi_sta;
    time_date_t date;
    char *mq_data;
    // r286_sys_language_t  language;
    dev_state_update_cb state_update_cb;

    dev_cloud_control_cb control_cb;


} aipi_dev_t;



// /*----------------------------------variable----------------------------------*/

// /*-------------------------------------os-------------------------------------*/

// /*----------------------------------function----------------------------------*/
aipi_dev_t* aipi_device_init(void);
void aipi_cloud_parse_control_data(char *control_data);
void flash_del(void);
/*------------------------------------test------------------------------------*/


