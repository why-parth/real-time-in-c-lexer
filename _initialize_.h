

#ifdef _REQ_INIT_V_DSL
#undef _REQ_INIT_V_DSL
#define _INIT_V_DSL

#ifdef DYNAMIC_CHARCLASS

#else

#ifndef charclass
#define charclass VERLET_charclass
#endif

#endif

#define VSCRIPT_INVALID_CHAR_CASE return VERLET_charclass_extend(c)

#endif

#ifdef _REQ_INIT_V_COLLECT
#undef _REQ_INIT_V_COLLECT
#define _INIT_V_COLLECT

#define VLU_NO_VALUE_HANDLE goto VLU_ERROR_CHECK
#define VLU_NO_KEY_HANDLE goto VLU_ERROR_CHECK
#define VLU_ERROR_HANDLE VERLET_VLU_exit()
#define VLU_ERROR_RETURN_HANDLE COLLECT_OUT_NULL

#endif
