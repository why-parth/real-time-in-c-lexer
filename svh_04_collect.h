//
// Verlet Collect Header
//
// 1.0
/*
This header initialises token recognition (token collection).
*/

#ifndef _INC_V_COLLECT
#define _INC_V_COLLECT

#include "svh_03_vlut.h"

struct collect_out {
    avsme char_class;
    char * old_token;
    char * new_token;
};

extern struct collect_out COLLECT_OUT_NULL;

extern struct collect_out COLLECT_OUT;

#define print_collected(out)    \
    printf("%.*s", out.new_token - out.old_token, out.old_token)

#define paste_collected(buffer, out)				                \
    memcpy(buffer, out.old_token, out.new_token - out.old_token);   \
    buffer[(out.new_token - out.old_token)] = 0

#define collects(_str)  \
    = collect_immediate_in(_str)).old_token && (1

#define collecting(_str, n_keys, table, _stride)  \
    = collect_variation_in(_str, n_keys, table, _stride)).old_token && (1

size_t print_out_collective(struct collect_out out);

struct collect_out collect_immediate(char * _str);

struct collect_out collect_immediate_in(char * _str);

size_t print_str_collective_immediate(char * _str);

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

size_t print_str_collective_variation   (
                                            char * 	_str,
                                            size_t 	n_keys,
                                            avsme * table,
                                            size_t 	_stride
                                        );

#define token_is(cmp, ...) (_Generic(cmp,                                                                           \
    hash : (get_fnv == (hash)cmp),                                                                                  \
    char * : (get_fnv == __fnv((char *)cmp)),                                                                       \
    uint8_t : ( (COLLECT_OUT.char_class & AVSME_MAINCLASS) == _Generic((cmp), char *: 0, int *: 0, default : cmp)   \
        __VA_OPT__(&& (COLLECT_OUT.char_class & AVSME_SUBCLASS) == (0 __VA_ARGS__)) ),                              \
    default : (0)                                                                                                   \
))

#define token_is_not(cmp, ...) (!token_is(cmp __VA_OPT__(,) __VA_ARGS__))

#define class_is(cmp, ...) ( (COLLECT_OUT.char_class & AVSME_MAINCLASS) == cmp __VA_OPT__(&& (COLLECT_OUT.char_class & AVSME_SUBCLASS) == __VA_ARGS__) )

#define _REQ_INIT_V_COLLECT

#endif