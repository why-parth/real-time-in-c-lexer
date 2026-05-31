
#ifdef _REQ_INIT_V_SCRIPT
#undef _REQ_INIT_V_SCRIPT
#define _INIT_V_SCRIPT

#define charclass VERLET_charclass
#define VSCRIPT_INVALID_CHAR_CASE return VERLET_charclass_extend(c)

#include "svh____impl.c"

#endif

#ifdef _REQ_INIT_V_COLLECT
#undef _REQ_INIT_V_COLLECT
#define _INIT_V_COLLECT

#define VLUT_NO_VALUE_HANDLE goto VLUT_ERROR_CHECK
#define VLUT_NO_KEY_HANDLE goto VLUT_ERROR_CHECK
#define VLUT_ERROR_HANDLE VERLET_VLUT_exit()
#define VLUT_ERROR_RETURN_HANDLE COLLECT_OUT_NULL

#include "svh____impl.c"

#endif