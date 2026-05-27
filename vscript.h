#if !defined(VERLET_SCRIPT)
#define VERLET_SCRIPT
#include "vcollect.h"
#include <string.h>

_Noreturn void VERLET_VLUT_exit(void);

avsme VERLET_charclass(char c);

_Noreturn void VERLET_charclass_invalid_char_error(char c);

avsme VERLET_charclass_extend(char c);

void VERLET_charclass_print(void);

struct VS_RECENT_VLUT{
    size_t n_keys;
    size_t n_values;
    size_t max_keys;
    size_t max_values;
    avsme ** table;
};

#define esc_seq_str(c)                                                                          \
    (c == '\n') ? __charclass_extend_esc_n :                                                    \
    (c == '\t') ? __charclass_extend_esc_t :                                                    \
    (c == '\r') ? __charclass_extend_esc_r :                                                    \
    (c == '\a') ? __charclass_extend_esc_a : "\033[31m<! invalid escape sequence. !>\033[39m"

extern char * __recent_charclass_extend_esc_str;
extern char __charclass_extend_esc_n[4];
extern char __charclass_extend_esc_t[4];
extern char __charclass_extend_esc_r[4];
extern char __charclass_extend_esc_a[4];

extern struct VS_RECENT_VLUT VS_RECENT_VLUT;

void VS__free_recent_vlut(void);

#ifndef VS_RECENT_VLUT_MAX
#define VS_RECENT_VLUT_MAX
#define VS_RECENT_VLUT_MAX_KEYS 8
#define VS_RECENT_VLUT_MAX_VALUES 8
#endif

#ifndef VS_RECENT_VLUT_INC_MAX_KEYS
#define VS_RECENT_VLUT_INC_MAX_KEYS 2
#endif

#ifndef VS_RECENT_VLUT_INC_MAX_VALUES
#define VS_RECENT_VLUT_INC_MAX_VALUES 2
#endif

#define new_vlut                                                                                    \
    VS__free_recent_vlut();                                                                         \
    VS_RECENT_VLUT.n_keys = 0;                                                                      \
    VS_RECENT_VLUT.n_values = 0;                                                                    \
    VS_RECENT_VLUT.max_keys = VS_RECENT_VLUT_MAX_KEYS;                                              \
    VS_RECENT_VLUT.max_values = VS_RECENT_VLUT_MAX_VALUES;                                          \
    VS_RECENT_VLUT.table = (avsme * *)malloc(VS_RECENT_VLUT.max_keys * sizeof(avsme *))

#define __new_entry VS_RECENT_VLUT.table[VS_RECENT_VLUT.n_keys] = (avsme *)malloc(VS_RECENT_VLUT.max_values * sizeof(avsme))

#define new_key   if(                                                               \
                        (VS_RECENT_VLUT.n_keys == VS_RECENT_VLUT.max_keys)          \
                     && (VS_RECENT_VLUT.max_keys += VS_RECENT_VLUT_INC_MAX_KEYS)    \
                     && (printf("<max keys : %zu>", VS_RECENT_VLUT.max_keys))       \
                    )                                                               \
    VS_RECENT_VLUT.table = (avsme * *)realloc(                                           \
                                            VS_RECENT_VLUT.table,                           \
                                            VS_RECENT_VLUT.max_keys * sizeof(avsme *)    \
                                            );                                              \
    __new_entry;                                                                        \
    (VS_RECENT_VLUT.table[VS_RECENT_VLUT.n_keys])[1] = (VS_RECENT_VLUT.n_values = 0);   \
    (VS_RECENT_VLUT.table[VS_RECENT_VLUT.n_keys++])[0] = 

#define new_value if(                                                                   \
                        (VS_RECENT_VLUT.n_values == VS_RECENT_VLUT.max_values)          \
                     && (VS_RECENT_VLUT.max_values += VS_RECENT_VLUT_INC_MAX_VALUES)    \
                     && (printf("<max values : %zu>", VS_RECENT_VLUT.max_values))       \
                    )                                                                   \
    VS_RECENT_VLUT.table[VS_RECENT_VLUT.n_keys - 1] = (avsme *)realloc(                         \
                                            VS_RECENT_VLUT.table[VS_RECENT_VLUT.n_keys - 1],    \
                                            VS_RECENT_VLUT.max_values * sizeof(avsme)           \
                                            );                                                  \
    VS_RECENT_VLUT.n_values++;  \
    VS_RECENT_VLUT.table[VS_RECENT_VLUT.n_keys - 1][2 + (VS_RECENT_VLUT.table[VS_RECENT_VLUT.n_keys - 1][1]++)] = 


