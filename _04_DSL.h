

#ifndef _INC_V_DSL
#define _INC_V_DSL

#include "_03_collect.h"

// DSL : VScript

struct VS_RECENT_VLU {
    size_t n_keys;
    size_t n_values;
    size_t max_keys;
    size_t max_values;
    avsme ** table;
};

extern struct VS_RECENT_VLU VS_RECENT_VLU;

#ifndef VS_RECENT_VLU_MAX
#define VS_RECENT_VLU_MAX
#define VS_RECENT_VLU_MAX_KEYS      1
#define VS_RECENT_VLU_MAX_VALUES    1
#endif

#ifndef VS_RECENT_VLU_INC_MAX_KEYS
#define VS_RECENT_VLU_INC_MAX_KEYS      1
#endif

#ifndef VS_RECENT_VLU_INC_MAX_VALUES
#define VS_RECENT_VLU_INC_MAX_VALUES    1
#endif

#define new_VLU                                                                         \
    VS__free_recent_VLU(),                                                              \
    VS_RECENT_VLU.n_keys = 0,                                                           \
    VS_RECENT_VLU.n_values = 0,                                                         \
    VS_RECENT_VLU.max_keys = VS_RECENT_VLU_MAX_KEYS,                                    \
    VS_RECENT_VLU.max_values = VS_RECENT_VLU_MAX_VALUES,                                \
    VS_RECENT_VLU.table = (avsme * *)malloc(VS_RECENT_VLU.max_keys * sizeof(avsme *))

#define __new_entry VS_RECENT_VLU.table[VS_RECENT_VLU.n_keys] = (avsme *)malloc((VS_RECENT_VLU.max_values + 2) * sizeof(avsme))

#define new_key   if(                                                           \
                        (VS_RECENT_VLU.n_keys == VS_RECENT_VLU.max_keys)        \
                     && (VS_RECENT_VLU.max_keys += VS_RECENT_VLU_INC_MAX_KEYS)  \
                     && VS_RECENT_VLU.max_keys                                  \
                    )\
    VS_RECENT_VLU.table = (avsme * *)realloc(                                           \
                                            VS_RECENT_VLU.table,                        \
                                            VS_RECENT_VLU.max_keys * sizeof(avsme *)    \
                                            );                                          \
    __new_entry;                                                                        \
    (VS_RECENT_VLU.table[VS_RECENT_VLU.n_keys])[1] = (VS_RECENT_VLU.n_values = 0);      \
    (VS_RECENT_VLU.table[VS_RECENT_VLU.n_keys++])[0] = 

#define new_value if(                                                               \
                        (VS_RECENT_VLU.n_values == VS_RECENT_VLU.max_values)        \
                     && (VS_RECENT_VLU.max_values += VS_RECENT_VLU_INC_MAX_VALUES)  \
                     && VS_RECENT_VLU.max_values                                    \
                    )\
    VS_RECENT_VLU.table[VS_RECENT_VLU.n_keys - 1] = (avsme *)realloc(                       \
                                            VS_RECENT_VLU.table[VS_RECENT_VLU.n_keys - 1],  \
                                            (VS_RECENT_VLU.max_values + 2) * sizeof(avsme)      \
                                            );                                              \
    VS_RECENT_VLU.n_values++;  \
    VS_RECENT_VLU.table[VS_RECENT_VLU.n_keys - 1][2 + (VS_RECENT_VLU.table[VS_RECENT_VLU.n_keys - 1][1]++)] = 

void VS__free_recent_VLU(void);

void VS_RECENT_VLU_print(void);


#define VS_VLU_print(_VLU)	                                                    \
                    __VS_VLU_print (                                            \
                                        _VLU.n_keys,                            \
                                        (avsme *)_VLU.table,                    \
                                        _VLU.max_values + 2                     \
                                    ) /* Parameter Extraction Layer */

void __VS_VLU_print   (
                            size_t n_keys,
                            avsme * table,
                            size_t _stride
                        ); /* Actual Function */

