//
// Verlet Utility Header
//
// 1.0
/*
This header provides many of the frequently used functions throught the entire Verlet Lexer Framework.
*/

#ifndef _INC_V_UTILS
#define _INC_V_UTILS

// vfilename : Splits a files full path and return's only the file name, if file name is the full path, returns the full path.
#define vfilename(path) ( strrchr(path, '/') ? strrchr(path, '/') + 1 : strrchr(path, '\\') ? strrchr(path, '\\') + 1 : path )

// IF_ARGS : Returns valid C expressions on the basis of __VA_ARGS__ existence.
#define __TYPE_ARGS(...) __VA_OPT__((int*))((int)0)
#define IF_ARGS(yes, no, ...) _Generic(__TYPE_ARGS(__VA_ARGS__), int*: yes, int: no)

/* To examine ASCII characters. */
#define __char_to_lower(_char) (_char - ('A' - 'a'))
#define __char_to_upper(_char) (_char - ('a' - 'A'))
#define __char_lower_alpha(_char) ('a' <= _char && _char <= 'z')
#define __char_upper_alpha(_char) ('A' <= _char && _char <= 'Z')
#define __char_alpha(_char) (__char_lower_alpha(_char) || __char_upper_alpha(_char))
#define __char_digit(_char) ('0' <= _char && _char <= '9')
#define __char_alnum(_char) (__char_digit(_char) || __char_alpha(_char))

#define __char_idvalid(_char) (__char_alpha(_char) || (_char == '_'))  // always valid in identifiers
#define __char_nsymbol(_char) (__char_alnum(_char) || (_char == '_'))  // not a symbol (not valid in the start of identifiers)

#define __char_symbol(_char) ((!__char_nsymbol(_char)))  // is a symbol

// stringof : Returns the string of everything that is passed to it.
#define __stringof(a) #a
#define stringof(a) __stringof(a)

/* To Merge tokens. */
#define __mergetoken(a, b) a##b
#define mergetoken(a, b) __mergetoken(a, b)

#define __mergetokens(a, b, c) a##b##c
#define mergetokens(a, b, c) __mergetokens(a, b, c)

#define __mergetokens4(a, b, c, d) a##b##c##d
#define mergetokens4(a, b, c, d) __mergetokens4(a, b, c, d)

// uniq : Returns a unique name for the name space that is passed to it.
// (it uses __LINE__ to encode newness, different files may have same uniq, always use with caution.)
#define uniq(_iden) mergetoken(_iden, __LINE__)

/* To alter the style of printed text. */
#define style_bold printf("\033[1m")
#define style_bold_reset printf("\033[22m")

#define style_italics printf("\033[3m")
#define style_italics_reset printf("\033[23m")

#define style_underline printf("\033[4m")
#define style_underline_reset printf("\033[24m")

/* To alter the color of the printed text. */
#define color_fore(r, g, b) printf("\033[38;2;%u;%u;%um", r, g, b)
#define color_back(r, g, b) printf("\033[48;2;%u;%u;%um", r, g, b)
#define color_fore_reset    printf("\033[39m")
#define color_back_reset    printf("\033[49m")

#define color_fore_grey(g)  color_fore(g, g, g)
#define color_back_grey(g)  color_back(g, g, g)

#define full_reset color_fore_reset; color_back_reset; style_bold_reset; style_italics_reset; style_underline_reset

/* To debug. */
#define debug_name "DEBUG"

#define debug_print(format, ...) printf("\033[1;34m\n[" debug_name "] \033[22m" format "\033[39m" __VA_OPT__(,) __VA_ARGS__)

#define debug_carry(i, format, ...) printf("\n %*.s" format "\033[39m", 4 * i, "" __VA_OPT__(,) __VA_ARGS__)

#define debug_continue(format, ...) printf("\033[33m" format "\033[39m" __VA_OPT__(,) __VA_ARGS__)

#define debug_bar "\033[34m|\033[39m"

/* Verlet Standard Implementation. */
#define v_implement "svh____init.h"

#endif