#define idvalid (uint8_t)(1 << AVSME_MAINCLASS_SHIFT)
    #define under   | (1 << AVSME_SUBCLASS_SHIFT)
    #define lower   | (2 << AVSME_SUBCLASS_SHIFT)
    #define upper   | (3 << AVSME_SUBCLASS_SHIFT)

#define numeric (uint8_t)(2 << AVSME_MAINCLASS_SHIFT)

#define operate (uint8_t)(3 << AVSME_MAINCLASS_SHIFT)
    #define single  | (1 << AVSME_SUBCLASS_SHIFT)
    #define double  | (2 << AVSME_SUBCLASS_SHIFT)

#define enclose (uint8_t)(4 << AVSME_MAINCLASS_SHIFT)
    #define open    | (1 << AVSME_SUBCLASS_SHIFT)
    #define close   | (2 << AVSME_SUBCLASS_SHIFT)

#define quoting (uint8_t)(5 << AVSME_MAINCLASS_SHIFT)

#define disjoin (uint8_t)(6 << AVSME_MAINCLASS_SHIFT)
    #define comma   | (1 << AVSME_SUBCLASS_SHIFT)
    #define dot     | (2 << AVSME_SUBCLASS_SHIFT)
    #define space   | (3 << AVSME_SUBCLASS_SHIFT)

#define special (uint8_t)(7 << AVSME_MAINCLASS_SHIFT)

#define escape  (uint8_t)(8 << AVSME_MAINCLASS_SHIFT)

#define assign  (uint8_t)(9 << AVSME_MAINCLASS_SHIFT)

#define arbitrary_str(c) "\033[2;33m%c\033[22;39m", c
#define idvalid_str "\033[1;32midvalid\033[22;39m"
#define under_str  "\033[1;32midvalid \033[22;36munder\033[39m"
#define lower_str  "\033[1;32midvalid \033[22;36mlower\033[39m"
#define upper_str  "\033[1;32midvalid \033[22;36mupper\033[39m"
#define numeric_str "\033[1;32mnumeric\033[22;39m"
#define operate_str "\033[1;32moperate\033[22;39m"
#define single_str  "\033[1;32moperate \033[22;36msingle\033[39m"
#define double_str  "\033[1;32moperate \033[22;36mdouble\033[39m"
#define enclose_str "\033[1;32menclose\033[22;39m"
#define open_str  "\033[1;32menclose \033[22;36mopen\033[39m"
#define close_str  "\033[1;32menclose \033[22;36mclose\033[39m"
#define quoting_str "\033[1;32mquoting\033[22;39m"
#define disjoin_str "\033[1;32mdisjoin\033[22;39m"
#define comma_str  "\033[1;32mdisjoin \033[22;36mcomma\033[39m"
#define dot_str  "\033[1;32mdisjoin \033[22;36mdot\033[39m"
#define space_str  "\033[1;32mdisjoin \033[22;36mspace\033[39m"
#define special_str "\033[1;32mspecial\033[22;39m"
#define escape_str "\033[1;32mescape\033[22;39m"
#define assign_str "\033[1;32massign\033[22;39m"

#define __fc_check(fc, n, _str) (fc == n) ? _str :

#define VERLET_fullclass(_flag) AVSME_GET(_flag, MAINCLASS)*10 + AVSME_GET(_flag, SUBCLASS)

