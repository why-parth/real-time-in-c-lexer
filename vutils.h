#ifndef __VUTILS__
#define __VUTILS__

#define vfilename(path) ( strrchr(path, '/') ? strrchr(path, '/') + 1 : strrchr(path, '\\') ? strrchr(path, '\\') + 1 : path )

#define __TYPE_ARGS(...) __VA_OPT__((int*))0
#define IF_ARGS(yes, no, ...) _Generic(__TYPE_ARGS(__VA_ARGS__), int*: yes, int: no)

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

#define __is_token_at(_str, _strlen, _pos, _len)                    \
    ((_pos == 0 || __char_symbol(_str[_pos - 1])) &&                \
     (_pos + _len >= _strlen || __char_symbol(_str[_pos + _len])))  \
    ? (1 + 2*(_pos + _len >= _strlen) + (_pos == 0))                \
    : 0

#define __mergetoken(a, b) a##b
#define mergetoken(a, b) __mergetoken(a, b)

#define __mergetokens(a, b, c) a##b##c
#define mergetokens(a, b, c) __mergetokens(a, b, c)

#define __mergetokens4(a, b, c, d) a##b##c##d
#define mergetokens4(a, b, c, d) __mergetokens4(a, b, c, d)

#define uniq(_iden) mergetoken(_iden, __LINE__)

#define style_bold printf("\033[1m");
#define style_bold_reset printf("\033[22m");

#define style_italics printf("\033[3m");
#define style_italics_reset printf("\033[23m");

#define style_underline printf("\033[4m");
#define style_underline_reset printf("\033[24m");

#define color_fore(r, g, b) printf("\033[38;2;%u;%u;%um", r, g, b)
#define color_back(r, g, b) printf("\033[48;2;%u;%u;%um", r, g, b)
#define color_fore_reset    printf("\033[39m")
#define color_back_reset    printf("\033[49m")

#define color_fore_grey(g)  color_fore(g, g, g)
#define color_back_grey(g)  color_back(g, g, g)

#define full_reset color_fore_reset; color_back_reset; style_bold_reset; style_italics_reset; style_underline_reset

#define ifelse(condition, iftrue) (condition) ? iftrue :

#define debug_name "DEBUG"

#define debug_print(format, ...) printf("\033[1;34m\n[" debug_name "] \033[22m" format "\033[39m" __VA_OPT__(,) __VA_ARGS__)

#define debug_carry(i, format, ...) printf("\n %*.s" format "\033[39m", 4 * i, "" __VA_OPT__(,) __VA_ARGS__)

#define debug_continue(format, ...) printf("\033[33m" format "\033[39m" __VA_OPT__(,) __VA_ARGS__)

#define debug_bar "\033[34m|\033[39m"

#define v_script "vscript.h"
#define v_implement "verlet_std.h"
#define v_script_init "init_vscript.c"
#define v_collect "init_vscript.c"
#define v_collect_init "init_vcollect.c"

#endif