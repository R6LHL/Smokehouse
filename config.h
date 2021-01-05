#ifndef _CONFIG_H
#define _CONFIG_H

unsigned const char OS_QUEUE_SIZE = 5;
const float max_temp = 30.0;
const float norm_temp = 20.0;

unsigned const char fan_pwm_stop = 0;
unsigned const char fan_pwm_max = 255;
unsigned const char fan_pwm_additive = fan_pwm_max * 0.1; //motor start compensation;
unsigned const char fan_pwm_mid = (fan_pwm_max / 2) + fan_pwm_additive;
unsigned const char fan_pwm_min = fan_pwm_stop + fan_pwm_additive;

unsigned const int check_smoke_sensor_delay = 1000;
unsigned const int temp_check_delay = 1000;
unsigned const int null_delay = 0;

unsigned const int ADC_bit_depth = 1024;

//////////// PINOUTS DESCRIPTION///////////////////////////////////////////////////////
typedef unsigned const char pin_number;

pin_number fan_pin1 = 6;
pin_number fan_pin2 = 2;
pin_number rpm_regulator_pin = 23;
pin_number smoke_sensor_analog_pin = 24;
pin_number smoke_sensor_digital_pin = 8;

#define ONE_WIRE_PIN (12)
pin_number temp_sensor_pin = ONE_WIRE_PIN;
///////////////////////////////////////////////////////////////////////////////////////

#define DEBUG_ENABLED
  #ifdef DEBUG_ENABLED
    #define UART_BITRATE (9600)
  #endif //DEBUG_ENABLED

#endif // _CONFIG_H
