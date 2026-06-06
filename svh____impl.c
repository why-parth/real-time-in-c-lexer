#include <stdlib.h>

#if defined(_INC_V_AVSME) && !defined(_IMPL_V_AVSME)
#define _IMPL_V_AVSME

avsme AVSME_NONE = AVSME_SET(0, ASCII, 0);
avsme AVSME_TRUE = AVSME_SET(0, ASCII, 1);
avsme AVSME_FALSE = AVSME_SET(0, ASCII, 2);

#endif

#if defined(_INC_V_HASH) && !defined(_IMPL_V_HASH)
#define _IMPL_V_HASH

hash __token_meta = 14695981039346656037ULL;

hash __fnv(char * str, ...) {
    if (!str) return 0;
    uint64_t hash = 14695981039346656037ULL;
    if (!*str) return hash;
    do
    {
        hash *= 1099511628211ULL; 
        hash ^= (uint8_t)(*str);    
    }
    while ( *(++str) );
    return hash;
}

#endif

#if defined(_INC_V_COLLECT) && !defined(_IMPL_V_COLLECT) && defined(_INIT_V_COLLECT)
#define _IMPL_V_COLLECT

struct collect_out COLLECT_OUT_NULL = {AVSME_NULL, NULL, NULL};
struct collect_out COLLECT_OUT = {AVSME_NULL, NULL, NULL};


struct collect_out collect_immediate(char * _str) {
    if(!_str) return COLLECT_OUT_NULL;
    /* If the given string is NULL, char class is -1 and the pointers to tokens are NULL. */
    else if(!*_str) return COLLECT_OUT_NULL;
    /* If the given string is empty, char class is 0 and the pointers to tokens are NULL. */

    avsme char_class = charclass(_str[0]); // Char class of the first char.
    char * current_token = _str++; // Pointer to the current token (updates _str)

    while ((*_str) && (char_class == charclass(*_str))) _str++;
    /* Traversing the string unless the char class changes. */
    
    struct collect_out ret = {char_class, current_token, _str};
    return ret;
    /* Returning the collect_out of the char class and the pointers. */
}

struct collect_out collect_immediate_in(char * _str) {
    static char * current_string = NULL;
    /* To be able to reset when the given string changes. */

    static struct collect_out out;
    /* To store the collect’s return value. */

    if (_str != current_string) {
        /* If the given string doesn’t match the string that we’ve been keeping track of, all the tracking resets. Thus, new tracking for new strings. */
        current_string = _str;
        out.new_token = _str;
    }

    if (!*(out.new_token)) {
        // If the out terminates, then reset the static string.
        current_string = NULL;
        return COLLECT_OUT_NULL;
    }

    out = collect_immediate(out.new_token); // Collection of token and pointers to tokens

    return out; // Returning collect_out
}

size_t print_out_collective(struct collect_out out) {
    size_t count = (size_t)(out.new_token - out.old_token); // number of char(s)

    for (int i = 0; i < count; i++) printf("%d", out.char_class); // printing of charclass

    return count; // returning the length of printed string
}

size_t print_str_collective_immediate(char * _str) {
    struct collect_out out; /* To store the return value of collect. */
    size_t count; /* To store the number of char(s). */
    size_t ret; /* To store the number of printed char(s). */

    while (( out collects(_str) )) {
        // number of char(s)
        ret += (count = (size_t)(out.new_token - out.old_token));

        // printing of charclass
        for (int i = 0; i < count; i++) printf("%d", AVSME_GET(out.char_class, MAINCLASS));
    }

    collect_immediate_in(NULL); // resetting collect_in
    return ret; // returning the length of printed string
}

// #define DB_collect_variation_in
#undef debug_name
#define debug_name "COLLECT"

