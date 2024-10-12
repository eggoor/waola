#pragma once

#define NS_IN_SEC 1000000000ull
#define MS_IN_NS 1000000
#define SEC_IN_HOUR 3600
#define SEC_IN_MIN 60

typedef struct stopwatch stopwatch_t;

stopwatch_t* stopwatch_make(void);
void stopwatch_free(stopwatch_t* const restrict sw);
void stopwatch_start(stopwatch_t* const restrict sw);
unsigned long long int stopwatch_restart(stopwatch_t* const restrict sw);
unsigned long long int stopwatch_lap(stopwatch_t* const restrict sw);
void print_elapsed(unsigned long long int elapsed);
