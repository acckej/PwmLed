#pragma once

#define Arduino

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