struct collect_out collect_variation  (
                                            char * 	_str,
                                            size_t 	n_keys,
                                            avsme * table,
                                            size_t 	_stride,
                                            size_t * countptr
                                        ) /* Actual Function */
{
    if(!_str) return COLLECT_OUT_NULL;
    /* If the given string is NULL, char class is -1 and the pointers to tokens are NULL. */
    else if(!*_str) return COLLECT_OUT_NULL;
    /* If the given string is empty, char class is 0 and the pointers to tokens are NULL. */

    avsme char_class = charclass(*_str);	// Char class of the first char.
    avsme ret_char_class = char_class;	    // Char class of the return collect out.
    push_fnv *(_str);
    avsme new_char_class; 				    // Char class of the new char.
    char * current_token = _str; 			// Pointer to the current token (updates _str)
    avsme check_value;
    avsme check_key;
    int count = (countptr) ? * countptr + 1 : 0;

    int check_key_ascii;
    int check_value_ascii;
    int check_key_bool;
    int check_value_bool;
    
    #ifdef DB_collect_variation_in
        debug_print("scanning (%zu) input : ", count - 1);
        debug_continue("'%c' : (fullclass) %d%d ", *_str, AVSME_GET(char_class, MAINCLASS), AVSME_GET(char_class, SUBCLASS));
    #endif

    for ( ; *(++_str) ; )
    {   
        new_char_class = charclass(*_str);
        /* Traversal of the string. Assignment of the new char class. */

        #ifdef DB_collect_variation_in
            debug_print("scan : %zu | ", count);

            if AVSME_COMPARE(char_class, new_char_class, ASCII) debug_continue("'%c' : (ascii) %d ", *_str, *_str);
            else debug_continue("'%c' : (fullclass) %d%d ", *_str, AVSME_GET(new_char_class, MAINCLASS), AVSME_GET(new_char_class, SUBCLASS));
        
            debug_print("VLU : ");
        #endif

        for (int i = 0; i < n_keys; i++) {
            /* Char Class comparison, if not equal, iterating over table to table for new char class’s entry. */
            check_key = table[i*_stride];
            check_key_bool = (AVSME_BOOLEAN(check_key) == AVSME_TRUE);
            check_key_ascii = AVSME_GET(check_key, ASCII);
            
            #ifdef DB_collect_variation_in
                debug_continue(" [%d] %c%c%c -> %d%d ?",
                    i + 1,

                    (check_key_ascii) ? ((check_key_bool) ? 'T' : '\'') : ((AVSME_GET(check_key, MAINCLASS) + '0')),
                    (check_key_ascii) ? ((check_key_bool) ? 'R' : check_key_ascii) : (AVSME_GET(check_key, SUBCLASS) + '0'),
                    (check_key_ascii) ? ((check_key_bool) ? 'U' : '\'') : 0,

                    AVSME_GET(char_class, MAINCLASS),
                    AVSME_GET(char_class, SUBCLASS));
            #endif

            if ( check_key_bool || AVSME_OVERLAP(check_key, char_class) )
            {
                
                #ifdef DB_collect_variation_in
                    debug_carry(1, "key matched");
                    printf(" " debug_bar);
                #endif

                for (int j = 2; j < table[i*_stride + 1] + 2; j++) {
                /* If the char class entry is found, iterating over all the values of the found entry. */
                    check_value = table[i*_stride + j];
                    check_value_bool = (AVSME_BOOLEAN(check_value) == AVSME_TRUE);
                    check_value_ascii = AVSME_GET(check_value, ASCII);

                    #ifdef DB_collect_variation_in
                        debug_continue(" [%d] %c%c%c -> %d%d ?",
                            j - 1,

                            (check_value_ascii) ? ((check_value_bool) ? 'T' : '\'') : ((AVSME_GET(check_value, MAINCLASS) + '0')),
                            (check_value_ascii) ? ((check_value_bool) ? 'R' : check_value_ascii) : (AVSME_GET(check_value, SUBCLASS) + '0'),
                            (check_value_ascii) ? ((check_value_bool) ? 'U' : '\'') : 0,

                            AVSME_GET(new_char_class, MAINCLASS),
                            AVSME_GET(new_char_class, SUBCLASS)
                        );
                    #endif
                    
                    if ( check_value_bool || AVSME_OVERLAP(check_value, new_char_class) )  {
                        /* If the entry contains the new char class. */

                        #ifdef DB_collect_variation_in
                            debug_carry(2, "value matched");
                            printf(" " debug_bar);
                        #endif

                        if ( AVSME_VARIANT(check_value) ) // if the entry is for variance,
                        {
                            
                            #ifdef DB_collect_variation_in
                                debug_continue(" variant? \033[0myes");
                            #endif

                            if (countptr) (*countptr) = count + 1;

                            struct collect_out ret = {ret_char_class, current_token, _str};
                            return ret; // return.
                        }
                        else {

                            #ifdef DB_collect_variation_in
                                debug_continue(" variant? \033[0mno");
                            #endif

                            push_fnv *(_str);

                            if (AVSME_BOOLEAN(table[i*_stride + j + 1]) == AVSME_FALSE) {
                                #ifdef DB_collect_variation_in
                                    debug_print("charclass assigned : %d%d", AVSME_GET(new_char_class, MAINCLASS), AVSME_GET(new_char_class, SUBCLASS));
                                #endif
                                j++;
                                char_class = new_char_class;
                            }

                            goto VLUT_NEXT_CHAR;
                        }; // else, continue.
                    }
                    
                    else if (j == table[i*_stride + 1] + 1) VLUT_NO_VALUE_HANDLE; // goto VLUT_ERROR_CHECK for safety

                    #ifdef DB_collect_variation_in
                        printf(" " debug_bar);
                    #endif

                }
            }

            else if (i == n_keys - 1) VLUT_NO_KEY_HANDLE; // goto VLUT_ERROR_CHECK for safety

            #ifdef DB_collect_variation_in
                printf(" " debug_bar);
            #endif

        VLUT_NEXT_ENTRY:
        }
        
    VLUT_NEXT_CHAR:
    count++;
    }

    /* Default behaviour of collect_variation */

    VLUT_ERROR_CHECK:
    if (countptr) (*countptr) = count + 1;

    if (!*_str) {
        struct collect_out ret = {char_class, current_token, _str};
        return ret; // return.
    }

    VLUT_ERROR_HANDLE;
    return VLUT_ERROR_RETURN_HANDLE;
}

