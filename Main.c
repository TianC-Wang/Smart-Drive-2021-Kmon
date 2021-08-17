#include "Kmon.h"

/* ----- Define ----- */

#define getGrayscale(index)   geteadc(index)
#define getDistance(index)    getadc(index)

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

const int expect            = 550;
const int kp                = 1.8;

/* ----- Array ----- */

short int motors[2]         = { 0, 0 };

/* ---- Function ----- */

// @brief A Frame for Drivetrain.
void driveFrame(void) { go(motors[0], motors[1]); }

// @brief Stop Drivetrain. 
void stopDrivetrain(void)
{
    motors[0] = 0;
    motors[1] = 0;
    driveFrame();
}

// @brief Set Drivetrain Speed.
// @param _LSpeed Left Drivetrain Speed.
// @param _RSpeed Right Drivetrain Speed.
void setDrivetrainSpeed(short _LSpeed, short _RSpeed)
{
    motors[0] = _LSpeed;
    motors[1] = _RSpeed;
    driveFrame();
}

// @brief Entrance of The Program.
// @return Status Code.
int main(void) {
	printf("³É¹¦£¡");
	while (1) {
		/*
		cls();
		printf("%3d, %3d, %3d, %3d, %3d, %3d\n%3d, %3d, %3d", geteadc(1), geteadc(2), geteadc(3),
			geteadc(4), geteadc(5), geteadc(6), getadc(3), getadc(4), getadc(5));
		*/
		int adc3 = getadc(3);
		int p = expect - adc3;
		if (p > 100) p = 100;
		motor(1, p * kp + 500);
		motor(2, p * -kp + 500);
	}
	return 0;
}

