#include "Kmon.h"

/* ----- Const ----- */

/// @brief The 3rd Grayscale of Left.
const int gsL3                        = 1;
/// @brief The 2nd Grayscale of Left.
const int gsL2                        = 2;
/// @brief The 1st Grayscale of Left.
const int gsL1                        = 3;
/// @brief The 1st Grayscale of Right.
const int gsR1                        = 4;
/// @brief The 2nd Grayscale of Right.
const int gsR2                        = 5;
/// @brief The 3rd Grayscale of Right.
const int gsR3                        = 6;
/// @brief The Sonar of Right.
const int snR                         = 3;
/// @brief The Sonar of Left.
const int snL                         = 4;
/// @brief The Sonar of Front.
const int snF                         = 5;
/// @brief The Expected Position of The Cart on Road.
const int expect                      = 550;
/// @brief The Proportion The Cart Clear its Offset from The Expected Position on Road.
const int kp                          = 1.8;
/// @brief Get The Grayscale Value of a Sensor.
/// @param index The Target Sensor Index.
/// @return Grayscale Value(0~1023).
int (*const getGrayscale)(int index)  = geteadc;
/// @brief Get The Distance Value of a Sonar.
/// @param index The Target Sonar Index.
/// @return Distance Value(0~1023).
int (*const getDistance)(int index)   = getadc;

/* ----- Array ----- */

/// @brief The Speed of Drivetrain Motors.
short int driveTrain[2]  = { 0, 0 };

/* ---- Function ----- */

/// @brief A Frame for Drivetrain.
void driveFrame(void) { go(driveTrain[0], driveTrain[1]); }

/// @brief Stop Drivetrain. 
void stopDrivetrain(void)
{
    driveTrain[0] = 0;
    driveTrain[1] = 0;
    driveFrame();
}

/// @brief Set Drivetrain Speed.
/// @param _LSpeed Left Drivetrain Speed.
/// @param _RSpeed Right Drivetrain Speed.
void setDrivetrainSpeed(short _LSpeed, short _RSpeed)
{
    driveTrain[0] = _LSpeed;
    driveTrain[1] = _RSpeed;
    driveFrame();
}

/// @brief Entrance of The Program.
/// @return Status Code.
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