#define VS_ptr_size sizeof(uintptr_t)
#define VS_mode_span (sizeof(uintptr_t) / sizeof(avsme) + !!((8 * sizeof(uintptr_t)) % (8 * sizeof(avsme) - 1)))

typedef void (* VS_mode_ptr_type) (void);

#define VS_mode_to_buffer(_buffer, _address)                                                    \
    for (int VS_i = 0; VS_i < VS_mode_span; VS_i++)                                             \
    (_buffer)[VS_i] = (avsme)((((uintptr_t)_address) >> (VS_i*((sizeof(avsme)*8) - 1))) << 1)

#define VS_buffer_to_mode(_address, _buffer)                                            \
    for (int VS_i = 0; VS_i < VS_mode_span; VS_i++)                                     \
    _address += ((uintptr_t)((_buffer)[VS_i] >> 1)) << (VS_i*((sizeof(avsme)*8) - 1));


#define VLU for ( new_VLU ; VS_RECENT_VLU.table ; VS__free_recent_VLU() )
#define For new_key AVSME_EXISTS |
#define Join ;new_value AVSME_EXISTS | 
#define Vary ;new_value AVSME_EXISTS | AVSME_VARIANCE | 
#define Char << AVSME_ASCII_SHIFT
#define Set ;new_value AVSME_FALSE
#define All AVSME_TRUE
#define Show VS_RECENT_VLU_print()
#define ShowVLU(_VLU) VS_VLU_print(_VLU)

#define Call(_address) ; for (int VS_i = 0; VS_i < VS_mode_span; VS_i++)                         \
    { new_value (avsme)((((uintptr_t)_address) >> (VS_i*((sizeof(avsme)*8) - 1))) << 1); }

#define token_show          printf("\033[38;2;200;200;0m<\033[39m"), VERLET_print_collected(COLLECT_OUT), printf("\033[38;2;200;200;0m>\033[39m")
#define token_print         print_collected(COLLECT_OUT)
#define token_paste(dest)   paste_collected(dest, COLLECT_OUT)

#define token_is(cmp, ...) (_Generic(cmp,                                                                           \
    hash : (hash_value == (hash)cmp),                                                                              \
    char * : (hash_value == hash_function((char *)cmp)),                                                                   \
    uint8_t : ( (COLLECT_OUT.char_class & AVSME_MAINCLASS) == _Generic((cmp), char *: 0, int *: 0, default : cmp)   \
        __VA_OPT__(&& (COLLECT_OUT.char_class & AVSME_SUBCLASS) == (0 __VA_ARGS__)) ),                              \
    default : (0)                                                                                                   \
))

#define token_is_not(cmp, ...) (!token_is(cmp __VA_OPT__(,) __VA_ARGS__))

#define class_is(cmp, ...) ( (COLLECT_OUT.char_class & AVSME_MAINCLASS) == cmp __VA_OPT__(&& (COLLECT_OUT.char_class & AVSME_SUBCLASS) == __VA_ARGS__) )


#define expect(_token, ...)                                                                             \
    static char mergetoken(EXPEC_, __LINE__) = 0;                                                       \
    if (_token && token_is(_token __VA_OPT__(,) __VA_ARGS__)) mergetoken(EXPEC_, __LINE__) = 1;         \
    else if (_token) for (; (mergetoken(EXPEC_, __LINE__) == 1); (mergetoken(EXPEC_, __LINE__) = 0))

#define keep_expecting break
#define stop_expecting continue


#define VS_cast_VLU_to_static(_name)                                        \
    _name.n_keys = VS_RECENT_VLU.n_keys;                                    \
    _name.max_keys = VS_RECENT_VLU.max_keys;                                \
    _name.max_values = VS_RECENT_VLU.max_values;                            \
    for (int i = 0; i <= VS_RECENT_VLU.n_keys; i++) {                       \
        if (i != VS_RECENT_VLU.n_keys) {                                    \
            memcpy(                                                         \
                _name.table + i*(VS_RECENT_VLU.max_values + 2),             \
                VS_RECENT_VLU.table[i],                                     \
                ((int)(VS_RECENT_VLU.table[i][1] + 2))*sizeof(avsme)        \
            );                                                              \
        }                                                                   \
        else {                                                              \
            VS__free_recent_VLU();                                          \
            VS_RECENT_VLU.n_keys = 0;                                       \
            VS_RECENT_VLU.n_values = 0;                                     \
        }

#define Make(_name, _max_keys, _max_values)             \
    break;}                                             \
    VLU_DEFINE(_name, _max_keys, _max_values);          \
    VS_cast_VLU_to_static(_name)

