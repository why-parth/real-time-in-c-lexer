//
// Verlet Token List Header
//
// 1.0
/*
This header initialises the pre-compilation pipeline of Verlet Lexer.
*/

/*
#define NO_PRECOMPILATION
This is used to hint that we are not going to be using the Pre-compilation SVH.

#define SELF_PRECOMPILATION
This is used to hint that we are going to be using our own implementation of the Pre-compilation SVH.
The header in this case will expect all the functions and macros to be correctly defined

#define VERLET_PRECOMPILATION
This is the default and standard implementation of Pre-compilation SVH.
*/

// RULE : Do not include .c files at the time of compilation, the pre-compiler wont be able to read that. Use #include to inlcude the directly into the .c files.

#ifndef _INC_V_PRECOMPILATION
#define _INC_V_PRECOMPILATION

#include "svh_08_cimpl.h"

#ifndef NO_DEFAULT_PRECOMPILATION
#define VERLET_PRECOMPILATION
#endif

#ifndef NO_PRECOMPILATION

#if defined SELF_PRECOMPILATION

// pass

#elif defined VERLET_PRECOMPILATION

// all of the preprocessing can save global data so that next step can use it

void PC_process(char * path);

#endif

#endif

#endif