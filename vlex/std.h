#if !defined(__INC_STDV)

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define vfilename(path) ( strrchr(path, '/') ? strrchr(path, '/') + 1 : strrchr(path, '\\') ? strrchr(path, '\\') + 1 : path )

#if defined(_WIN32) && !defined(__vshell_format_copy)                                               
#define vshell_format_copy "copy %s \"tr-%s.c\" >nul"     
#else                                                               
#define vshell_format_copy "cp %s \"tr-%s.c\""                
#endif   

#define __TYPE_ARGS(...) __VA_OPT__((int*))((int)0)
#define IF_ARGS(yes, no, ...) _Generic(__TYPE_ARGS(__VA_ARGS__), int*: yes, int: no)

#define __char_to_lower(_char) (_char - ('A' - 'a'))
#define __char_to_upper(_char) (_char - ('a' - 'A'))
#define __char_lower_alpha(_char) ('a' <= _char && _char <= 'z')
#define __char_upper_alpha(_char) ('A' <= _char && _char <= 'Z')
#define __char_alpha(_char) (__char_lower_alpha(_char) || __char_upper_alpha(_char))
#define __char_digit(_char) ('0' <= _char && _char <= '9')
#define __char_alnum(_char) (__char_digit(_char) || __char_alpha(_char))

#define __char_idvalid(_char) (__char_alpha(_char) || (_char == '_'))
#define __char_nsymbol(_char) (__char_alnum(_char) || (_char == '_'))

#define __char_symbol(_char) ((!__char_nsymbol(_char)))

#define __stringof(a) #a
#define stringof(a) __stringof(a)

#define __mergetoken(a, b) a##b
#define mergetoken(a, b) __mergetoken(a, b)

#define __mergetokens(a, b, c) a##b##c
#define mergetokens(a, b, c) __mergetokens(a, b, c)

#define __mergetokens4(a, b, c, d) a##b##c##d
#define mergetokens4(a, b, c, d) __mergetokens4(a, b, c, d)

#define uniq(_iden) mergetoken(_iden, __LINE__)

#define style_bold printf("\033[1m")
#define style_bold_reset printf("\033[22m")

#define style_italics printf("\033[3m")
#define style_italics_reset printf("\033[23m")

#define style_underline printf("\033[4m")
#define style_underline_reset printf("\033[24m")

#define color_fore(r, g, b) printf("\033[38;2;%u;%u;%um", r, g, b)
#define color_back(r, g, b) printf("\033[48;2;%u;%u;%um", r, g, b)
#define color_fore_reset    printf("\033[39m")
#define color_back_reset    printf("\033[49m")

#define color_fore_grey(g)  color_fore(g, g, g)
#define color_back_grey(g)  color_back(g, g, g)

#define full_reset color_fore_reset; color_back_reset; style_bold_reset; style_italics_reset; style_underline_reset

#define debug_name "DEBUG"

#define debug_print(format, ...) printf("\033[1;34m\n[" debug_name "] \033[22m" format "\033[39m" __VA_OPT__(,) __VA_ARGS__)

#define debug_carry(i, format, ...) printf("\n %*.s" format "\033[39m", 4 * i, "" __VA_OPT__(,) __VA_ARGS__)

#define debug_continue(format, ...) printf("\033[33m" format "\033[39m" __VA_OPT__(,) __VA_ARGS__)

#define debug_bar "\033[34m|\033[39m"

typedef uint_least16_t avsme;

#define AVSME_NULL 0

#define AVSME_ASCII 		0b1111111100000000
#define AVSME_VARIANCE 	    0b0000000010000000
#define AVSME_SUBCLASS  	0b0000000001100000
#define AVSME_MAINCLASS 	0b0000000000011110
#define AVSME_EXISTS 		0b0000000000000001

#define AVSME_ASCII_SHIFT 		    8
#define AVSME_VARIANCE_SHIFT 		7
#define AVSME_SUBCLASS_SHIFT 		5
#define AVSME_MAINCLASS_SHIFT 	    1
#define AVSME_EXISTS_SHIFT 		    0

