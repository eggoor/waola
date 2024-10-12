#pragma once

#ifdef WAOLAD_EXPORTS
#define WAOLAD_API __declspec( dllexport )
#else
#define WAOLAD_API __declspec( dllimport )
#endif
