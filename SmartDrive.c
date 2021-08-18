#include "Kmon.h"

#pragma region /* ----- Const Values ----- */
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

#pragma region /* ----- Header ----- */
/// @brief Drivetrain Speeds.
struct
{
    /// @brief Left Outputting Speed.
    float lSpeed;
    /// @brief Right Outputting Speed.
    float rSpeed;
    /// @brief Left Target Speed.
    float lTgtSpeed;
    /// @brief Right Target Speed.
    float rTgtSpeed;
    /// @brief Speed Follow Ratio.
    float ratio;
} drivetrain = { 0, 0, 0, 0, .0025f };

/// @brief A Unit of an Async Function.
struct asyncFunc
{
    /// @brief The Time Before The Next Run.
    unsigned int sleepFrames;
    /// @brief Whether This Function is Running.
    bool         isRunning;
    /// @brief The Function.
    /// @param _This The Current Async Function Index.
    void       (*function)(unsigned int _This);
};

/// @brief Output Speed.
/// @param _This The Current Async Function Index.
void driveAsync(unsigned int _This)
{
    go((int)drivetrain.lSpeed, (int)drivetrain.rSpeed);
    drivetrain.lSpeed += (drivetrain.lTgtSpeed - drivetrain.lSpeed) * drivetrain.ratio;
    drivetrain.rSpeed += (drivetrain.rTgtSpeed - drivetrain.rSpeed) * drivetrain.ratio;
}

/// @brief The Pool of The Async Function.
struct asyncFunc asyncFuncPool[5] =
{
    { 0, false, 0 },
    { 0, false, 0 },
    { 0, false, 0 },
    { 0, false, 0 },
    { 0, false, 0 }
};

/// @brief Start an Async Function to The Pool.
/// @param _Func The Target Function.
/// @return The Index of The Function in Pool
unsigned int startAsyncFunc(void (*_Func)(unsigned int))
{
    unsigned int i;
    for (i = 0; i < 5; i++)
        if (!asyncFuncPool[i].function)
        {
            asyncFuncPool[i].function = _Func;
            asyncFuncPool[i].isRunning = true;
            asyncFuncPool[i].sleepFrames = 0;
            return i;
        }
}
#pragma endregion

#pragma region /* ----- Main ----- */
/// @brief Run a Frame.
void frame(void)
{
    unsigned int i;
    for (i = 0; i < sizeof(asyncFuncPool) / sizeof(struct asyncFunc); i++)
        if (asyncFuncPool[i].isRunning)
            if (asyncFuncPool[i].sleepFrames)
                asyncFuncPool[i].sleepFrames--;
            else
                asyncFuncPool[i].function(i);
    wait(.001f);
}

/// @brief Wait for Some Frames.
/// @param _FrameCount Frame Count.
void waitForFrames(unsigned int _FrameCount)
{
    unsigned int i;
    for (i = 0; i < _FrameCount; i++)
        frame();
}

/// @brief Program Entrance.
/// @return Don't Matter. lol.
int main(void)
{
    startAsyncFunc(driveAsync);
    drivetrain.lTgtSpeed = 400;
    drivetrain.rTgtSpeed = 400;
    waitForFrames(2000);
    drivetrain.lTgtSpeed = 0;
    drivetrain.rTgtSpeed = 0;
    waitForFrames(1000);
    printf("Done!");
    return 0;
}
#pragma endregion
