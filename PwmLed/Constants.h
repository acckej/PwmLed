#pragma once

#define DISTANCE static_cast<double>(2350)
#define TOP_SPEED static_cast<double>(static_cast<double>(8333) / static_cast<double>(2000))

#define RED_PIN 3
#define GREEN_PIN 6
#define BLUE_PIN 5
#define BLINK_PIN 13

#define DEBOUNCE_DELAY 100
#define NOT_MOVING_DELAY 6000
#define COLOR_CHANGE_PERIOD 1000
#define MS_COEF static_cast<double>(1000)

#define VOLTAGE_PIN 4
#define VOLTAGE_COEF 0.345
#define VCC 5.0
#define THRESHOLD_VOLTAGE 9
#define VOLTAGE_DELIMITER 1023 * VCC / VOLTAGE_COEF
#define BLINK_DELAY 1000
#define IDLE_DELAY 300
