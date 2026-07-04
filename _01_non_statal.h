

#ifndef _INC_V_NON_STATAL
#define _INC_V_NON_STATAL

// Data Structure : avsme

#include "_00_utils.h"
#include <stdint.h>

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

#define AVSME_GET(_name, _mask)     \
(( _name & mergetoken(AVSME_, _mask) ) >> mergetokens(AVSME_, _mask, _SHIFT))

#define AVSME_SET(_name, _mask, _value)     \
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
// && AVSME_GET(a, EXISTS)

#define AVSME_OVERLAP(a, b) (				    \
    AVSME_COMPARE(a, b, ASCII)			        \
    || (									    \
        AVSME_COMPARE(a, b, MAINCLASS) 	        \
        && (                                    \
            !AVSME_GET(a, SUBCLASS)             \
            ||                                  \
            AVSME_COMPARE(a, b, SUBCLASS)       \
        )								        \
    )                                           \
)

#define AVSME_VARIANT(a) (AVSME_GET(a, VARIANCE))
#define AVSME_EXISTENT(a) (AVSME_GET(a, EXISTS))

// Data Structure : hash

typedef uint64_t hash;
extern hash __token_meta;

#define hash_push __token_meta *= 1099511628211ULL; __token_meta ^= (uint8_t)
#define hash_value (__token_meta)
#define hash_set(_value) hash_value = hash_of(_value)
#define hash_reset __token_meta = 14695981039346656037ULL
#define hash_none 14695981039346656037ULL

hash hash_function_fnv(char * _str);

#define hash_function(_str) hash_function_fnv(_str)

#define hash_of(_any) (_Generic((_any), \
    char * : (hash_function_fnv((char *)_any)),     \
    default : (hash)_any                \
    ))

// Data Structure : Token Lists (Re Done)

struct TL_hash_link {
    hash value;
    struct TL_hash_link * next;
};

#define TL_TOKENLIST_TYPE(_name) mergetokens(TL_, _name, _type)
#define TL_TOKENLIST_IDEN(_name) mergetokens(TL_, _name, _iden)
#define TL_TOKENLIST_SIZE(_name) mergetokens(TL_, _name, _size)
#define TL_TOKENLIST_BUFFER(_name) mergetokens(TL_, _name, _buffer)
#define TL_TOKENLIST_ENABLE(_name) mergetokens(TL_, _name, _enable)
#define TL_TOKENLIST_LAST(_name) mergetokens(TL_, _name, _last)

// This macro partially declares the token list
// Type, Iden, Size, all of these are stored statically.
#define TL_token_list_decl__(_name, ...)							\
char TL_TOKENLIST_TYPE(_name) = !(0 __VA_OPT__(+ 1));					\
char TL_TOKENLIST_IDEN(_name)[] = #_name;							\
size_t TL_TOKENLIST_SIZE(_name) = 0 __VA_OPT__(+ __VA_ARGS__)

// These (2) macro completes the declarataion
#define TL_token_list_type_     struct TL_hash_link
#define TL_token_list_type_0    hash
#define TL_token_list_type(...)         \
__mergetoken(TL_token_list_type_, __VA_OPT__(0))

#define TL_token_list_type_exception_(...) = {0, NULL}
#define TL_token_list_type_exception_0(...) __VA_OPT__([__VA_ARGS__])
#define TL_token_list_type_exception(...) \
__mergetoken(TL_token_list_type_exception_, __VA_OPT__(0))(__VA_ARGS__)

// This macro completely declares the token list
#define TL_token_list_decl(_name, ...)		            \
TL_token_list_decl__(_name, __VA_ARGS__);		        \
                                                        \
TL_token_list_type(__VA_ARGS__)				            \
TL_TOKENLIST_BUFFER(_name)				                \
TL_token_list_type_exception(__VA_ARGS__)


// STATIC

extern hash * TL_enable_buffer;
extern size_t TL_enable_i;
extern size_t TL_enable_max;

#define TL_token_list_static_append if (TL_enable_i >= TL_enable_max) { \
        printf("\033[31m<! Token List Segmentation Fault !>\033[0m");   \
        exit(-1);                                                       \
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

// DYNAMIC

#define TL_token_list_dynamic_define(_name)	                                    \
struct TL_hash_link * TL_TOKENLIST_LAST(_name) = &TL_TOKENLIST_BUFFER(_name)

#define TL_token_list_dynamic_append(_name)	                            \
TL_TOKENLIST_SIZE(_name)++;                                             \
TL_TOKENLIST_LAST(_name)->next = malloc(sizeof(struct TL_hash_link));   \
TL_TOKENLIST_LAST(_name) = TL_TOKENLIST_LAST(_name)->next;              \
TL_TOKENLIST_LAST(_name)->next = NULL;                                  \
TL_TOKENLIST_LAST(_name)->value =

char TL_token_list_dynamic_search(struct TL_hash_link _buffer, size_t _size, hash _search);

// INTERFACE

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

#define free_token_list(_name)                          \
TL_TOKENLIST_SIZE(_name) = 0;                           \
TL_free_token_list(TL_TOKENLIST_BUFFER(_name).next);    \
TL_TOKENLIST_BUFFER(_name).next = NULL;                 \
TL_TOKENLIST_LAST(_name) = NULL

// C Cast Init

union TL_token_list_buffer {
    hash static_buffer;
    struct TL_hash_link dynamic_buffer;
};

struct TL_token_list_view {
    char type;
    char * iden;
    size_t size;
    union TL_token_list_buffer buffer;
};

#define CC_Type_token_list(_name) _Generic( TL_TOKENLIST_BUFFER(_name), struct TL_hash_link : 4, default : 3 )

void CC_ccast_token_list_static(hash * _buffer, size_t _size, char * _iden);

void CC_ccast_token_list_dynamic(struct TL_hash_link _buffer, size_t _size, char * _iden);


// Data Structure : CCast's Linked List (CC_ccll)

typedef struct CC_ccll {
    void * object;
    char type;
    struct CC_ccll * next;
} * CC_Link;

extern CC_Link CC_List;

struct CC_ccll CC_base = {NULL, 0, NULL};
CC_Link CC_List = &CC_base;
CC_Link CC_List_last = &CC_base;

#define CC_List_append(_type)                               \
    CC_List_last->next = malloc(sizeof(struct CC_ccll));    \
    CC_List_last = CC_List_last->next;                      \
    CC_List_last->type = _type;                             \
    CC_List_last->next = NULL;                              \
    CC_List_last->object = 

#define CC_List_static_append(_datatype, _type)                             \
    _datatype mergetoken(CC_List_static_variable_, __LINE__);               \
    CC_List_last->next = &mergetoken(CC_List_static_variable_, __LINE__);   \
    CC_List_last = CC_List_last->next;                                      \
    CC_List_last->type = _type;                                             \
    CC_List_last->next = NULL;                                              \
    CC_List_last->object = 

#endif
