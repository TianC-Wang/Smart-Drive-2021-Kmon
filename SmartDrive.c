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
/// @brief Get The Grayscale Value of a Sensor.
/// @param _Index The Target Sensor Index.
/// @return Grayscale Value(0~1023).
int (*const getGrayscale)(int _Index) = geteadc;
/// @brief Get The Distance Value of a Sonar.
/// @param _Index The Target Sonar Index.
/// @return Distance Value(0~1023).
int (*const getDistance)(int _Index)  = getadc;
#pragma endregion

#pragma region /* ----- Header ----- */
#define waitUntil(__CONDI) \
while (!(__CONDI)) frame()

/// @brief The Driver Function Index.
unsigned int driver;

/// @brief The Wall Tracker Function Index.
unsigned int walltracker;

/// @brief The Safe Guard Function Index.
unsigned int safeguard;

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

/// @brief The Pool of The Async Function.
struct asyncFunc asyncFuncPool[5] =
{
    { 0, false, 0 },
    { 0, false, 0 },
    { 0, false, 0 },
    { 0, false, 0 },
    { 0, false, 0 }
};

/// @brief Get The Absolute Value.
/// @param _Num The Number.
float absf(float _Num)
{
    if (_Num > 0)
        return _Num;
    return -_Num;
}

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
} drivetrain = { .0f, .0f, .0f, .0f, .005f };

/// @brief Wall Tracking Parameters.
struct
{
    /// @brief Target Position.
    float tgt;
    /// @brief Error.
    float err;
    /// @brief Previous Error.
    float preErr;
    /// @brief Proportional Parameter.
    float kp;
    /// @brief Integal Parameter.
    float ki;
    /// @brief Differencial Parameter.
    float kd;
    /// @brief Base Speed.
    float baseSpeed;
} walltracking = { 500.f, .0f, .0f, 1.2f, .0f, 100.f, 500.f };

/// @brief Turning Parameters.
struct
{
    float speedRatio;
    float wallTach;
} turning = { 13.f, 370.f };


struct
{
    unsigned int safeDistance;
} safeoptions = { 500 };



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

/// @brief Clean an Async Function in The Pool.
/// @param _Index The Function Index.
void cleanAsyncFunc(unsigned int _Index)
{
    asyncFuncPool[_Index].isRunning = false;
    asyncFuncPool[_Index].sleepFrames = 0;
    asyncFuncPool[_Index].function = 0;
}

/// @brief Stop an Async Function in The Pool.
/// @param _Index The Function Index.
void stopAsyncFunc(unsigned int _Index)
{
    asyncFuncPool[_Index].isRunning = false;
}

void stopAllAsyncFunc(void)
{
    unsigned int i;
    for (i = 0; i < 5; i++)
        asyncFuncPool[i].isRunning = false;
}

/// @brief Resume an Async Function in The Pool.
/// @param _Index The Function Index.
void resumeAsyncFunc(unsigned int _Index)
{
    asyncFuncPool[_Index].isRunning = true;
}

/// @brief Sleep an Async Function in The Pool.
/// @param _Index The Function Index.
/// @param _FrameCount The Sleep Frame Count.
void sleepAsyncFunc(unsigned int _Index, unsigned int _FrameCount)
{
    asyncFuncPool[_Index].sleepFrames = _FrameCount;
}

/// @brief Change Drivetrain Speed Smoothly.
/// @param _LSpeed Left Speed.
/// @param _RSpeed Right Speed.
void setSpeed(float _LSpeed, float _RSpeed)
{
    if (!((int)_LSpeed || (int)_RSpeed))
        stopAsyncFunc(safeguard);
    else
        resumeAsyncFunc(safeguard);
    stopAsyncFunc(walltracker);
    resumeAsyncFunc(driver);
    drivetrain.lTgtSpeed = _LSpeed;
    drivetrain.rTgtSpeed = _RSpeed;
}

