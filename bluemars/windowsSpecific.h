#ifndef _INCLUDE_WINDOWS_SPECIFIC
#define _INCLUDE_WINDOWS_SPECIFIC

/* Cmake will define BlueMars_EXPORTS on Windows when it
configures to build a shared library. If you are going to use
another build system on windows or create the visual studio
projects by hand you need to define BlueMars_EXPORTS when
building a DLL on windows.
*/

// We are using the Visual Studio Compiler and building Shared libraries
#if defined (_WIN32) 
	#if defined(BlueMars_EXPORTS)
		#define  BlueMars_EXPORT __declspec(dllexport)
	#else
		#define  BlueMars_EXPORT __declspec(dllimport)
	#endif
#else
	#define BlueMars_EXPORT
#endif

#endif