#include "pch.h"

#include "stopwatch.h"

void print_elapsed(unsigned long long int elapsed)
{
	int h = (int)(elapsed / (NS_IN_SEC * SEC_IN_HOUR));
	elapsed -= h * NS_IN_SEC * SEC_IN_HOUR;
	int m = (int)(elapsed / (NS_IN_SEC * SEC_IN_MIN));
	elapsed -= m * NS_IN_SEC * SEC_IN_MIN;
	int s = (int)(elapsed / NS_IN_SEC);
	elapsed -= s * NS_IN_SEC;
	int ms = (int)(elapsed / MS_IN_NS);
	if (elapsed % MS_IN_NS >= MS_IN_NS / 2) {
		++ms;
	}

	if (h) {
		printf("%02d:", h);
	}

	printf("%02d:", m);

	printf("%02d.%03d\n", s, ms);
}