struct collect_out collect_variation_in   (
                                                char * _str,
                                                size_t n_keys,
                                                avsme * table,
                                                size_t _stride
                                            ) /* Actual Function */
{
    static char * current_string = NULL;
    static size_t count = 0;
    /* To be able to reset when the given string changes. */

    static struct collect_out out;
    /* To store the collect’s return value. */

    if (_str != current_string) {
        /* If the given string doesn’t match the string that we’ve been keeping track of, all the tracking resets. Thus, new tracking for new strings. */
        current_string = _str;
        out.new_token = _str;
    }
    
    if (!*(out.new_token)) {
        // If the out terminates, then reset the static string.
        current_string = NULL;
        return COLLECT_OUT_NULL;
    }
    
    reset_fnv;
    out = collect_variation(out.new_token, n_keys, table, _stride, &count);
    if (out.old_token == out.new_token) out.new_token += 1;
    // Collection of token and pointers to tokens

    return out; // Returning collect_out
}

size_t print_str_collective_variation (
                                            char * 	_str,
                                            size_t 	n_keys,
                                            avsme * table,
                                            size_t 	_stride
                                        ) /* Actual Function */
{
    struct collect_out out; /* To store the return value of collect. */
    size_t count; /* To store the number of char(s). */
    size_t ret; /* To store the number of printed char(s). */

    while (( out collecting(_str, n_keys, table, _stride) )) {
        // number of char(s)
        ret += (count = (size_t)(out.new_token - out.old_token));

        // printing of charclass
        for (int i = 0; i < count; i++) printf("%d",  AVSME_GET(out.char_class, MAINCLASS));
    }

    collect_variation_in(NULL, 0, NULL, 0); // resetting collect_in
    return ret; // returning the length of printed string
}

