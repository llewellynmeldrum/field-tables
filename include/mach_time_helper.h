#ifndef MACH_TIME_HELPER_H
#define MACH_TIME_HELPER_H

#include <stdio.h>

#define get_cputime_ns() clock_gettime_nsec_np(CLOCK_PROCESS_CPUTIME_ID)
#define get_realtime_ns() clock_gettime_nsec_np(CLOCK_REALTIME)
double get_clock_ghz(ssize_t busywait_count);
void busywait_n_cycles(ssize_t busywait_count);

#define stoms(seconds)	((double)seconds) * 1'000.0
#define stous(seconds)	((double)seconds) * 1'000'000.0
#define stons(seconds)	((double)seconds) * 1'000'000'000.0
#define mstos(miilis) 	((double)millis ) / 1'000.0
#define mstous(millis)	((double)millis ) * 1'000.0
#define mstons(millis)	((double)millis )* 1'000'000.0
#define ustos(micros) 	((double)micros ) / 1'000'000.0
#define ustoms(micros)	((double)micros ) / 1'000.0
#define ustons(micros)	((double)micros ) * 1'000.0
#define nstos(nanos)  	((double)nanos  ) / 1'000'000'000.0
#define nstoms(nanos) 	((double)nanos  ) / 1'000'000.0
#define nstous(nanos) 	((double)nanos  ) / 1'000.0

#endif // MACH_TIME_HELPER_H
