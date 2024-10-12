#include "pch.h"

#include "mem.h"
#include "threads.h"

int main(int argc, char* argv[])
{
	wlog_set_level(wll_error);

	testMem();
	testThreads();

	return 0;
}