#endif

#if defined(_INC_V_SCRIPT) && defined(_INIT_V_SCRIPT) && !defined(_IMPL_V_SCRIPT)
#define _IMPL_V_SCRIPT

char * __recent_charclass_extend_esc_str = NULL;
char __charclass_extend_esc_n[4] = "\\n";
char __charclass_extend_esc_t[4] = "\\t";
char __charclass_extend_esc_r[4] = "\\r";
char __charclass_extend_esc_a[4] = "\\a";

size_t VS_active_vlut_n_keys;
avsme * VS_active_vlut_table;
size_t VS_active_vlut_stride;

struct VS_RECENT_VLUT VS_RECENT_VLUT;

void VS__free_recent_vlut(void) {
    for (int i = 0; i < VS_RECENT_VLUT.n_keys; i++) 
    if (VS_RECENT_VLUT.table[i]) free(VS_RECENT_VLUT.table[i]), VS_RECENT_VLUT.table[i] = NULL;
    VS_RECENT_VLUT.table = NULL;
}

_Noreturn void VERLET_VLUT_exit(void) {
    printf("\033[31m<! VLUT case is not registered. !>\033[0m");
    exit(-1);
}

avsme VERLET_charclass(char c) {
    avsme RET_SUBCLASS;
    avsme RET_MAINCLASS;

    if (__char_idvalid(c)) {
        if (__char_lower_alpha(c)) RET_SUBCLASS = 2;
        else if (__char_upper_alpha(c)) RET_SUBCLASS = 3;
        else RET_SUBCLASS = 1;
        RET_MAINCLASS = 1;
    }
    else if (__char_digit(c)) {
        RET_SUBCLASS = 0;
        RET_MAINCLASS = 2;
    }
    else switch (c)
    {
    case '='    :
    RET_SUBCLASS = 0;
    RET_MAINCLASS = 9; goto VERLET_CHARCLASS_RETURN;

    case '\\'   :
    RET_SUBCLASS = 0;
    RET_MAINCLASS = 8; goto VERLET_CHARCLASS_RETURN;

    case '`'    :
    case '"'    :
    case '\''   :
    RET_SUBCLASS = 0;
    RET_MAINCLASS = 5; goto VERLET_CHARCLASS_RETURN;

    case '#'    :
    case '$'    :
    case '@'    :
    case '?'    :
    RET_SUBCLASS = 0;
    RET_MAINCLASS = 7; goto VERLET_CHARCLASS_RETURN;

    case ','    :
    case ';'    :
    RET_SUBCLASS = 1; goto MAINCLASS_6;
    case '.'    :
    case ':'    :
    RET_SUBCLASS = 2; goto MAINCLASS_6;
    case ' '    :
    RET_SUBCLASS = 3;
    MAINCLASS_6: RET_MAINCLASS = 6; goto VERLET_CHARCLASS_RETURN;

    case '<'    :
    case '('    :
    case '['    :
    case '{'    :
    RET_SUBCLASS = 1; goto MAINCLASS_4;
    case '>'    :
    case ')'    :
    case ']'    :
    case '}'    :
    RET_SUBCLASS = 2;
    MAINCLASS_4: RET_MAINCLASS = 4; goto VERLET_CHARCLASS_RETURN;

    case '~'    :
    case '!'    :
    RET_SUBCLASS = 1; goto MAINCLASS_3;
    case '+'    :
    case '-'    :
    case '*'    :
    case '/'    :
    case '%'    :
    case '^'    :
    case '&'    :
    case '|'    :
    RET_SUBCLASS = 2; 
    MAINCLASS_3: RET_MAINCLASS = 3; goto VERLET_CHARCLASS_RETURN;
    
    default:
        VSCRIPT_INVALID_CHAR_CASE;
    }

    VERLET_CHARCLASS_RETURN:

    avsme RET = AVSME_SET(0, ASCII, c);
    RET = AVSME_SET(RET, MAINCLASS, RET_MAINCLASS);
    RET = AVSME_SET(RET, SUBCLASS, RET_SUBCLASS);
    RET = AVSME_SET(RET, EXISTS, 1);

    return RET;
}

