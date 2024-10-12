#include "pch.h"

#include "include/wlist_s.h"
#include <wcont4r_factory.h>

wcont4r_t* wcont4r_list_make(wlist_t* const restrict list);

wcont4r_t* make_cont_list(void)
{
	wcont4r_t* cont = NULL;

	wlist_t* list = wlist_make_s();

	if (!list) {
		wlog_if_level(wll_warning,
			"Unable to make a list\n");
		goto end;
	}

	cont = wcont4r_list_make(list);

end:
	return cont;
}
