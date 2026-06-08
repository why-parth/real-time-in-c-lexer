//
// Verlet Token List Header
//
// 1.0
/*
This header initialises token lists for Verlet Lexer.
*/

#ifndef _INC_V_TOKENLIST
#define _INC_V_TOKENLIST

#include "svh_06_readwrite.h"

extern size_t VCo_enable_i;
extern hash * VCo_recent_set;

#define __token_set(_name, __size)                           \
    hash _name[__size];                                      \
    size_t mergetokens(VCo_enable_, _name, _size) = __size;  \
    int mergetoken(VCo_enable_, _name) (void)

#define keyword(_kw, ...) VCo_recent_set[VCo_enable_i++ __VA_OPT__(* 0 + __VA_ARGS__)] = fnv(#_kw)

#define keyword_str(_kw, ...) VCo_recent_set[VCo_enable_i++ __VA_OPT__(* 0 + __VA_ARGS__)] = fnv(_kw)

#define keyword_hash(_kw, ...) VCo_recent_set[VCo_enable_i++ __VA_OPT__(* 0 + __VA_ARGS__)] = _kw

#define VCo_enable_token_set_init(_name) for (int i = VCo_enable_i; i < sizeof(_name)/sizeof(hash); i++) _name[i] = 0;

#define enable_token_set(_name, ...) VCo_enable_i = 0; VCo_recent_set = _name; __VA_OPT__(__VA_ARGS__ =) mergetoken(VCo_enable_, _name)(); VCo_enable_token_set_init(_name)

#define token_set_len VCo_enable_i

#define select_token_set(_name) VCo_recent_set = _name

int VCo_search_set(hash * _set, size_t _size);

#define token_in_set(_set_name) ( VCo_search_set((hash *)_set_name, mergetokens(VCo_enable_, _set_name, _size)) )

struct VCo_hash_link {
    hash value;
    struct VCo_hash_link * next;
};

struct VCo_hash_link * VCo_new_hash_link(hash _token);

#define new_hash_link(_token) VCo_new_hash_link(cast_to_fnv(_token))

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

#define token_in_list(_name, ...) (VCo_search_hash_list( _name, __VA_ARGS__ __VA_OPT__(,) get_fnv))

// - Macro Overloading

#define VCo_token_list_type_(_name, ...) __token_list(_name)
#define VCo_token_list_type_0(_name, ...) __token_set(_name, __VA_ARGS__)
#define VCo_token_list_type(...) __mergetoken(VCo_token_list_type_, __VA_OPT__(0))

#define token_list(_name, ...) VCo_token_list_type(__VA_ARGS__)(_name __VA_OPT__(,) __VA_ARGS__)

// - Macro Handling

#define token_in(_name, ...) (_Generic((_name),                             \
    hash * : token_in_set(_name),                                   \
    struct VCo_hash_link * : token_in_list((struct VCo_hash_link *)_name) \
))

#define token_not_in(_name, ...) (!token_in(_name __VA_OPT__(,) __VA_ARGS__))

#endif