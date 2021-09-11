#include "Kmon.h"
#pragma region /* ~~~~~ HEAD ~~~~~ */
#pragma region /* === DEFINITION & CONSTANT === */
#pragma region /* --- DEFINITION --- */
#define null ((void*)0)
typedef unsigned char byte;
typedef const byte port_t;
typedef void* object;
typedef void (*action)(object);
typedef unsigned int frame_t;
#pragma endregion
#pragma region /* --- CONSTANT --- */
port_t gsL3 = (port_t)1;
port_t gsL2 = (port_t)2;
port_t gsL1 = (port_t)3;
port_t gsR1 = (port_t)4;
port_t gsR2 = (port_t)5;
port_t gsR3 = (port_t)6;
port_t disR = (port_t)3;
port_t disL = (port_t)4;
port_t disF = (port_t)5;
int (*const getGrayscale)(int) = geteadc;
int (*const getDistance)(int) = getadc;
#pragma endregion
#pragma endregion
#pragma region /* === UTILITY === */
#pragma region /* --- MATH --- */
float absf(float _Num) { return _Num < 0 ? -_Num : _Num; }
float rangef(float _Num, float _Max, float _Min)
{
    if (_Num > _Max) return _Max;
    if (_Num < _Min) return _Min;
    return _Num;
}
#pragma endregion
#pragma region /* --- LIST --- 
struct list
{
    struct list* last;
    struct list* next;
    object content;
};
struct list* list_new(struct list* _Last, struct list* _Next, object _Content)
{
    struct list* newList = malloc(sizeof(struct list));
    memset(newList, 0, sizeof(newList));
    newList->last = _Last;
    newList->next = _Next;
    newList->content = _Content;
}
void list_delete(struct list* _List, bool _DeleteContent)
{
    if (_DeleteContent && _List->content)
        free(_List->content);
    if (_List->next)
        _List->next->last = _List->last;
    if (_List->last)
        _List->last->next = _List->next;
    free(_List);
}
struct list* list_head(struct list* _List)
{
    while (_List->last)
        _List = _List->last;
    return _List;
}
struct list* list_tail(struct list* _List)
{
    while (_List->next)
        _List = _List->next;
    return _List;
}
struct list* list_get(struct list* _List, int _Offset)
{
    while (_Offset > 0)
    {
        _List = _List->next;
        _Offset--;
    }
    while (_Offset < 0)
    {
        _List = _List->last;
        _Offset++;
    }
    return _List;
}
void list_clear(struct list* _List, bool _DeleteContent)
{
    while (_List->next)
        list_delete(_List->next, _DeleteContent);
    while (_List->last)
        list_delete(_List->last, _DeleteContent);
}
struct list* list_appendAfter(struct list* _List, struct list* _New)
{
    if (_New->next = _List->next)
        _New->next->last = _New;
    _List->next = _New;
    _New->last = _List;
    return _New;
}
struct list* list_appendBefore(struct list* _List, struct list* _New)
{
    if (_New->last = _List->last)
        _New->last->next = _New;
    _List->last = _New;
    _New->next = _List;
    return _New;
}
void list_forEach(struct list* _List, action _CallBack)
{
    _List = list_head(_List);
    while (_List)
    {
        _CallBack(_List->content);
        _List = _List->next;
    }
} */
#pragma endregion
#pragma endregion
#pragma region /* === ASYNC === */
#pragma region /* --- BASIS --- */
struct task
{
    action act;
    object param;
};
#pragma endregion
#pragma region /* --- DRIVE --- */
struct driveParam
{
    bool drive;
    float leftSpeed;
    float rightSpeed;
    float ratio;
    float ratioRate;
    float ratioTarget;
};
void driveAsync(object _Param)
{
    struct driveParam* param = (struct driveParam*)_Param;
    go(param->leftSpeed * param->ratio, param->rightSpeed * param->ratio);
    if (param->drive)
    {
        if (absf(param->ratio - param->ratioTarget) > param->ratioRate)
            param->ratio += param->ratio > param->ratioTarget ? -param->ratioRate : param->ratioRate;
        else
            param->ratio = param->ratioTarget;
    }
}
void setSpeed(float _LSpeed, float _RSpeed, struct driveParam* _Param)
{
    _Param->ratio *= absf(_Param->leftSpeed + _Param->rightSpeed) / absf(_LSpeed + _RSpeed);
    _Param->ratioTarget = 1.f;
    _Param->leftSpeed = _LSpeed;
    _Param->rightSpeed = _RSpeed;
}
#pragma endregion
#pragma region /* --- WALL TRACK --- */
struct wallTrackParam
{
    port_t sensor;
    bool run;
    float base;
    float target;
    float error;
    float errorPrevious;
    float errorSum;
    float proportional;
    float integal;
    float differencial;
    struct driveParam* driver;
};
void wallTrackAsync(object _Param)
{
    struct wallTrackParam* param = (struct wallTrackParam*)_Param;
    struct driveParam* driver = param->driver;
    if (driver->drive && param->run)
    {
        param->error = rangef(getDistance(param->sensor) - param->target, 100, -100);
        driver->leftSpeed = param->base - param->proportional * param->error - param->integal * param->errorSum - param->differencial * (param->error - param->errorPrevious);
        driver->rightSpeed = param->base + param->proportional * param->error + param->integal * param->errorSum + param->differencial * (param->error - param->errorPrevious);
        param->errorPrevious = param->error;
        param->errorSum += param->error;
    }
}
#pragma endregion
#pragma region /* --- SAFE --- */
struct safeParam
{
    bool run;
    float safeDistance;
    float ratio;
    struct driveParam* driver;
};
void safeAsync(object _Param)
{
    struct safeParam* param = (struct safeParam*)_Param;
    struct driveParam* driver = param->driver;
    if (param->run && !(driver->drive = getDistance(disF) <= param->safeDistance))
        if (absf(driver->ratio) > param->ratio)
            if (driver->ratio > .0f)
                driver->ratio -= param->ratio;
            else
                driver->ratio += param->ratio;
        else
            driver->ratio = .0f;
    else
        driver->drive = true;
}
#pragma endregion
#pragma region /* --- GRAYSCALE --- */
struct grayscaleValue
{
    bool l3;
    bool l2;
    bool l1;
    bool r1;
    bool r2;
    bool r3;
    float thresL3;
    float thresL2;
    float thresL1;
    float thresR1;
    float thresR2;
    float thresR3;
};
void grayscaleAsync(object _Value)
{
    struct grayscaleValue* value = (struct grayscaleValue*)_Value;
    value->l3 = getGrayscale(gsL3) < value->thresL3;
    value->l2 = getGrayscale(gsL2) < value->thresL2;
    value->l1 = getGrayscale(gsL1) < value->thresL1;
    value->r1 = getGrayscale(gsR1) < value->thresR1;
    value->r2 = getGrayscale(gsR2) < value->thresR2;
    value->r3 = getGrayscale(gsR3) < value->thresR3;
}
#pragma endregion
#pragma endregion

