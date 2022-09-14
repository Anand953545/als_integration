#ifndef LED_CONFIG_H_
#define LED_CONFIG_H_

#include <stdint.h>

#define AWARE_LED_CONFIG_FILE_PATH "/datatx/led.conf"

typedef struct
{
    uint8_t led_on_enum;
    uint16_t duration_in_sec;
    
    uint8_t led_1_status_enum;
    uint8_t led_1_duration_in_sec;

    uint8_t led_2_status_enum;
    uint8_t led_2_duration_in_sec;

    uint8_t transition_duration_in_sec;

    uint8_t led_1_info_enum;
    uint8_t led_1_info_duration_in_sec;
    uint8_t led_1_info_pause_in_sec;

    uint8_t led_2_info_enum;
    uint8_t led_2_info_duration_in_sec;
    uint8_t led_2_info_pause_in_sec;

    uint8_t led_color_no_of_digits;
    uint8_t led_color_digit;
    uint8_t led_color_digit_duration_in_sec;
    uint8_t led_color_blank_duration_in_sec;
    uint8_t led_color_pause_digit;
    uint8_t led_all_digit_color;
    uint8_t led_no_of_min;


    
}led_config_t;

int led_config_update(void*, uint8_t*);
int led_config_create(led_config_t*);

#endif