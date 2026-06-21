

#ifndef _INC_V_STATAL
#define _INC_V_STATAL

// to-add
// STATIC_CHARCLASS
// STATIC_VLU

#include "_01_non_statal.h"

struct VLU_VLU_view {
    avsme * table;
    char * iden;
    size_t n_keys;
    size_t n_values;
    size_t max_keys;
    size_t max_values;
};

#define CC_Type_VLU 2

#define VLU_DECL(name, max_keys, max_values)                                                    \
    avsme mergetokens(VLU_, name, _table)[max_keys * (max_values + 2)];                         \
    char mergetokens(VLU_, name, _iden)[] = #name;                                              \
    struct VLU_VLU_view name = {0, mergetokens(VLU_, name, _iden), 0, 0, max_keys, max_values}

#define VLU_INIT(name)                                          \
    name.table = mergetokens(VLU_, name, _table);               \
    CC_List_append(CC_Type_VLU) &name

#define VLU_DEFINE(name, max_keys, max_values)     \
    VLU_DECL(name, max_keys, max_values);          \
    VLU_INIT(name)

#define __VLU_PUSH_KEY(name)                                    \
    name.table[                                                 \
        name.n_keys * name.max_values                           \
        + (name.table[name.n_keys * name.max_values + 1] = 0)   \
    ]

#define VLU_PUSH_KEY(name)			    __VLU_PUSH_KEY(name) =

#define __VLU_PUSH_VALUE(name, i_key)  \
    name.table[                         \
        name.n_keys * name.max_values   \
        + ++(name.table[i_key][1]) + 1  \
    ]
#define VLU_PUSH_VALUE(name, i_key)	__VLU_PUSH_VALUE(name, i_key) =

#endif
