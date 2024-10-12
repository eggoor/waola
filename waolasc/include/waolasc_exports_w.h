#pragma once

#ifdef WAOLACOD_EXPORTS
#define WAOLACOD_API __declspec( dllexport )
#else
#define WAOLACOD_API __declspec( dllimport )
#endif
