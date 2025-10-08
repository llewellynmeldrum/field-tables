#include "mach_time_helper.h"
#include <stdint.h>
#include <time.h>
double get_clock_ghz(ssize_t busywait_count) {
	uint64_t ns_start = get_cputime_ns();
	busywait_n_cycles(busywait_count);
	uint64_t ns_elapsed = get_cputime_ns() - ns_start;
	double measured_ghz = (double)busywait_count / ns_elapsed;
	return measured_ghz;
}

/* this only works on arm64 systems obviously
i wonder how portable a simple tight loop would be, such as:
	 int a = busywait_count;
	 label:
	 --a;
	 if (a) goto label;
worth trying on godbolt
 */

void busywait_n_cycles(ssize_t busywait_count) {
	__asm__ volatile(
	    "1:\n\t"
	    "subs %w0, %w0, #1\n\t"
	    "b.ne 1b\n\t;"
	    :"+r"(busywait_count)		/* read+write operand */
	    :
	    :"cc"		/* condition codes get clobbered*/
	);
	if (busywait_count) perror("Busywait failed");
}