#define AVSME_BIT(_name, _mask)     ( _name & mergetoken(AVSME_, _mask) )

#define AVSME_GET(_name, _mask)                                                         \
(( _name & mergetoken(AVSME_, _mask) ) >> mergetokens(AVSME_, _mask, _SHIFT))

#define AVSME_SET(_name, _mask, _value)                                                 \
((_name & ~mergetoken(AVSME_, _mask)) | (_value << mergetokens(AVSME_, _mask, _SHIFT)))

#define AVSME_COMPARE(a, b, _mask) (AVSME_BIT(a, _mask) == AVSME_BIT(b, _mask))

extern avsme AVSME_NONE;
extern avsme AVSME_TRUE;
extern avsme AVSME_FALSE;

#define AVSME_BOOLEAN(a)                            \
( ((AVSME_GET(a, ASCII)) == 1) ? AVSME_TRUE         \
: ( ((AVSME_GET(a, ASCII)) == 2) ? AVSME_FALSE      \
: AVSME_NONE                                        \
) )

#define AVSME_OVERLAP(a, b) (				        \
    AVSME_COMPARE(a, b, ASCII)			            \
    || (									        \
        AVSME_COMPARE(a, b, MAINCLASS) 	            \
        && (                                        \
            !AVSME_GET(a, SUBCLASS)                 \
            ||                                      \
            AVSME_COMPARE(a, b, SUBCLASS)           \
        )								            \
    )                                               \
)

#define AVSME_VARIANT(a) (AVSME_GET(a, VARIANCE))
#define AVSME_EXISTENT(a) (AVSME_GET(a, EXISTS))

typedef uint64_t hash;
extern hash __token_meta;

#define hash_push __token_meta *= 1099511628211ULL; __token_meta ^= (uint8_t)
#define hash_value (__token_meta)
#define hash_set(_value) hash_value = hash_of(_value)
#define hash_reset __token_meta = 14695981039346656037ULL
#define hash_none 14695981039346656037ULL

hash hash_function_fnv(char * _str);

#define hash_function(_str) hash_function_fnv(_str)

#define hash_of(_any) (_Generic((_any),             \
    char * : (hash_function_fnv((char *)_any)),     \
    default : (hash)_any                            \
    ))

struct TL_hash_link {
    hash value;
    struct TL_hash_link * next;
};

#define CC_Type_token_list_static 3

#define CC_Type_token_list_dynamic 4

#define TL_token_list_view_0 struct TL_token_list_static_view
#define TL_token_list_view_ struct TL_token_list_dynamic_view

#define CC_Type_token_list(_name) _Generic( TL_TOKENLIST_BUFFER(_name), struct TL_hash_link : 4, default : 3 ) 

#define TL_TOKENLIST_TYPE(_name) mergetokens(TL_, _name, _type)
#define TL_TOKENLIST_IDEN(_name) mergetokens(TL_, _name, _iden)
#define TL_TOKENLIST_SIZE(_name) mergetokens(TL_, _name, _size)
#define TL_TOKENLIST_BUFFER(_name) mergetokens(TL_, _name, _buffer)
#define TL_TOKENLIST_ENABLE(_name) mergetokens(TL_, _name, _enable)
#define TL_TOKENLIST_LAST(_name) mergetokens(TL_, _name, _last)

#define TL_token_list_decl_(_name, ...)							\
char TL_TOKENLIST_TYPE(_name) = !(0 __VA_OPT__(+ 1));			\
char TL_TOKENLIST_IDEN(_name)[] = #_name;						\
size_t TL_TOKENLIST_SIZE(_name) = 0 __VA_OPT__(+ __VA_ARGS__)

#define TL_token_list_type_     struct TL_hash_link
#define TL_token_list_type_0    hash
#define TL_token_list_type(...)                                 \
__mergetoken(TL_token_list_type_, __VA_OPT__(0))

