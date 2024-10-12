#include "pch.h"

#include "waolag.h"

int main(int argc, char* argv[])
{
	wlog_if_level(wll_verbose, "Starting waolag\n");

	WaolaG* app = waolag_new("com.asuscomm.smartnclever.WaolaG",
		G_APPLICATION_DEFAULT_FLAGS);

	int rc = g_application_run(G_APPLICATION(app), argc, argv);
	g_object_unref(app);
	
	return rc;
}
