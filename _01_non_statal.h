

#ifndef _INC_V_NON_STATAL
#define _INC_V_NON_STATAL

// Data Structure : avsme

#include "svh_00_utils.h"
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
( ((AVSME_GET(a, ASCII)) == 1) ? AVSME_TRUE       \
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
#define hash_reset __token_meta = 14695981039346656037ULL
#define hash_none 14695981039346656037ULL

hash hash_function_fnv(char * _str);

#define hash_function(_str) hash_function_fnv(_str)

#define hash_of(_any) (_Generic((_any), \
    char * : (hash_function_fnv((char *)_any)),     \
    default : (hash)_any                \
    ))

#endif