#define TL_token_list_type_exception_(...) = {0, NULL}
#define TL_token_list_type_exception_0(...) __VA_OPT__([__VA_ARGS__])
#define TL_token_list_type_exception(...)                       \
__mergetoken(TL_token_list_type_exception_, __VA_OPT__(0))(__VA_ARGS__)

#define TL_token_list_decl(_name, ...)		                    \
TL_token_list_decl_(_name, __VA_ARGS__);		                \
                                                                \
TL_token_list_type(__VA_ARGS__)				                    \
TL_TOKENLIST_BUFFER(_name)				                        \
TL_token_list_type_exception(__VA_ARGS__)

#define CC_ccast_token_list(_name)                              \
_Generic(                                                       \
    TL_TOKENLIST_BUFFER(_name),                                 \
    struct TL_hash_link : CC_ccast_token_list_dynamic,          \
    default : CC_ccast_token_list_static                        \
)                                                               \
(                                                               \
    TL_TOKENLIST_BUFFER(_name),                                 \
    TL_TOKENLIST_SIZE(_name),                                   \
    TL_TOKENLIST_IDEN(_name)                                    \
)

extern hash * TL_enable_buffer;
extern size_t TL_enable_i;
extern size_t TL_enable_max;

#define TL_token_list_static_append if (TL_enable_i >= TL_enable_max) {     \
        printf("\033[31m<! Token List Segmentation Fault !>\033[0m");       \
        exit(-1);                                                           \
    } TL_enable_buffer[ TL_enable_i++ ] = 

#define TL_token_list_enable_define(...) { __VA_ARGS__ }
#define TL_token_list_static_define(_name, _size)	                        \
void TL_TOKENLIST_ENABLE(_name) (void) TL_token_list_enable_define

#define TL_token_list_static_enable(_name)                                  \
TL_enable_buffer = TL_TOKENLIST_BUFFER(_name);                              \
TL_enable_i = 0;                                                            \
TL_enable_max = TL_TOKENLIST_SIZE(_name);                                   \
TL_TOKENLIST_ENABLE(_name)()

char TL_token_list_static_search(hash * _buffer, size_t _size, hash _search);

#define TL_token_list_dynamic_define(_name)	                                    \
struct TL_hash_link * TL_TOKENLIST_LAST(_name) = &TL_TOKENLIST_BUFFER(_name)

#define TL_token_list_dynamic_append(_name)	                                    \
TL_TOKENLIST_SIZE(_name)++;                                                     \
TL_TOKENLIST_LAST(_name)->next = malloc(sizeof(struct TL_hash_link));           \
TL_TOKENLIST_LAST(_name) = TL_TOKENLIST_LAST(_name)->next;                      \
TL_TOKENLIST_LAST(_name)->next = NULL;                                          \
TL_TOKENLIST_LAST(_name)->value =

char TL_token_list_dynamic_search(struct TL_hash_link _buffer, size_t _size, hash _search);

#define TL_token_list_define_0 TL_token_list_static_define
#define TL_token_list_define_  TL_token_list_dynamic_define
#define TL_token_list_define(...)  __mergetoken(TL_token_list_define_, __VA_OPT__(0))

#define token_list(_name, ...)                                      \
TL_token_list_decl(_name __VA_OPT__(,) __VA_ARGS__);                \
TL_token_list_define(__VA_ARGS__)(_name __VA_OPT__(,) __VA_ARGS__)

#define token_search(_name) _Generic( TL_TOKENLIST_BUFFER(_name),   \
    struct TL_hash_link : TL_token_list_dynamic_search,             \
    default : TL_token_list_static_search                           \
)

#define token_in(_name) ( token_search(_name) (TL_TOKENLIST_BUFFER(_name), TL_TOKENLIST_SIZE(_name), hash_value) )

#define token_not_in(_name) (!token_in(_name))

#define TL_token_append_(...) TL_token_list_static_append
#define TL_token_append_0(...) TL_token_list_dynamic_append(__VA_ARGS__)

#define token_list_enable TL_token_list_static_enable

#define token_append(...)  __mergetoken(TL_token_append_, __VA_OPT__(0))(__VA_ARGS__) hash_of

