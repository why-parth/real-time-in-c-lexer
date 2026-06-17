//
// Verlet Token List Header
//
// 1.0
/*
This header initialises token lists for Verlet Lexer.
*/


// CCast is an optimization
/*
    - __ccast hash is macro defined
    - __ccast tokenlist is const defined
    - cascading same types of token_is and token_in are converted into switch case
    - fnv(...) is converted into direct token
*/

/*
    
*/

#ifndef _INC_V_CCAST
#define _INC_V_CCAST

#include "svh_09_precompilation.h"

void __CCAST_cast_VLUT(char * _name, struct VLUT_vlut_veiw _VLUT);
#define CCAST_cast_VLUT(_name) __CCAST_cast_VLUT(#_name, _name) 

void __CCAST_cast_VLUT(char * _name, struct VLUT_vlut_veiw _VLUT) {

    int x = 0;

    printf("avsme VLUT_%s_table[%zu] = {\n", _name, _VLUT.max_keys*(2 + _VLUT.max_values));

    for (int y = 0; y < _VLUT.n_keys; y++) {

        printf("    %zu, %zu",
            _VLUT.table[y *(2 + _VLUT.max_values)],
            _VLUT.table[y *(2 + _VLUT.max_values) + 1]
        );

        for (x = 0; x < _VLUT.table[y *(2 + _VLUT.max_values) + 1]; x++)
        printf(", %zu", _VLUT.table[y*(2 + _VLUT.max_values) + 2 + x]);

        for (; x < _VLUT.max_values; x++)
        printf(", 0");

        if (y + 1 != _VLUT.n_keys) putchar(',');
        putchar('\n');
    }

    printf("}; ");

    printf("struct VLUT_vlut_veiw %s = {", _name);
    printf(" %d,", _VLUT.n_keys);
    printf(" %d,", _VLUT.n_values);
    printf(" %d,", _VLUT.max_keys);
    printf(" %d,", _VLUT.max_values);
    printf(" VLUT_%s_table };", _name);

}


// Cast state -> VLU + Token Lists -> static buffers
// Cast token -> const hash -> macros
// Cast cases -> if-else to switch-case


#endif