/* -*- Mode: C; tab-width: 8; c-basic-offset: 8; indent-tabs-mode: t; -*- */

#include "rrutil.h"

#define NUM_ITERATIONS (1 << 30)

static void breakpoint() {
	int break_here = 1;
	(void)break_here;
}

int spin() {
	int i, dummy = 0;

	atomic_puts("spinning");
	/* NO SYSCALLS AFTER HERE: the point of this test is to hit
	 * hpc interrupts to exercise the nonvoluntary interrupt
	 * scheduler. */
	for (i = 1; i < NUM_ITERATIONS; ++i) {
		dummy += i % (1 << 20);
		dummy += i % (79 * (1 << 20));
		if (i == NUM_ITERATIONS / 2) {
			breakpoint();
		}
	}
	return dummy;
}

int main(int argc, char *argv[]) {

	atomic_printf("EXIT-SUCCESS dummy=%d\n", spin());
	return 0;
}