void TL_free_token_list(struct TL_hash_link * _first);

#define free_token_list(_name)                                      \
TL_TOKENLIST_SIZE(_name) = 0;                                       \
TL_free_token_list(TL_TOKENLIST_BUFFER(_name).next);                \
TL_TOKENLIST_BUFFER(_name).next = NULL;                             \
TL_TOKENLIST_LAST(_name) = NULL

void CC_ccast_token_list_static(hash * _buffer, size_t _size, char * _iden);

void CC_ccast_token_list_dynamic(struct TL_hash_link _buffer, size_t _size, char * _iden);

typedef struct CC_ccll {
    void * object;
    char type;
    struct CC_ccll * next;
} * CC_Link;

extern CC_Link CC_List;

struct CC_ccll CC_base = {NULL, 0, NULL};
CC_Link CC_List = &CC_base;
CC_Link CC_List_last = &CC_base;

#define CC_List_append(_type)                                               \
    CC_List_last->next = malloc(sizeof(struct CC_ccll));                    \
    CC_List_last = CC_List_last->next;                                      \
    CC_List_last->type = _type;                                             \
    CC_List_last->next = NULL;                                              \
    CC_List_last->object = 

#define CC_List_static_append(_datatype, _type)                             \
    _datatype mergetoken(CC_List_static_variable_, __LINE__);               \
    CC_List_last->next = &mergetoken(CC_List_static_variable_, __LINE__);   \
    CC_List_last = CC_List_last->next;                                      \
    CC_List_last->type = _type;                                             \
    CC_List_last->next = NULL;                                              \
    CC_List_last->object = 

#ifdef DYNAMIC_CHARCLASS

#define CC_Type_CHARCLASS 1

typedef avsme (* CHARCLASS_type)(char _char);

struct CHARCLASS_charclass_view {
    char * iden;
    avsme (* function)(char _char);
};

void CC_ccast_charclass_ASCII_function(struct CHARCLASS_charclass_view _charclass);

#define dynamic_charclass(_charclass) CHARCLASS_active = _charclass

extern avsme (* CHARCLASS_active)(char _char);
extern avsme * CHARCLASS_charclass_active;

#define charclass(c) CHARCLASS_active(c)

avsme CHARCLASS_none_active(char _char);

#define CC_append_dynamic_charclass(_charclass)                                         \
    char mergetokens(CHARCLASS_, _charclass, _name)[] = #_charclass;                    \
    struct CHARCLASS_charclass_view mergetokens(CHARCLASS_, _charclass, _view) = {      \
        mergetokens(CHARCLASS_, _charclass, _name),                                     \
        _charclass                                                                      \
    };                                                                                  \
    CC_List_append(1) &mergetokens(CHARCLASS_, _charclass, _view)

#endif

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

#define __VLU_PUSH_VALUE(name, i_key)   \
    name.table[                         \
        name.n_keys * name.max_values   \
        + ++(name.table[i_key][1]) + 1  \
    ]

#define VLU_PUSH_VALUE(name, i_key)	__VLU_PUSH_VALUE(name, i_key) =

struct collect_out {
    avsme char_class;
    char * old_token;
    char * new_token;
};

extern struct collect_out COLLECT_OUT_NULL;

extern struct collect_out COLLECT_OUT;

#define formatted_string_of_collected(out)    \
    "%.*s", out.new_token - out.old_token, out.old_token

#define print_collected(out)    \
    printf("%.*s", out.new_token - out.old_token, out.old_token)

#define paste_collected(buffer, out)				                \
    memcpy(buffer, out.old_token, out.new_token - out.old_token);   \
    buffer[(out.new_token - out.old_token)] = 0

size_t print_out_collective(struct collect_out out);

