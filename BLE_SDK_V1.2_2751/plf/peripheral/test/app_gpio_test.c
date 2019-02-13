/*
 * app_gpio_test.c
 *
 *  Created on: 2018Äê7ÔÂ31ÈÕ
 *      Author: mingzhou
 */

#include "bx_config.h"
#include "app_gpio.h"
#include "log.h"

void app_gpio_test_cb(void)
{
    LOG(LOG_LVL_INFO,"app_gpio_test_cb\n");
}

/*
void app_gpio_test_input_example(void)
{
    app_gpio_set_callback(gpio2_4, &app_gpio_test_cb);
}
*/
