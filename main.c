#include <string.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "bflb_mtimer.h"
#include "board.h"


#include <FreeRTOS.h>
#include "task.h"
#include "queue.h"

#include "bflb_mtd.h"
#include "easyflash.h"

#include "aipi_device.h"
#include "aipi_cloud.h"



int main(void)
{
    int32_t ret = 0;

    board_init();

    configASSERT((configMAX_PRIORITIES > 4));


    
    aipi_dev_t* r2_device = aipi_device_init();
    aipi_aithinker_cloud_start(r2_device);


    // flash_del();

    r2_device->state_update_cb(AIPI_STATE_SYS_START);

    vTaskStartScheduler();

    while (1)
    {
    }
}