#define collects(_str)  \
    = collect_immediate_in(_str)).old_token && (1
        
struct collect_out collect_immediate(char * _str);

struct collect_out collect_immediate_in(char * _str);

size_t print_str_collective_immediate(char * _str);

#define collecting(_str, n_keys, table, _stride)  \
    = collect_variation_in(_str, n_keys, table, _stride)).old_token && (1

struct collect_out collect_variation    (
                                            char * 	_str,
                                            size_t 	n_keys,
                                            avsme * table,
                                            size_t 	_stride,
                                            size_t * countptr
                                        );

struct collect_out collect_variation_in     (
                                                char * _str,
                                                size_t n_keys,
                                                avsme * table,
                                                size_t _stride
                                            );

size_t print_str_collective_variation (
                                            char * 	_str,
                                            size_t 	n_keys,
                                            avsme * table,
                                            size_t 	_stride
                                        );

#define VLU_NO_VALUE_HANDLE goto VLU_ERROR_CHECK
#define VLU_NO_KEY_HANDLE goto VLU_ERROR_CHECK
#define VLU_ERROR_HANDLE VERLET_VLU_exit()
#define VLU_ERROR_RETURN_HANDLE COLLECT_OUT_NULL

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
#define VS_RECENT_VLU_MAX_KEYS          1
#define VS_RECENT_VLU_MAX_VALUES        1
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
                    )   \
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
                    )   \
    VS_RECENT_VLU.table[VS_RECENT_VLU.n_keys - 1] = (avsme *)realloc(                       \
                                            VS_RECENT_VLU.table[VS_RECENT_VLU.n_keys - 1],  \
                                            (VS_RECENT_VLU.max_values + 2) * sizeof(avsme)  \
                                            );                                              \
    VS_RECENT_VLU.n_values++;   \
    VS_RECENT_VLU.table[VS_RECENT_VLU.n_keys - 1][2 + (VS_RECENT_VLU.table[VS_RECENT_VLU.n_keys - 1][1]++)] = 

void VS__free_recent_VLU(void);

void VS_RECENT_VLU_print(void);

#define VS_VLU_print(_VLU)	                                                    \
                    __VS_VLU_print (                                            \
                                        _VLU.n_keys,                            \
                                        (avsme *)_VLU.table,                    \
                                        _VLU.max_values + 2                     \
                                    )

void __VS_VLU_print   (
                            size_t n_keys,
                            avsme * table,
                            size_t _stride
                        );

#define VS_ptr_size sizeof(uintptr_t)
#define VS_mode_span (sizeof(uintptr_t) / sizeof(avsme) + !!((8 * sizeof(uintptr_t)) % (8 * sizeof(avsme) - 1)))

typedef void (* VS_mode_ptr_type) (void);

#define VS_mode_to_buffer(_buffer, _address)                                                    \
    for (int VS_i = 0; VS_i < VS_mode_span; VS_i++)                                             \
    (_buffer)[VS_i] = (avsme)((((uintptr_t)_address) >> (VS_i*((sizeof(avsme)*8) - 1))) << 1)

#define VS_buffer_to_mode(_address, _buffer)                                                    \
    for (int VS_i = 0; VS_i < VS_mode_span; VS_i++)                                             \
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
    hash : (hash_value == (hash)cmp),                                                                               \
    char * : (hash_value == hash_function((char *)cmp)),                                                            \
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

#include <string.h>

_Noreturn void VERLET_VLU_exit(void);

avsme VERLET_charclass(char c);

extern char VS_OFF_CHARCLASS_ERR;

avsme VERLET_charclass_invalid_char_error(char c);

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

#ifdef DYNAMIC_CHARCLASS

#else

#ifndef charclass
#define charclass VERLET_charclass
#endif

#endif

#define VSCRIPT_INVALID_CHAR_CASE return VERLET_charclass_extend(c)

#define RW_save_path_buffer(_path, _size, ...) char uniq(RW_path_) [_size] __VA_OPT__(; memcpy(uniq(RW_path_), _path, _size); const char * uniq(RW_temp_)) = _path

#define RW_save_path(_path, ...) RW_save_path_buffer(_path, 1024 __VA_OPT__(,) __VA_ARGS__)

typedef struct RW_file_struct {
    FILE * file;
    char * path;
} RW_file_struct;

