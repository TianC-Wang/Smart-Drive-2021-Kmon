#include "Kmon.h"

/* ----- Define ----- */
#define getGrayscale(index)   geteadc(index)
#define getDistance(index)    getadc(index)
#define bool                  char
#define true                  1
#define false                 0

/* ----- Const ----- */
const int left3rdGrayscale  = 1;
const int left2ndGrayscale  = 2;
const int left1stGrayscale  = 3;
const int right1stGrayscale = 4;
const int right2ndGrayscale = 5;
const int right3rdGrayscale = 6;
const int rightSonar        = 3;
const int leftSonar         = 4;
const int frontLeftSonar    = 5;

/* ----- Array ----- */
short int motors[2]         = { 0, 0 };

/* ----- Function ----- */
void driveFrame(void) { go(motors[0], motors[1]); }