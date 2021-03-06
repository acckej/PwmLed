#pragma once

#define RED_PIN 3
#define GREEN_PIN 6
#define BLUE_PIN 5
#define BLINK_PIN 13

#define BT_RX_PIN 7
#define BT_TX_PIN 8

#define DEBOUNCE_DELAY 100
#define MS_COEF static_cast<double>(1000)
#define COLOR_THRESHOLD static_cast<double>(2)

#define VOLTAGE_PIN 4
#define VOLTAGE_COEF 0.345
#define VCC 5.0
#define THRESHOLD_VOLTAGE 9
#define VOLTAGE_DELIMITER 1023 * VCC / VOLTAGE_COEF
#define BLINK_DELAY 1000
#define IDLE_DELAY 300

#define READ_BUFFER_SIZE 512

#define COLOR_CORRECTION_BLUE static_cast<double>(1)
#define COLOR_CORRECTION_GREEN static_cast<double>(1.243)
#define COLOR_CORRECTION_RED static_cast<double>(2.706)




