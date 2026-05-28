
#if defined(VERLET_SCRIPT_IMPL)
#undef VERLET_SCRIPT_IMPL

#define charclass VERLET_charclass
#define VSCRIPT_INVALID_CHAR_CASE return VERLET_charclass_extend(c)

#include v_script_init

#endif

#if defined(VERLET_COLLECT_IMPL)
#undef VERLET_COLLECT_IMPL

#define VLUT_NO_VALUE_HANDLE goto VLUT_ERROR_CHECK
#define VLUT_NO_KEY_HANDLE goto VLUT_ERROR_CHECK
#define VLUT_ERROR_HANDLE VERLET_VLUT_exit()
#define VLUT_ERROR_RETURN_HANDLE COLLECT_OUT_NULL

#include v_collect_init

#endif