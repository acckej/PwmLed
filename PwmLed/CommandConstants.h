#pragma once

#define Arduino
#define ProMini

#ifdef Test

typedef __int16 ArduinoInt;
typedef float ArduinoDouble;
typedef __int16 ArduinoSize;

#endif

#ifdef Arduino

typedef int ArduinoInt;
typedef double ArduinoDouble;
typedef int ArduinoSize;

#endif

const unsigned char SystemInformationPacketId = 1;
const unsigned char SpeedColorProgramPacketId = 3;
const unsigned char CommandResultPacketId = 6;
const unsigned char ColorProgramPacketId = 7;
const unsigned char EmptyDataPacketId = 9;

const unsigned char GetSystemInformationCommandId = 2;
const unsigned char GetSpeedColorProgramCommandId = 4;
const unsigned char UploadSpeedColorProgramCommandId = 5;
const unsigned char UploadColorProgramCommandId = 8;
const unsigned char ErrorCommandId = 10;
const unsigned char GetColorProgramCommandId = 11;

const ArduinoDouble Distance = 2350;
const ArduinoDouble TopSpeed = static_cast<ArduinoDouble>(static_cast<ArduinoDouble>(8333) / static_cast<ArduinoDouble>(2000));

const ArduinoInt NotMovingDelay = 6000;
const ArduinoInt ColorChangePeriod = 1000;

const ArduinoInt BlinkDelay = 1000;
const ArduinoInt IdleDelay = 300;

const ArduinoDouble MuGreen = TopSpeed / 2;
const ArduinoDouble MuRed = TopSpeed / 4;
const ArduinoDouble MuBlue = 0;

const ArduinoDouble SigmaGreen = 4;
const ArduinoDouble SigmaRed = 4;
const ArduinoDouble SigmaBlue = 2;

#ifdef UnoMega

#define COLORPROGRAM_FLAG_ADDRESS 512
#define BUF_SIZE 256

#endif

#ifdef ProMini

#define COLORPROGRAM_FLAG_ADDRESS 512
#define BUF_SIZE 256

#endif

#define COLOR_PROGRAM_EEPROM_SEGMENT COLORPROGRAM_FLAG_ADDRESS + sizeof(ArduinoInt)

#define SPEEDCOLOR_FLAG_ADDRESS 0
#define SPEED_COLOR_EEPROM_SEGMENT sizeof(ArduinoInt)