/// @brief Change Drivetrain Speed Directly.
/// @param _LSpeed Left Speed.
/// @param _RSpeed Right Speed.
void setSpeed_HARD(float _LSpeed, float _RSpeed)
{
    if (!((int)_LSpeed || (int)_RSpeed))
        stopAsyncFunc(safeguard);
    else
        resumeAsyncFunc(safeguard);
    stopAsyncFunc(walltracker);
    resumeAsyncFunc(driver);
    drivetrain.lSpeed = drivetrain.lTgtSpeed = _LSpeed;
    drivetrain.rSpeed = drivetrain.rTgtSpeed = _RSpeed;
}

/// @brief Change Wall Tracking Speed Smoothly.
/// @param _Speed The Speed.
void setWallTrackingSpeed(float _Speed)
{
    walltracking.baseSpeed = _Speed;
    resumeAsyncFunc(walltracker);
}

/// @brief Change Wall Tracking Speed Directly.
/// @param _Speed The Speed.
void setWallTrackingSpeed_HARD(float _Speed)
{
    setSpeed_HARD(_Speed, _Speed);
    walltracking.baseSpeed = _Speed;
    resumeAsyncFunc(walltracker);
}

/// @brief Sync Double Side Drivetrain Speed Smoothly.
void syncSpeed(void)
{
    setSpeed((drivetrain.lSpeed + drivetrain.rSpeed) / 2.f, (drivetrain.lSpeed + drivetrain.rSpeed) / 2.f);
}

/// @brief Sync Double Side Drivetrain Speed Directly.
void syncSpeed_HARD(void)
{
    setSpeed_HARD((drivetrain.lSpeed + drivetrain.rSpeed) / 2.f, (drivetrain.lSpeed + drivetrain.rSpeed) / 2.f);
}

/// @brief Output Speed.
/// @param _This The Current Async Function Index.
void driveAsync(unsigned int _This)
{
    go((int)drivetrain.lSpeed, (int)drivetrain.rSpeed);
    if (!(drivetrain.lSpeed - drivetrain.lTgtSpeed || drivetrain.rSpeed - drivetrain.rTgtSpeed))
        stopAsyncFunc(_This);
    if (absf(drivetrain.lSpeed - drivetrain.lTgtSpeed) < 20.f)
        drivetrain.lSpeed = drivetrain.lTgtSpeed;
    else
        drivetrain.lSpeed += (drivetrain.lTgtSpeed - drivetrain.lSpeed) * drivetrain.ratio;
    if (absf(drivetrain.rSpeed - drivetrain.rTgtSpeed) < 20.f)
        drivetrain.rSpeed = drivetrain.rTgtSpeed;
    else
        drivetrain.rSpeed += (drivetrain.rTgtSpeed - drivetrain.rSpeed) * drivetrain.ratio;
}

/// @brief Track Wall.
/// @param _This The Current Async Function Index.
void wallTrackAsync(unsigned int _This)
{
    walltracking.err = getDistance(snR) - walltracking.tgt;
    setSpeed(walltracking.baseSpeed - 1.6f * walltracking.err * walltracking.kp + (walltracking.preErr - walltracking.err) * walltracking.kd * .25f,
        walltracking.baseSpeed + .4f * walltracking.err * walltracking.kp - (walltracking.preErr - walltracking.err) * walltracking.kd * 1.75f);
    walltracking.preErr = walltracking.err;
    resumeAsyncFunc(walltracker);
}

/// @brief Turn Left.
/// @param _This The Current Async Function Index.
void turnLeftAsync(unsigned int _This)
{
    static unsigned char step = 0;
    switch(step)
    {
        case 0:
            syncSpeed_HARD();
            setSpeed(turning.speedRatio * 22.f, turning.speedRatio * 33.f);
            step++;
            break;
        case 1:
            if (getDistance(snR) <= 200.f)
                step++;
            break;
        case 2:
            if (getDistance(snR) >= turning.wallTach)
                step++;
            break;
        case 3:
            stopAsyncFunc(_This);
            cleanAsyncFunc(_This);
            step = 0;
            break;
    }
}