#define RW_new_file_struct_ RW_file_struct uniq(RW_fs_);

#define RW_set_path_ uniq(RW_fs_).path = uniq(RW_path_);

extern FILE * RW_recent_open;

#define RW_set_file_(_mode) uniq(RW_fs_).file = (RW_recent_open = fopen( uniq(RW_path_), _mode )); RW_file_struct

#define Opened (RW_recent_open)

#define Open(_path, ...) RW_save_path(_path __VA_OPT__(,) __VA_ARGS__); RW_new_file_struct_ RW_set_path_
#define OpenPtr(_path) Open(_path, 0)

#define Mode ; uniq(RW_fs_).file = (RW_recent_open = fopen(uniq(RW_path_),

#define In(_iden) )); RW_file_struct _iden = uniq(RW_fs_)

#define RW_init_Writer RW_file_struct RW_writer_; fpos_t RW_writer_pos;

extern RW_file_struct RW_writer_;
extern fpos_t RW_writer_pos;

#define Writer RW_writer_  = uniq(RW_fs_); RW_writer_.file = (RW_recent_open = fopen(uniq(RW_path_), "w")); 

#define WriteChar(_char) fputc(_char, RW_writer_.file)

#define WriteLine(_line) fputs(_line, RW_writer_.file)

#define WriteFormat(_format, ...) fprintf(RW_writer_.file, _format __VA_OPT__(,) __VA_ARGS__)

#define UpdateWriterPos fgetpos(RW_writer_.file, &RW_writer_pos)

#define WriterPosType fpos_t

#define WriterPos RW_writer_pos

#define WriterGoTo(_pos) fsetpos(RW_writer_.file, &_pos)

#define ResetWriter rewind(RW_writer_.file)

#define CloseWriter fclose(RW_writer_.file)

#define RW_init_Reader RW_file_struct RW_reader_; fpos_t RW_reader_pos;

extern RW_file_struct RW_reader_;
extern fpos_t RW_reader_pos;

#define Reader RW_reader_ = uniq(RW_fs_); RW_reader_.file = (RW_recent_open = fopen(uniq(RW_path_), "r"));

#define ReadChar(_char) for (int _char = (fgetpos(RW_reader_.file, &RW_reader_pos), fgetc(RW_reader_.file)); _char != EOF; (fgetpos(RW_reader_.file, &RW_reader_pos), _char = fgetc(RW_reader_.file)))

#define ReadLine(_line, _size) for (char _line[_size]; (fgetpos(RW_reader_.file, &RW_reader_pos), fgets(_line, sizeof(_line), RW_reader_.file));)

#define UpdateReaderPos fgetpos(RW_reader_.file, &RW_reader_pos)

#define ReaderPosType fpos_t

#define ReaderPos RW_reader_pos

#define ReaderGoTo(_pos) fsetpos(RW_reader_.file, &_pos)

#define ResetReader rewind(RW_reader_.file)

#define Once(_code) {_code; break;}

#define CloseReader fclose(RW_reader_.file)

void CC_ccast_charclass_ASCII(void);

#ifdef DYNAMIC_CHARCLASS

void CC_ccast_charclass_ASCII_function(struct CHARCLASS_charclass_view _charclass);

#endif

void CC_ccast_VLU(void);

void CC_ccast_VLU_function(struct VLU_VLU_view _VLU);

void CC_ccast(void);

struct CC_append_charclass_calls {
    #ifdef DYNAMIC_CHARCLASS
    void (* Dynamic)(struct CHARCLASS_charclass_view _charclass);
    #else
    void (* Static)(void);
    #endif
};

struct CC_append_VLU_calls {
    #ifdef STATIC_VLU
    void (* Static)(void);
    #else
    void (* Dynamic)(struct VLU_VLU_view _VLU);
    #endif
};

struct CC_append_tokenlist_calls {
    void (* Static)(void);
    void (* Dynamic)(struct VLU_VLU_view _VLU);
};

#define __INC_STDV

#endif
