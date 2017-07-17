#ifndef DATA_H
#define DATA_H
#include "STC15F2K60S2.H"

#define ModeShifting_Button 1
#define InterfaceShifting_Button 3
#define SecondAdd_Button 4
#define SecondCut_Button 5
#define MSecondAdd_Button 6
#define MSecondCut_Button 7
#define MinAdd_Button 8
#define MinCut_Button 9
#define HoursAdd_Button 10
#define HoursCut_Button 11

typedef enum
{
    Timer,
    Clock,
    StopWatch,

    MAX_Mode
}mode;

typedef enum
{
    Start,
    Setting,

    MAX_Interface
}interface;

typedef struct
{
    int H;
    int M;
    int S;
    int MS;
}time;

typedef struct
{
    char CurrentInterface;
    time CurrentTime;
}clock;


typedef struct
{
    char CurrentMode;
    clock Clock;
    clock Timer;
    clock StopWatch;
}rootclock;

extern rootclock ClockBase;
#endif //DATA_H