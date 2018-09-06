/*
* @Brief Porvide time measurement (unit: s)
*/
#ifndef TIMER_H
#define TIMER_H

#include <sys/time.h>

typedef struct 
{
    struct timeval startTime;
    struct timeval endTime;
} Timer;

void startTime(Timer* timer);
void stopTime(Timer* timer);
float elapsedTime(Timer timer);

#endif // TIMER_H
