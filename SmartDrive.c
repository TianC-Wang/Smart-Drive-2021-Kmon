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

/* ----- Function Declaration ----- */

// @brief A Frame for Drivetrain.
void driveFrame(void);

// @brief Stop Drivetrain. 
void stop(void);

// @brief Set Drivetrain Speed.
// @param _LSpeed Left Drivetrain Speed.
// @param _RSpeed Right Drivetrain Speed.
void setSpeed(short _LSpeed, short _RSpeed);

// @brief Entrance of The Program.
// @return Status Code.
int main(void);

/* ---- Functions Definition ----- */

void driveFrame(void) { go(motors[0], motors[1]); }

void stop(void)
{
    motors[0] = 0;
    motors[1] = 0;
    driveFrame();
}

void setSpeed(short _LSpeed, short _RSpeed)
{
    motors[0] = _LSpeed;
    motors[1] = _RSpeed;
    driveFrame();
}

int main(void)
{
    go(250, 250);
    wait(3.f);
    stop();
    return 0;
}