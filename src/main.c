#include <math.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include <mach/mach.h>
#include <mach/thread_policy.h>

#include "field_tables.h"
#include "pretty_printing.h"
#include "mach_time_helper.h"

double heron_sqrt(int num);
#define rand_range(min,max) min+(rand()%(max-min));

#define BUSYWAIT_COUNT 		100'000'000.0
#define SQRT_TEST_MAX 		1'000'000
#define NUM_REPS 		10
#define NUM_ITERATIONS 		1'000
#define NUM_WARMUP_REPS 	10
#define NUM_WARMUP_ITERATIONS 	10'000

int main(int argc, char* argv[argc + 1]) {
	uint64_t realtime_start_ns = get_realtime_ns();
	uint64_t cputime_start_ns = get_cputime_ns();


	// warmup the thread
	for (int run = 0; run < NUM_WARMUP_REPS; ++run) {
		for (int run = 0; run < NUM_WARMUP_ITERATIONS; ++run) {
			int r = rand_range(0, 10000);
			double heron = heron_sqrt(r);
			double real = sqrt(r);
		}
	}

	double clock_ghz = get_clock_ghz(BUSYWAIT_COUNT);
	printf_banner("\x1b[33;1m", "COMPLETED WARMUP, CPU@%0.2lfGHz", clock_ghz);
	printf_center("\x1b[32;2m", "Testing with random sqrts up to %d.", SQRT_TEST_MAX);
	printf("\n");
	printf_banner("\x1b[32;1m", "BENCHMARKING ACCURACY");
	printf_center("\x1b[32;2;3m", "(subsequent calls of both type)");


	FieldTable_t results = new_FieldTable("Table");
	Field_t *TEST_NAME = 		new_Field("Test name", DT_CSTRING, "%s", true, 0);
	Field_t *MS_OVERALL = 		new_Field("Time elapsed", DT_DOUBLE, "%0.2lfms", false, 0);
	Field_t *MS_PER_REP = 		new_Field("Time per rep", DT_DOUBLE, "%0.2lfms", false, 0);
	Field_t *NS_PER_RUN = 		new_Field("Time per run", DT_DOUBLE, "%0.2lfns", false, 0);
	Field_t *CYCLES_PER_RUN = 	new_Field("Cycles per run", DT_DOUBLE, "~%0.1lf", false, 0);
	Field_t *ACCURACY = 		new_Field("Accuracy", DT_DOUBLE, "%0.2lf%%", false, 0);

	{
		double total_diff = 0;
		uint64_t ns_before = get_cputime_ns();
		for (int rep = 0; rep < NUM_REPS; ++rep) {
			for (int i = 0; i < NUM_ITERATIONS; ++i) {
				int r = rand_range(0, SQRT_TEST_MAX);
				double heron = heron_sqrt(r);
				double real = sqrt(r);
				total_diff += fabs(heron - real);
			}
		}
		uint64_t ns_elapsed = get_cputime_ns() - ns_before;
		double ns_per_rep = (double)ns_elapsed / NUM_REPS;
		double ns_per_run = ns_per_rep / NUM_ITERATIONS;
		double cpr = ns_per_run * clock_ghz;
		double accuracy = ( 1 - ( total_diff / (NUM_REPS * NUM_ITERATIONS) / 1) ) * 100.0;

		add_Field_entry(TEST_NAME, "Combined");
		add_Field_entry(MS_OVERALL, nstoms(ns_elapsed));
		add_Field_entry(MS_PER_REP, nstoms(ns_per_rep));
		add_Field_entry(NS_PER_RUN, ns_per_run);
		add_Field_entry(CYCLES_PER_RUN, cpr);
		add_Field_entry(ACCURACY, accuracy);



	}

	printf_banner("\x1b[32;1m", "BENCHMARKING BUILTIN SQRT");
	{
		uint64_t ns_before = get_cputime_ns();
		for (int run = 0; run < NUM_REPS; ++run) {
			for (int sub_run = 0; sub_run < NUM_ITERATIONS; ++sub_run) {
				int r = rand_range(0, SQRT_TEST_MAX);
				double real = sqrt(r);
			}
		}
		uint64_t ns_elapsed = get_cputime_ns() - ns_before;
		double ns_per_rep = (double)ns_elapsed / NUM_REPS;
		double ns_per_run = ns_per_rep / NUM_ITERATIONS;
		double cpr = ns_per_run * clock_ghz;
		add_Field_entry(TEST_NAME, "Builtin");
		add_Field_entry(MS_OVERALL, nstoms(ns_elapsed));
		add_Field_entry(MS_PER_REP, nstoms(ns_per_rep));
		add_Field_entry(NS_PER_RUN, ns_per_run);
		add_Field_entry(CYCLES_PER_RUN, cpr);



	}
	printf_banner("\x1b[32;1m", "BENCHMARKING HERON SQRT");
	{
		uint64_t ns_before = get_cputime_ns();
		for (int run = 0; run < NUM_REPS; ++run) {
			for (int sub_run = 0; sub_run < NUM_ITERATIONS; ++sub_run) {
				int r = rand_range(0, SQRT_TEST_MAX);
				double heron = heron_sqrt(r);
			}
		}
		uint64_t ns_elapsed = get_cputime_ns() - ns_before;
		double ns_per_rep = (double)ns_elapsed / NUM_REPS;
		double ns_per_run = ns_per_rep / NUM_ITERATIONS;
		double cpr = ns_per_run * clock_ghz;
		add_Field_entry(TEST_NAME, "Herons");
		add_Field_entry(MS_OVERALL, nstoms(ns_elapsed));
		add_Field_entry(MS_PER_REP, nstoms(ns_per_rep));
		add_Field_entry(NS_PER_RUN, ns_per_run);
		add_Field_entry(CYCLES_PER_RUN, cpr);

	}
	assign_Field(&results, TEST_NAME);
	assign_Field(&results, MS_OVERALL);
	assign_Field(&results, MS_PER_REP);
	assign_Field(&results, NS_PER_RUN);
	assign_Field(&results, CYCLES_PER_RUN);
	assign_Field(&results, ACCURACY);
	print_FieldTable(results);

	double walltime_s = nstos(get_realtime_ns() - realtime_start_ns);
	double cputime_s = nstos(get_cputime_ns() - cputime_start_ns);
	printf_heading("total: %0.2lfs wall, %0.2lfs cpu", walltime_s, cputime_s);


	exit(EXIT_SUCCESS);

}


double heron_sqrt(int num) {
	int g = 0;
	int base = 0;
	int sq = 0;
	int prev_base = 0;
	int prev_sq = 0;
	while (sq <= num) {
		sq = pow(base, 2);
		if (sq >= num) {
			g = base;
			break;
		}
		++base;
		prev_base = base;
		prev_sq = sq;
	}
	double b = (double)num / (double)g;
	double ans = (g + b) / 2.0;
	return ans;

}



