
#ifndef relays_dev.c
#define relays_dev .c

/*----------------------------------include-----------------------------------*/
#include "aipi_device.h"
#include "relays_dev.h"

#include "bflb_pwm_v2.h"
#include "bflb_clock.h"
#include "board.h"
#include "bflb_gpio.h"
/*-----------------------------------macro------------------------------------*/

/*----------------------------------typedef-----------------------------------*/
struct bflb_device_s *pwm;

struct bflb_device_s *gpio;
/*----------------------------------variable----------------------------------*/

/*-------------------------------------os-------------------------------------*/

/*----------------------------------function----------------------------------*/
void set_RGB_pwm(int DPID, double low_threhold)
{
    // bflb_pwm_v2_channel_set_threshold(pwm, PWM_CH2, 0, i);
    // // PWM 占空比 = (high_threhold - low_threhold)/period
    switch (DPID)
    {
    case 101:
        bflb_pwm_v2_channel_set_threshold(pwm, PWM_CH0, 0, low_threhold * 10);
        break;
    case 102:
        bflb_pwm_v2_channel_set_threshold(pwm, PWM_CH2, 0, low_threhold * 10);
        break;
    case 103:
        bflb_pwm_v2_channel_set_threshold(pwm, PWM_CH3, 0, low_threhold * 10);
        break;
    default:
        break;
    }
}

void cloud_control_cb(char *pid, char *valuestring, double valuedouble)
{

    printf("\r\n 标识 pid---------->:%s\r\n", pid);

    int pid_int = atoi(pid);

    switch (pid_int)
    {
    case 1:
    {
        printf("\r\n 标识 pid---------->:%s---%d\r\n", pid, valuedouble);
        abs(valuedouble) < 0.000001 ? bflb_gpio_set(gpio, GPIO_PIN_10) : bflb_gpio_reset(gpio, GPIO_PIN_10);
        // if (abs(valuedouble) < 0.000001)
        // {
        //     printf("\r\n 关灯 \r\n");
        //     bflb_gpio_set(gpio, GPIO_PIN_10);
            
        // }
        // else 
        // {
        //     printf("\r\n 开灯 \r\n");
        //    bflb_gpio_reset(gpio, GPIO_PIN_10);
           
        // }
        
        //         // bflb_gpio_set(gpio, GPIO_PIN_10);
        //  bflb_gpio_reset(gpio, GPIO_PIN_10);
    }
    break;
    case 101:
    case 102:
    case 103:
        set_RGB_pwm(pid_int, valuedouble);
        break;
    default:
        break;
    }

    // 得上传状态改变
    // aiio_service_report_attribute(msgMid, from, json_str);
}

void board_pwm_led_init()
{
    struct bflb_device_s *pwm_gpio;

    pwm_gpio = bflb_device_get_by_name("gpio");
    // 红灯: GPIO_PIN_12, PWM_CH0
    bflb_gpio_init(pwm_gpio, GPIO_PIN_12, GPIO_FUNC_PWM0 | GPIO_ALTERNATE | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_1);
    // 绿灯: GPIO_PIN_14, PWM_CH2
    bflb_gpio_init(pwm_gpio, GPIO_PIN_14, GPIO_FUNC_PWM0 | GPIO_ALTERNATE | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_1);
    // 蓝灯: GPIO_PIN_15, PWM_CH3
    bflb_gpio_init(pwm_gpio, GPIO_PIN_15, GPIO_FUNC_PWM0 | GPIO_ALTERNATE | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_1);

    pwm = bflb_device_get_by_name("pwm_v2_0");

    /* period = .XCLK / .clk_div / .period = 40MHz / 40 / 1000 = 1KHz */
    struct bflb_pwm_v2_config_s cfg = {
        .clk_source = BFLB_SYSTEM_PBCLK,
        .clk_div = 80,
        .period = 1000,
    };

    bflb_pwm_v2_init(pwm, &cfg);
    bflb_pwm_v2_start(pwm);

    uint16_t high_threhold = 1000;

    bflb_pwm_v2_channel_positive_start(pwm, PWM_CH0);
    bflb_pwm_v2_channel_positive_start(pwm, PWM_CH2);
    bflb_pwm_v2_channel_positive_start(pwm, PWM_CH3);

    gpio = bflb_device_get_by_name("gpio");
    bflb_gpio_init(gpio, GPIO_PIN_10, GPIO_OUTPUT | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_0);
}

void relays_device_init(void *device_t)
{

    aipi_dev_t *aipi_device = (aipi_dev_t *)device_t;

    aipi_device->control_cb = cloud_control_cb;

    board_pwm_led_init();
}

/*------------------------------------test------------------------------------*/

#endif /* relays_dev.c */