#define VERLET_fullclass_str(fc)                              \
__fc_check(fc, 10, "\033[1;32midvalid\033[22;39m")    \
__fc_check(fc, 11, "\033[1;32midvalid \033[22;36munder\033[39m")           \
__fc_check(fc, 12, "\033[1;32midvalid \033[22;36mlower\033[39m")           \
__fc_check(fc, 13, "\033[1;32midvalid \033[22;36mupper\033[39m")           \
__fc_check(fc, 20, "\033[1;32mnumeric\033[22;39m")    \
__fc_check(fc, 30, "\033[1;32moperate\033[22;39m")    \
__fc_check(fc, 31, "\033[1;32moperate \033[22;36msingle\033[39m")          \
__fc_check(fc, 32, "\033[1;32moperate \033[22;36mdouble\033[39m")          \
__fc_check(fc, 40, "\033[1;32menclose\033[22;39m")    \
__fc_check(fc, 41, "\033[1;32menclose \033[22;36mopen\033[39m")            \
__fc_check(fc, 42, "\033[1;32menclose \033[22;36mclose\033[39m")           \
__fc_check(fc, 50, "\033[1;32mquoting\033[22;39m")    \
__fc_check(fc, 60, "\033[1;32mdisjoin\033[22;39m")    \
__fc_check(fc, 61, "\033[1;32mdisjoin \033[22;36mcomma\033[39m")           \
__fc_check(fc, 62, "\033[1;32mdisjoin \033[22;36mdot\033[39m")             \
__fc_check(fc, 63, "\033[1;32mdisjoin \033[22;36mspace\033[39m")           \
__fc_check(fc, 70, "\033[1;32mspecial\033[22;39m")    \
__fc_check(fc, 80, "\033[1;32mescape\033[22;39m")     \
__fc_check(fc, 90, "\033[1;32massign\033[22;39m")     \
"\033[;31m<! invalid fullclass code !>\033[39m"

#define __print_mainclass(m) printf(mergetoken(m, _str))
#define __print_subclass(m, s) printf(mergetokens4(m, _, c, _str))
#define __print_arbitr_char(c) printf(arbitrary_str(c))

size_t VERLET_avsme_print(avsme _flag);

void VS_RECENT_VLUT_print(void);

#define VS_VLUT_print(_vlut)	                                                \
                    __VS_VLUT_print (                                           \
                                        _vlut.n_keys,                           \
                                        (avsme *)_vlut.table,                   \
                                        sizeof(*(_vlut.table)) / sizeof(avsme)  \
                                    ) /* Parameter Extraction Layer */

void __VS_VLUT_print   (
                            size_t n_keys,
                            avsme * table,
                            size_t _stride
                        ); /* Actual Function */

#define Vlut new_vlut; if (1)
#define For new_key
#define Join ;new_value 1 + 0 | 
#define Vary ;new_value 1 + AVSME_VARIANCE | 
#define Char << AVSME_ASCII_SHIFT
#define Set ;new_value AVSME_FALSE
#define All AVSME_TRUE
#define Show VS_RECENT_VLUT_print()
#define ShowVLUT(_vlut) VS_VLUT_print(_vlut)

#define token_show          printf("\033[38;2;200;200;0m<\033[39m"), print_collected(out), printf("\033[38;2;200;200;0m>\033[39m")
#define token_print         print_collected(out)
#define token_paste(dest)   paste_collected(dest, out)

#define Make(_name, _max_keys, _max_values)                             \
    VLUT_DECLARE(_name, _max_keys, _max_values);                        \
    _name.n_keys = VS_RECENT_VLUT.n_keys;                               \
    for (int i = 0; i < VS_RECENT_VLUT.n_keys; i++) {                   \
        memcpy(                                                         \
            (avsme *)(_name.table) + i*(VS_RECENT_VLUT.max_values + 2), \
            VS_RECENT_VLUT.table[i],                                    \
            ((int)(VS_RECENT_VLUT.table[i][1] + 2))*sizeof(avsme)       \
        );                                                              \
    }                                                                   \
    VS__free_recent_vlut()

extern size_t VS_active_vlut_n_keys;
extern avsme * VS_active_vlut_table;
extern size_t VS_active_vlut_stride;

#define Using(_vlut)                                                 \
    VS_active_vlut_n_keys = _vlut.n_keys;                            \
    VS_active_vlut_table = (avsme *)_vlut.table;                     \
    VS_active_vlut_stride = sizeof(*(_vlut.table)) / sizeof(avsme)

#define Collect(string)\
    ;for ( struct collect_out out = { .char_class = 1000 } ; out.char_class == 1000 ; ) \
    while (                                                                             \
            (                                                                           \
            out = __collect_variation_in (                                              \
                string,                                                                 \
                VS_active_vlut_n_keys,                                                  \
                (avsme *)VS_active_vlut_table,                                          \
                VS_active_vlut_stride                                                   \
            )                                                                           \
        ).old_token                                                                     \
    )

#define VERLET_SCRIPT_IMPL

#endif