#define Decl(_name, _max_keys, _max_values)             \
    VLU_DECL(_name, _max_keys, _max_values);

#define Save(_name)                                     \
    break;}                                             \
    VLU_INIT(_name);                                    \
    VS_cast_VLU_to_static(_name)


extern size_t VS_active_VLU_n_keys;
extern avsme * VS_active_VLU_table;
extern size_t VS_active_VLU_stride;

#define Using(_VLU)                                                     \
    VS_active_VLU_n_keys = _VLU.n_keys;                                 \
    VS_active_VLU_table = _VLU.table;                                   \
    VS_active_VLU_stride = _VLU.max_values + 2


#define Collect(string)                                                                 \
    ;for ( COLLECT_OUT.char_class = 1000 ; COLLECT_OUT.char_class == 1000 ; )           \
    while (                                                                             \
            (                                                                           \
            COLLECT_OUT = collect_variation_in (                                        \
                string,                                                                 \
                VS_active_VLU_n_keys,                                                   \
                VS_active_VLU_table,                                                    \
                VS_active_VLU_stride                                                    \
            )                                                                           \
        ).old_token                                                                     \
    )

// Token Lists

extern size_t VCo_enable_i;
extern hash * VCo_recent_set;

#define __token_set(_name, __size)                           \
    hash _name[__size];                                      \
    size_t mergetokens(VCo_enable_, _name, _size) = __size;  \
    int mergetoken(VCo_enable_, _name) (void)

#define keyword(_kw, ...) VCo_recent_set[VCo_enable_i++ __VA_OPT__(* 0 + __VA_ARGS__)] = hash_function(#_kw)

#define keyword_str(_kw, ...) VCo_recent_set[VCo_enable_i++ __VA_OPT__(* 0 + __VA_ARGS__)] = hash_function(_kw)

#define keyword_hash(_kw, ...) VCo_recent_set[VCo_enable_i++ __VA_OPT__(* 0 + __VA_ARGS__)] = _kw

#define VCo_enable_token_set_init(_name) for (int i = VCo_enable_i; i < sizeof(_name)/sizeof(hash); i++) _name[i] = 0;

#define enable_token_list(_name, ...) VCo_enable_i = 0; VCo_recent_set = _name; __VA_OPT__(__VA_ARGS__ =) mergetoken(VCo_enable_, _name)(); VCo_enable_token_set_init(_name)

#define token_set_len VCo_enable_i

#define select_token_set(_name) VCo_recent_set = _name

int VCo_search_set(hash * _set, size_t _size);

#define token_in_set(_set_name) ( VCo_search_set((hash *)_set_name, mergetokens(VCo_enable_, _set_name, _size)) )

struct VCo_hash_link {
    hash value;
    struct VCo_hash_link * next;
};

struct VCo_hash_link * VCo_new_hash_link(hash _token);

#define new_hash_link(_token) VCo_new_hash_link(hash_of(_token))

void VCo_free_hash_links(struct VCo_hash_link * _hash_link);

#define __token_list(_name)                                             \
    size_t mergetokens(VCo_enable_, _name, _size);                      \
    size_t mergetokens(VCo_, _name, _count) = 0;                        \
    struct VCo_hash_link * _name = NULL;                                \
    struct VCo_hash_link * mergetokens(VCo_, _name, _last) = NULL       \