#pragma region /* ~~~~~ MAIN ~~~~~ */

int main(void)
{
    struct driveParam dp;
    struct wallTrackParam wtp = { disR };
    struct safeParam sp;
    struct grayscaleValue gv;
    dp.ratio = .0f;
    dp.ratioRate = .0025f;
    dp.ratioTarget = 1.f;
    wtp.base = 500.f;
    wtp.run = true;
    wtp.differencial = 180.f;
    wtp.integal = .0002f;
    wtp.proportional = .8f;
    wtp.target = 500.f;
    wtp.driver = &dp;
    sp.run = true;
    sp.ratio = .005f;
    sp.safeDistance = 270.f;
    sp.driver = &dp;
    gv.thresL3 = 220.f;
    gv.thresL2 = 300.f;
    gv.thresL1 = 300.f;
    gv.thresR1 = 400.f;
    gv.thresR2 = .0f;
    gv.thresR3 = 280.f;
    bool l, r;
#define UPDATE { driveAsync(&dp); wallTrackAsync(&wtp); safeAsync(&sp); grayscaleAsync(&gv); wait(.001); }
#define WAITUNTIL(__CONDITION__) while (!(__CONDITION__)) UPDATE
#define WAITMSEC(__MILLISEC__) for (int i = 0; i < __MILLISEC__; i++) UPDATE
#define DISABLEWALLTRACK wtp.run = false;
#define ENABLEWALLTRACK wtp.run = true; dp.ratioTarget = 1.f;
#define DISABLESAFE sp.run = false;
#define ENABLESAFE sp.run = true;
#define TURNRIGHT setSpeed(530.f, 265.f, &dp); DISABLEWALLTRACK WAITUNTIL(getDistance(disR) < 200.f) WAITMSEC(800) WAITUNTIL(getDistance(disR) > 350.f && getDistance(disF) < 100.f) WAITMSEC(200) wtp.target = rangef(getDistance(disR), 500, 300); ENABLEWALLTRACK
#define TURNLEFT setSpeed(340.f, 510.f, &dp); DISABLEWALLTRACK WAITUNTIL(getDistance(disR) < 200.f) WAITMSEC(800) WAITUNTIL(getDistance(disR) > 360.f && getDistance(disF) < 100.f) WAITMSEC(300) wtp.target = rangef(getDistance(disR), 500, 300); ENABLEWALLTRACK
#define TURNFORWARD setSpeed(450.f, 455.f, &dp); DISABLEWALLTRACK WAITUNTIL(getDistance(disR) < 200.f) WAITMSEC(800) WAITUNTIL(getDistance(disR) > 300.f && getDistance(disF) < 100.f) WAITMSEC(200) wtp.target = rangef(getDistance(disR), 500, 300); ENABLEWALLTRACK
#define STOP DISABLEWALLTRACK dp.ratioTarget = .0f; WAITUNTIL(!(int)(dp.leftSpeed*dp.ratio) && !(int)(dp.rightSpeed*dp.ratio)) wtp.target = rangef(getDistance(disR), 500, 300); dp.ratioTarget = 1.f; ENABLEWALLTRACK
#define LEFT DISABLEWALLTRACK dp.leftSpeed = -400.f; dp.rightSpeed = 400.f; dp.ratioTarget = 1.f; WAITMSEC(470) ENABLEWALLTRACK
#define RIGHT DISABLEWALLTRACK dp.leftSpeed = 400.f; dp.rightSpeed = 400.f; dp.ratioTarget = 1.f; WAITMSEC(470) ENABLEWALLTRACK
#define ALIGNLINE l = false; r = false; WAITUNTIL(!(gv.l1||gv.l2||gv.l3||gv.r1||gv.r2||gv.r3)) WAITUNTIL(gv.l1||gv.l2||gv.l3||gv.r1||gv.r2||gv.r3) DISABLEWALLTRACK go(-200, -200); while (!(l && r)) { if (gv.l1) l = true; if (gv.r1) r = true; if (l) motor(1, 0); if (r) motor(2, 0); UPDATE } ENABLEWALLTRACK
#define ARMDOWN servo(2, 1800);
#define ARMUP servo(2, 0);
#define GRAB servo(1, 1250);
#define RELEASE servo(1, 450);
#define BLINK setport(1, 1); wait(.4); setport(1, 0); wait(.4);
    STOP
    ARMUP
    GRAB
    WAITMSEC(1000);
    ALIGNLINE
    TURNRIGHT
    ALIGNLINE
    TURNRIGHT
    DISABLESAFE
    WAITUNTIL(getDistance(disF) > 260.f)
    STOP
    LEFT
    STOP
    WAITUNTIL(getDistance(disF) > 270.f)
    STOP
    BLINK BLINK BLINK
    LEFT
    STOP
    ENABLESAFE
    ALIGNLINE
    TURNLEFT
    ALIGNLINE
    TURNFORWARD
    ALIGNLINE
    TURNLEFT
    DISABLESAFE
    WAITUNTIL(getDistance(disF) > 290.f)
    STOP
    LEFT
    STOP
    WAITMSEC(300)
    STOP
    DISABLEWALLTRACK
    dp.ratioTarget = .0f;
    RELEASE
    ARMDOWN
    WAITMSEC(1000)
    GRAB
    WAITMSEC(1000)
    ARMUP
    ENABLEWALLTRACK
    WAITUNTIL(getDistance(disF) > 300.f)
    STOP
    LEFT
    STOP
    ENABLESAFE
    ENABLEWALLTRACK
    ALIGNLINE
    TURNFORWARD
    STOP
    dp.ratioTarget = .0f;
    BLINK BLINK BLINK BLINK BLINK
    ENABLEWALLTRACK
    ALIGNLINE
    TURNRIGHT
    ALIGNLINE
    TURNLEFT
    ALIGNLINE
    TURNLEFT
    WAITUNTIL(getDistance(disF) > 270.f)
    STOP
    dp.ratioTarget = .0f;
    WAITUNTIL(false)
}
#pragma endregion

