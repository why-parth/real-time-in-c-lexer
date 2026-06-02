//
// Verlet Hash Header
//
/*
This header initialises a FNV hash function and buffer for faster token operation.
*/

#ifndef _INC_V_HASH
#define _INC_V_HASH

#include "svh_01_avsme.h"

typedef uint64_t hash;
extern hash __token_meta;

#define push_fnv __token_meta *= 1099511628211ULL; __token_meta ^= (uint8_t)
#define get_fnv (__token_meta)
#define reset_fnv __token_meta = 14695981039346656037ULL

hash __fnv(char * str, ...);

#define cast_to_fnv(_token) (_Generic((_token), \
    char * : (__fnv((char *)_token)),           \
    default : (hash)_token                      \
))

#define fnv(_token, ...) __VA_OPT__(_token; _token =) __fnv(__VA_ARGS__ __VA_OPT__(,) _token )

#define token_is(cmp, ...) (_Generic((cmp),                                                     \
    hash : (get_fnv == (hash)cmp),                                                              \
    char * : (get_fnv == __fnv((char *)cmp)),                                                   \
    uint8_t : ( (out.char_class & AVSME_MAINCLASS) == _Generic((cmp), char *: 0, default : cmp) \
        __VA_OPT__(&& (out.char_class & AVSME_SUBCLASS) == (0 __VA_ARGS__)) ),                  \
    default : (0)                                                                               \
))

#define token_is_not(cmp, ...) (!token_is(cmp __VA_OPT__(,) __VA_ARGS__))

#define class_is(cmp, ...) ( (out.char_class & AVSME_MAINCLASS) == cmp __VA_OPT__(&& (out.char_class & AVSME_SUBCLASS) == __VA_ARGS__) )

#endif