#define append_token_list(_name, _token)                                                \
    mergetokens(VCo_, _name, _count)++;                                                 \
    if (mergetokens(VCo_, _name, _last))                                                \
        mergetokens(VCo_, _name, _last) = (                                             \
            mergetokens(VCo_, _name, _last)->next = new_hash_link(_token)               \
        );                                                                              \
    else mergetokens(VCo_, _name, _last) = (                                            \
            _name = new_hash_link(_token)                                               \
        )

#define free_token_list(_name)                              \
    VCo_free_hash_links(_name),                             \
    mergetokens(VCo_, _name, _count) = 0,                   \
    _name = NULL,                                           \
    mergetokens(VCo_, _name, _last) = NULL

size_t VCo_search_hash_list(struct VCo_hash_link * _links, hash _token, ...);

#define token_in_list(_name, ...) (VCo_search_hash_list( _name, __VA_ARGS__ __VA_OPT__(,) hash_value))

// - Macro Overloading

#define VCo_token_list_type_(_name, ...) __token_list(_name)
#define VCo_token_list_type_0(_name, ...) __token_set(_name, __VA_ARGS__)
#define VCo_token_list_type(...) __mergetoken(VCo_token_list_type_, __VA_OPT__(0))

#define token_list(_name, ...) VCo_token_list_type(__VA_ARGS__)(_name __VA_OPT__(,) __VA_ARGS__)

// - Macro Handling

#define token_in(_name, ...) (_Generic((_name),                             \
    hash * : token_in_set(_name),                                           \
    struct VCo_hash_link * : token_in_list((struct VCo_hash_link *)_name)   \
))

#define token_not_in(_name, ...) (!token_in(_name __VA_OPT__(,) __VA_ARGS__))

// VScript's Standard Initialization 

#include <string.h>

_Noreturn void VERLET_VLU_exit(void);

avsme VERLET_charclass(char c);

_Noreturn void VERLET_charclass_invalid_char_error(char c);

avsme VERLET_charclass_extend(char c);

void VERLET_charclass_print(void);


extern char * __recent_charclass_extend_esc_str;
extern char __charclass_extend_esc_n[4];
extern char __charclass_extend_esc_t[4];
extern char __charclass_extend_esc_r[4];
extern char __charclass_extend_esc_a[4];

#define esc_seq_str(c)                                                                          \
    (c == '\n') ? __charclass_extend_esc_n :                                                    \
    (c == '\t') ? __charclass_extend_esc_t :                                                    \
    (c == '\r') ? __charclass_extend_esc_r :                                                    \
    (c == '\a') ? __charclass_extend_esc_a : "\033[31m<! invalid escape sequence. !>\033[39m"


#define idvalid (uint8_t)(1 << AVSME_MAINCLASS_SHIFT)
    #define under   | (1 << AVSME_SUBCLASS_SHIFT)
    #define lower   | (2 << AVSME_SUBCLASS_SHIFT)
    #define upper   | (3 << AVSME_SUBCLASS_SHIFT)

#define numeric (uint8_t)(2 << AVSME_MAINCLASS_SHIFT)

#define operate (uint8_t)(3 << AVSME_MAINCLASS_SHIFT)
    #define oneval  | (1 << AVSME_SUBCLASS_SHIFT)
    #define twoval  | (2 << AVSME_SUBCLASS_SHIFT)

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
#define oneval_str  "\033[1;32moperate \033[22;36moneval\033[39m"
#define twoval_str  "\033[1;32moperate \033[22;36mtwoval\033[39m"
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
__fc_check(fc, 31, "\033[1;32moperate \033[22;36moneval\033[39m")          \
__fc_check(fc, 32, "\033[1;32moperate \033[22;36mtwoval\033[39m")          \
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


#define VERLET_print_collected(out)                 \
    (out.old_token[0] == '\n') ? printf("\\n") :    \
    (out.old_token[0] == '\t') ? printf("\\t") :    \
    (out.old_token[0] == '\r') ? printf("\\r") :    \
    (out.old_token[0] == '\a') ? printf("\\a") :    \
    print_collected(out)


#define _REQ_INIT_V_DSL

#endif
