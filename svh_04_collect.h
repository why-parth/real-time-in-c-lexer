//
// Verlet Collect Header
//
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

extern struct collect_out out;

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

#define _REQ_INIT_V_COLLECT

#endif