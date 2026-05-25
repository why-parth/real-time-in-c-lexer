//
// Standard Verlet Header
//

#if !defined(VERLET_COLLECT)
#define VERLET_COLLECT

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "vutils.h"

typedef uint_least16_t avsme;
/* AVSME's definition. */

#define AVSME_NULL 0
/* AVSME 0 */

struct collect_out {
    avsme char_class; // Char class of the collect’s recently scanned token.
    char * old_token; // Pointer to collect’s recently scanned token.
    char * new_token; // Pointer to where collect’s recently scanned token ends.
};
/* Return type of collect_in functions. */

extern struct collect_out COLLECT_OUT_NULL;
/* Represents nothing in terms of struct collect_out. */

#define print_collected(out) printf("%.*s", out.new_token - out.old_token, out.old_token)
// print_token(out) prints the token that the out corresponds to

#define paste_collected(buffer, out)									\
memcpy(buffer, out.old_token, out.new_token - out.old_token);		    \
buffer[out.new_token - out.old_token] = 0
// paste_token(buffer, out) pastes the token in the buffer that out corresponds to

#define collects(_str) = collect_immediate_in(_str)).old_token && (1
// while (( out collects(string) )) { ... } is initialised, the double brackets mustn't be removed, it's a convention of this macro.

#define collecting(_str, _vlut) = collect_variation_in(_str, _vlut)).old_token && (1
// while (( out collecting(string, vlut) )) { ... } is initialised, the double brackets mustn't be removed, it's a convention of this macro.

#define __collecting(_str, n_keys, table, _stride) = __collect_variation_in(_str, n_keys, table, _stride)).old_token && (1
// while (( out __collecting(string, n_keys, table, _stride) )) { ... } is initialised, the double brackets mustn't be removed, it's a convention of this macro.


// C23 Specific Syntax : Bit Masks for the AVSME data layout
#define AVSME_ASCII 		0b1111111100000000
#define AVSME_VARIANCE 	    0b0000000010000000
#define AVSME_SUBCLASS  	0b0000000001100000
#define AVSME_MAINCLASS 	0b0000000000011110
#define AVSME_EXISTS 		0b0000000000000001

// Shifts for each Bit Mask for the AVSME data layout
#define AVSME_ASCII_SHIFT 		    8
#define AVSME_VARIANCE_SHIFT 		7
#define AVSME_SUBCLASS_SHIFT 		5
#define AVSME_MAINCLASS_SHIFT 	    1
#define AVSME_EXISTS_SHIFT 		    0

/* These macros make getting and setting the AVSME data extremely easy. */

#define AVSME_BIT(_name, _mask) 									            \
( _name & mergetoken(AVSME_, _mask) )
// Gets the Bit Mask

#define AVSME_GET(_name, _mask) 									            \
(( _name & mergetoken(AVSME_, _mask) )							                \
>> mergetokens(AVSME_, _mask, _SHIFT))
// Gets the Bit Mask’s value

#define AVSME_SET(_name, _mask, _value) 							            \
((_name & ~mergetoken(AVSME_, _mask))							                \
| (_value << mergetokens(AVSME_, _mask, _SHIFT)))
// Sets the Bit Mask’s value

#define __VLUT_typedef_pointer_to_array(name, max_values)   					\
    typedef avsme (* mergetoken(name, _VLUT_TABLE_t))[max_values + 2]
/* This macro creates a UID typedef of a pointer-to avsme array of size max_value + 2. The max_value corresponds to the maximum number of values that can be stored in an entry (+ 2 to account for storing keys and the number of values). */

#define __VLUT_key_value_declare(name, max_keys, max_values) 					\
    avsme mergetoken(__VLUT__, __LINE__)[max_keys][max_values + 2]
/* This macro creates a 2D UID array of the given dimensions {max_keys, max_values + 2} (+ 2 to account for storing keys and the number of values). */

#define __VLUT_variable_declare(name, max_values) 			        			        \
    typedef struct { size_t n_keys; volatile mergetoken(name, _VLUT_TABLE_t) table; }   \
    mergetoken(name, _VLUT_t); mergetoken(name, _VLUT_t) name; name.n_keys = 0;         \
    name.table = mergetoken(__VLUT__, __LINE__)
/* This macro defines a struct especially to use the UID typedef from the macro __VLUT_typedef_pointer_to_array and then creates its instance whose n_keys is set 0 and table is set to UID array from __VLUT_key_value_declare. */

#define VLUT_DECLARE(name, max_keys, max_values) 		    					\
    __VLUT_typedef_pointer_to_array(name, max_values);      				    \
    __VLUT_key_value_declare(name, max_keys, max_values);		    			\
    __VLUT_variable_declare(name, max_values)							
/* This macro includes all the macros sequentially (in line) to maintain the UID mechanism. */

#define __VLUT_PUSH_KEY(name)			    name.table[name.n_keys + (name.table[name.n_keys][1] = 0)][ ((name.n_keys)++, 0) ]
#define VLUT_PUSH_KEY(name)			    __VLUT_PUSH_KEY(name) =
/* This macro pushes a key onto a VLUT. */
 
