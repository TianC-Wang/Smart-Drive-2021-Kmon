#include "Kmon.h"

#define getGrayscale(index)   geteadc(index)
#define getDistance(index)    getadc(index)
#define bool                  char
#define true                  1
#define false                 0

const int left3rdGrayscale  = 1;
const int left2ndGrayscale  = 2;
const int left1stGrayscale  = 3;
const int right1stGrayscale = 4;
const int right2ndGrayscale = 5;
const int right3rdGrayscale = 6;
const int rightSonar        = 3;
const int frontRightSonar   = 4;
const int frontLeftSonar    = 5;

short int motors[2]         = { 0, 0 };

void driveFrame(void)
{
    go(motors[0], motors[1]);
}