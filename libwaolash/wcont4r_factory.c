#include "pch.h"

#include <wcont4r_factory.h>

wcont4r_t* make_cont_list(void);

wcont4r_t* wmake_cont4r_s(wcont_type_t ct)
{
	wcont4r_t* cont = NULL;

	switch (ct)
	{
	case wct_list:
		cont = make_cont_list();
		break;
	default:
		wlog_if_level(wll_warning, "Invalid container type %d passed to %s\n",
			ct, __func__);
	}
	
	return cont;
}