#define __VLUT_PUSH_VALUE(name, i_key)	name.table[i_key][ ++(name.table[i_key][1]) + 1 ]
#define VLUT_PUSH_VALUE(name, i_key)	__VLUT_PUSH_VALUE(name, i_key) =
/* This macro pushes a value onto an entry (corresponding to a key)  of a VLUT. */

#define VLUT_N_KEY                      .n_keys
#define VLUT_N_VALUE(i_key)             .table[i_key][1]
/* These macros return the number of keys and values. */

#define VLUT_KEY(i_key)                 .table[i_key][0]
#define VLUT_VALUE(i_key, i_value)      .table[i_key][i_value + 2]
/* These macros return the keys and values. */
 
#define AVSME_COMPARE(a, b, _mask) (AVSME_BIT(a, _mask) == AVSME_BIT(b, _mask))
/* This macro compares specific bit masks of two AVSMEs. */

extern avsme AVSME_NONE;
extern avsme AVSME_TRUE;
extern avsme AVSME_FALSE;
/* Boolean AVSMEs. */

#define AVSME_BOOLEAN(a)                                                                            \
(((a >> AVSME_ASCII_SHIFT == 1) || (a >> AVSME_ASCII_SHIFT == 1)) ? AVSME_TRUE                      \
: ( ((a >> AVSME_ASCII_SHIFT == 2) || (a >> AVSME_ASCII_SHIFT == 2)) ? AVSME_FALSE : AVSME_NONE ))
/* This macro allows boolean logic with AVSMEs. */

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
/* This macro compares ASCII, Sub-class and Class bit masks of the two AVSMEs.*/

#define AVSME_VARIANT(a) AVSME_GET(a, VARIANCE)
/* This macro returns the variance of the AVSME.*/

typedef uint64_t hash;
extern hash __token_meta;

#define push_fnv __token_meta *= 1099511628211ULL; __token_meta ^= (uint8_t)
#define get_fnv (__token_meta)
#define reset_fnv __token_meta = 14695981039346656037ULL

hash __fnv(char * str, ...);

#define fnv(_token, ...) __VA_OPT__(_token; _token =) __fnv(__VA_ARGS__ __VA_OPT__(,) _token )

#define token_is(cmp, ...) (_Generic((cmp),                                                     \
    hash : (get_fnv == (hash)cmp),                                                              \
    char * : (get_fnv == __fnv((char *)cmp)),                                                   \
    uint8_t : ( (out.char_class & AVSME_MAINCLASS) == _Generic((cmp), char *: 0, default : cmp) \
        __VA_OPT__(&& (out.char_class & AVSME_SUBCLASS) == (0 __VA_ARGS__)) ),                    \
    default : (0)                                                                               \
))

#define class_is(cmp, ...) ( (out.char_class & AVSME_MAINCLASS) == cmp __VA_OPT__(&& (out.char_class & AVSME_SUBCLASS) == __VA_ARGS__) )

#define VERLET_COLLECT_IMPL

// #elif !defined(VERLET_COLLECT_SECONDARY)
// #define VERLET_COLLECT_SECONDARY


struct collect_out collect_immediate(char * _str);


struct collect_out collect_immediate_in(char * _str);


size_t print_out_collective(struct collect_out out);


size_t print_str_collective_immediate(char * _str);


#define collect_variation(_str, _vlut)                                              \
                   __collect_variation  (										    \
                                            _str,								    \
                                            _vlut.n_keys,						    \
                                            (avsme *)_vlut.table,					\
                                            sizeof(*(_vlut.table)) / sizeof(avsme)	\
                                            0\
                                        ) /* Parameter Extraction Layer */

struct collect_out __collect_variation  (
                                            char * 	_str,
                                            size_t 	n_keys,
                                            avsme * table,
                                            size_t 	_stride,
                                            size_t * countptr
                                        ); /* Actual Function */


#define collect_variation_in(_str, _vlut)	                                            \
                   __collect_variation_in   (					                        \
                                                _str,                                   \
                                                _vlut.n_keys,                           \
                                                (avsme *)_vlut.table,                   \
                                                sizeof(*(_vlut.table)) / sizeof(avsme)  \
                                            ) /* Parameter Extraction Layer */

struct collect_out __collect_variation_in   (
                                                char * _str,
                                                size_t n_keys,
                                                avsme * table,
                                                size_t _stride
                                            ); /* Actual Function */


#define print_str_collective_variation(_str, _vlut)                                 \
                   __print_str_collective_variation  (								\
                                            _str,								    \
                                            _vlut.n_keys,						    \
                                            (avsme *)_vlut.table,					\
                                            sizeof(*(_vlut.table)) / sizeof(avsme)	\
                                        ) /* Parameter Extraction Layer */

size_t __print_str_collective_variation (
                                            char * 	_str,
                                            size_t 	n_keys,
                                            avsme * table,
                                            size_t 	_stride
                                        ); /* Actual Function */

#endif

// v0.1.1 -> VLUT_PUSH_KEY is changed so that IF_ARGS supports it