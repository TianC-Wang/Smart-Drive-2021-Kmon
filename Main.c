#include "Kmon.h"

#pragma region /* ----- Const ----- */

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
#pragma endregion

#pragma region /* ----- Struct ----- */

/// @brief The Speed Target of Drivetrain.
struct
{
	/// @brief The Linear Ratio.
	float ratio;
	/// @brief The Target Speed.
	short int targetSpeed[2];
} lerpSpeed = { .2f, { 0, 0 } };

/// @brief A Async Function.
struct asyncFunc
{
	bool running;
	unsigned int sleep;
	void (*func)(void);
};
#pragma endregion

#pragma region /* ----- Async Function ----- */

/// @brief The Speed of Drivetrain Motors.
short int drivetrain[2]        = { 0, 0 };
/// @brief Drivetrain Speed Adjust, EVERY-FRAME CALL.
void driveFrame(void)
{
	go(drivetrain[0], drivetrain[1]);
	drivetrain[0] += (lerpSpeed.targetSpeed[0] - drivetrain[0]) * lerpSpeed.ratio;
	drivetrain[1] += (lerpSpeed.targetSpeed[1] - drivetrain[1]) * lerpSpeed.ratio;
}
/// @brief The Pool of Async Functions.
struct asyncFunc asyncFuncs[1] =
{
	{ true, 0, driveFrame }
};
#pragma endregion

#pragma region /* ----- Function ----- */

/// @brief Stop Drivetrain. 
void stopDrivetrain_HARD(void)
{
	drivetrain[0] = drivetrain[1]
	= lerpSpeed.targetSpeed[0] = lerpSpeed.targetSpeed[1] = 0;
}

/// @brief Stop Drivetrain.
void stopDrivetrain(void) { lerpSpeed.targetSpeed[0] = lerpSpeed.targetSpeed[1] = 0; }

/// @brief Set Drivetrain Speed.
/// @param _LSpeed Left Drivetrain Speed.
/// @param _RSpeed Right Drivetrain Speed.
void setDrivetrainSpeed_HARD(short _LSpeed, short _RSpeed)
{
    drivetrain[0] = lerpSpeed.targetSpeed[0] = _LSpeed;
    drivetrain[1] = lerpSpeed.targetSpeed[1] = _RSpeed;
}

/// @brief Set Drivetrain Speed.
/// @param _LSpeed Left Drivetrain Speed.
/// @param _RSpeed Right Drivetrain Speed.
void setDrivetrainSpeed(short _LSpeed, short int _RSpeed)
{
	lerpSpeed.targetSpeed[0] = _LSpeed;
	lerpSpeed.targetSpeed[1] = _RSpeed;
}

/// @brief Delay of Main Process.
/// @param _Millisec The Delay Length in Milliseconds.
void delay(unsigned int _Millisec)
{
	unsigned int i;
	unsigned int j;
	for (i = 0; i < _Millisec; i++)
	{
		for (j = 0; j < sizeof(asyncFuncs) / sizeof(struct asyncFunc); j++)
			if (asyncFuncs[j].running)
				asyncFuncs[j].func();
			else if (asyncFuncs[j].sleep > 0)
				asyncFuncs[j].sleep--;
			else if (!asyncFuncs[j].sleep)
				asyncFuncs[j].running = true;
		wait(.001f);
	}
}

void asyncFuncSleep(struct asyncFunc* _Func, unsigned int _Millisec)
{
	_Func->sleep = _Millisec;
	_Func->running = false;
}
#pragma endregion

#pragma region /* ----- Main ----- */
/// @brief Entrance of The Program.
/// @return Status Code.
int main(void) // use delay() instead of wait().
{
	return 0;
}
#pragma endregion
