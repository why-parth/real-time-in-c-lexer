//
// Verlet VLUT Header
//
/*
This header initialises the basic data structure that stores the Verlet Lexer's behaviour
*/

#ifndef _INC_V_VLUT
#define _INC_V_VLUT

#include "svh_02_hash.h"

struct VLUT_vlut_veiw {
    size_t n_keys;
    size_t n_values;
    size_t max_keys;
    size_t max_values;
    avsme * table;
};

#define VLUT_HINT(name, max_keys, max_values)   \
    avsme mergetokens(VLUT_, name, _table)[max_keys * (max_values + 2)]; struct VLUT_vlut_veiw name = {0, 0, max_keys, max_values, 0}

#define VLUT_INIT(name)   \
    name.table = mergetokens(VLUT_, name, _table)

#define VLUT_DECLARE(name, max_keys, max_values)    \
    VLUT_HINT(name, max_keys, max_values);          \
    VLUT_INIT(name)

#define __VLUT_PUSH_KEY(name)                                   \
    name.table[                                                 \
        name.n_keys * name.max_values                           \
        + (name.table[name.n_keys * name.max_values + 1] = 0)   \
    ]
#define VLUT_PUSH_KEY(name)			    __VLUT_PUSH_KEY(name) =

#define __VLUT_PUSH_VALUE(name, i_key)  \
    name.table[                         \
        name.n_keys * name.max_values   \
        + ++(name.table[i_key][1]) + 1  \
    ]
#define VLUT_PUSH_VALUE(name, i_key)	__VLUT_PUSH_VALUE(name, i_key) =

#endif