_Noreturn void VERLET_charclass_invalid_char_error(char c) {
    printf("\033[31m<! Verlet Char Class of \033[33m'%c'\033[31m does not exist !>\033[0m", c);
    exit(-1);
}

avsme VERLET_charclass_extend(char c) {
    avsme RET_SUBCLASS;
    avsme RET_MAINCLASS = 8;
    
    switch (c) {
        case '\n': __recent_charclass_extend_esc_str = __charclass_extend_esc_n; 
        RET_SUBCLASS = 1; break;
        case '\t': __recent_charclass_extend_esc_str = __charclass_extend_esc_t; 
        RET_SUBCLASS = 2; break;
        case '\r': __recent_charclass_extend_esc_str = __charclass_extend_esc_r; 
        RET_SUBCLASS = 3; break;
        case '\a': __recent_charclass_extend_esc_str = __charclass_extend_esc_a; 
        RET_SUBCLASS = 4; break;
        default :
        VERLET_charclass_invalid_char_error(c);    
    }
    
    avsme RET = AVSME_SET(0, ASCII, c);
    RET = AVSME_SET(RET, MAINCLASS, RET_MAINCLASS);
    RET = AVSME_SET(RET, SUBCLASS, RET_SUBCLASS);
    RET = AVSME_SET(RET, EXISTS, 1);
    
    return RET;
}

void VERLET_charclass_print(void) {

    avsme to_print;

    for (int i = 1; ;) {

        style_bold;
        color_back_grey(150 + (i%2)*20);
        color_fore_grey(60);
        printf(" %d ", i);
        color_fore_reset;
        color_back_reset;
        style_bold_reset;

        int count = 0;
        int prev_count = -1;
        int s_printed = 0;

        for (int s = 0; s < 4; s++) {

            prev_count = count;

            for (int j = 32; j <= 126; j++) {

                to_print = VERLET_charclass(j);
                
                if ((AVSME_GET(to_print, MAINCLASS) == i) && (AVSME_GET(to_print, SUBCLASS) == s)) {
                    count++;
                }
            }
            
            if ( s && (prev_count != count) ) {
                color_back_reset;
                color_fore_grey(200);
                printf("%s%d ", (s != 1) ? "\n    " : " ", s);
            }

            color_fore_grey(250);
            for (int j = 32; j <= 126; j++) {

                to_print = VERLET_charclass(j);
                
                if ((AVSME_GET(to_print, MAINCLASS) == i) && (AVSME_GET(to_print, SUBCLASS) == s)) {
                    
                    color_back_grey(30 + (i%2)*20 + ((prev_count)%2)*10);
                    printf(" %c ", j);
                    
                    prev_count++;
                }
            }
            
        }

        color_back_reset;
        i++;
        if (i <= 9) printf("\n");
        else break;
    }
}

size_t VERLET_avsme_print(avsme _flag) {
    size_t ret = 0;
    if (_flag & AVSME_ASCII) putchar(' '), ret += 1 + __print_arbitr_char(AVSME_GET(_flag, ASCII));
    if (_flag & AVSME_MAINCLASS) putchar(' '), ret += 1 + printf(VERLET_fullclass_str(VERLET_fullclass(_flag)));
    putchar(' ');
    return ret + 1;
}