/// @brief Turn Right.
/// @param _This The Current Async Function Index.
void turnRightAsync(unsigned int _This)
{
    static unsigned char step = 0;
    switch(step)
    {
        case 0:
            syncSpeed_HARD();
            setSpeed(turning.speedRatio * 32.f, turning.speedRatio * 18.f);
            step++;
            break;
        case 1:
            if (getDistance(snR) <= 200.f)
                step++;
            break;
        case 2:
            if (getDistance(snR) >= turning.wallTach)
                step++;
            break;
        case 3:
            stopAsyncFunc(_This);
            cleanAsyncFunc(_This);
            step = 0;
            break;
    }
}

/// @brief QR Code Monitor.
/// @param _This The Current Async Function Index.
void monitorAsync(unsigned int _This)
{
}

/// @brief Safe Guard.
/// @param _This The Current Async Function Index.
void safeAsync(unsigned int _This)
{
    if (getDistance(snF) > safeoptions.safeDistance)
    {
        go(0, 0);
        while (getDistance(snF) > safeoptions.safeDistance)
            wait(.001f);
        drivetrain.lSpeed = drivetrain.rSpeed = 0;
        resumeAsyncFunc(driver);
    }
}

/// @brief Grab Object.
/// @param _This The Current Async Function Index.
void grabAsync(unsigned int _This)
{
}

/// @brief Release Object.
/// @param _This The Current Async Function Index.
void releaseAsync(unsigned int _This)
{
}
#pragma endregion

#pragma region /* ----- Main ----- */
void start(void)
{
    driver = startAsyncFunc(driveAsync);
    walltracker = startAsyncFunc(wallTrackAsync);
    safeguard = startAsyncFunc(safeAsync);
}

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

/// @brief Wait for a Function's Completion.
/// @param _Index The Function Index.
void waitForComplete(unsigned int _Index)
{
    while (asyncFuncPool[_Index].isRunning)
        frame();
}

/// @brief Wait for a Button Click.
void waitForClick(void)
{
    while (!trigger()) frame();
    while (trigger()) frame();
}

/// @brief Wait The Sonar to Cross a Threshold Value.
/// @param _Index The Sonar Index.
/// @param _Val The Threshold Value.
void waitForSonarPass(unsigned int _Index, float _Val)
{
    stopAsyncFunc(safeguard);
    if (getDistance(_Index) > _Val)
        while(getDistance(_Index) > _Val)
            frame();
    else
        while(getDistance(_Index) < _Val)
            frame();
    resumeAsyncFunc(safeguard);
}

/// @brief Wait The Grayscale to Cross a Threshold Value.
/// @param _Index The Grayscale Index.
/// @param _Val The Threshold Value.
void waitForGrayscalePass(unsigned int _Index, float _Val)
{
    if (getGrayscale(_Index) > _Val)
        while(getGrayscale(_Index) > _Val)
            frame();
    else
        while(getGrayscale(_Index) < _Val)
            frame();
}

/// @brief Program Entrance.
/// @return Don't Matter. lol.
int main(void)
{
    unsigned int i;
    while (true)
    {
        for (i = 0; i < 5; i++)
            cleanAsyncFunc(i);
        cls();
        printf("\n\n\n那信仰与依恋之间的，\n便是你，公主，\n艾瑟依拉姆\n薇瑟\n艾莉欧斯亚。");
        waitForClick();
        cls();
        start();

        /* ---------- */
        
        waitForComplete(startAsyncFunc(turnRightAsync));
        setWallTrackingSpeed(300.f);
        waitForSonarPass(snF, 320.f);
        setSpeed(.0f, .0f);
        waitForComplete(driver);
        
        waitForClick();

        waitForComplete(startAsyncFunc(turnLeftAsync));
        setWallTrackingSpeed(300.f);
        waitForSonarPass(snF, 320.f);
        setSpeed(.0f, .0f);
        waitForComplete(driver);

        /* ---------- */

        cls();
        printf("\n\n\n——蓝色的玫瑰，\n    折射的天空，\n消散的，骑士，\n斯雷因\n特洛耶特。");
        waitForClick();
        cls();
    }
}
#pragma endregion
