#ifndef SLIC3REXPORT_1614581188304_H
#define SLIC3REXPORT_1614581188304_H

#ifdef WIN32
	#ifdef SLIC3R_WRAPPER_DLL
		#define SLIC3R_WRAPPER_API __declspec(dllexport)
	#else
		#define SLIC3R_WRAPPER_API __declspec(dllimport)
	#endif
#else
	#ifdef SLIC3R_WRAPPER_DLL
		#define SLIC3R_WRAPPER_API __attribute__((visibility("default")))
	#else
		#define SLIC3R_WRAPPER_API
	#endif
#endif

#endif // SLIC3REXPORT_1614581188304_H