void VS_RECENT_VLUT_print(void) {
    
    for (int k = 0; ; ) {
        

        if (AVSME_BOOLEAN(VS_RECENT_VLUT.table[k][0]) == AVSME_TRUE) {
                style_bold;
                color_fore(200, 200, 0);
                printf("all");
                color_fore_reset;
                style_bold_reset;
        }
        else {
            color_back_grey(200 + (k%2)*40);
            VERLET_avsme_print(VS_RECENT_VLUT.table[k][0]);
            color_back_reset;
        }
        
        printf(" :");
        
        avsme current;
        
        for (int v = 2; v < VS_RECENT_VLUT.table[k][1] + 2; v++) {
            current = VS_RECENT_VLUT.table[k][v];
            
            style_bold;
            if (current & AVSME_VARIANCE) printf(" vary ");
            else printf(" join ");
            style_bold_reset;
            
            if (AVSME_BOOLEAN(current) == AVSME_TRUE) {
                style_bold;
                color_fore(200, 200, 0);
                printf("all");
                color_fore_reset;
                style_bold_reset;
            }
            else color_back_grey(30 + (k%2)*30), VERLET_avsme_print(current), color_back_reset;
        }
        
        k++;
        if (k == VS_RECENT_VLUT.n_keys) break;
        putchar('\n');
        putchar('\n');
        
    }
}

void __VS_VLUT_print   (
                            size_t n_keys,
                            avsme * table,
                            size_t _stride
                        )
{

    for (int k = 0; ; ) {
        
        if (AVSME_BOOLEAN(VS_RECENT_VLUT.table[k][0]) == AVSME_TRUE) {
            style_bold;
            color_fore(200, 200, 0);
            printf("all");
            color_fore_reset;
            style_bold_reset;
        }
        else {
            color_back_grey(200 + (k%2)*40);
            VERLET_avsme_print(table[ k * _stride ]);
            color_back_reset;
        }
        
        printf(" :");
        
        avsme current;
        
        for (int v = 2; v < table[ k * _stride + 1] + 2; v++) {
            
            current = table[ k * _stride + v ];
            
            style_bold;
            if (current & AVSME_VARIANCE) printf(" vary ");
            else printf(" join ");
            style_bold_reset;
            
            if (AVSME_BOOLEAN(current) == AVSME_TRUE) {
                style_bold;
                color_fore(200, 200, 0);
                printf("all");
                color_fore_reset;
                style_bold_reset;
            }
            else color_back_grey(30 + (k%2)*30), VERLET_avsme_print(current), color_back_reset;
        }
        
        k++;
        if (k == n_keys) break;
        putchar('\n');
        putchar('\n');
        
    }
}

#endif


#if defined(_INC_V_READWRITE) && !defined(_IMPL_V_READWRITE)
#define _IMPL_V_READWRITE

FILE * RW_recent_open = NULL;

RW_init_Reader

RW_init_Writer

#endif

#if defined(_INC_V_TOKENLIST) && !defined(_IMPL_V_TOKENLIST)
#define _IMPL_V_TOKENLIST

size_t VCo_enable_i = 0;
hash * VCo_recent_set = NULL;

int VCo_search_set(hash * _set, size_t _size) {
    for (int i = 0; i < _size; i++) if token_is(_set[i]) return 1;
    return 0;
}

struct VCo_hash_link * VCo_new_hash_link(hash _token) {
    struct VCo_hash_link * ret = malloc(sizeof(struct VCo_hash_link));
    ret->value = _token;
    ret->next = NULL;
    return ret;
}

void VCo_free_hash_links(struct VCo_hash_link * _hash_link) {
    if (!_hash_link) return;
    struct VCo_hash_link * current = _hash_link;
    struct VCo_hash_link * to_free;

    while (current->next) {
        to_free = current;
        current = current->next;
        free(to_free);
    }
    free(current);
}

size_t VCo_search_hash_list(struct VCo_hash_link * _links, hash _token, ...) {
    if (!_links) return 0;
    size_t ret = 1;

    struct VCo_hash_link * curr = _links;

    while (curr->value != _token) {
        if (!curr->next) return 0;
        curr = curr->next;
        ret++;
    }

    return ret;
}

#endif