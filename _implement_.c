
#if defined(_INC_V_NON_STATAL) && !defined(_IMPL_V_NON_STATAL)
#define _IMPL_V_NON_STATAL

avsme AVSME_NONE = AVSME_SET(AVSME_EXISTS, ASCII, 0);
avsme AVSME_TRUE = AVSME_SET(AVSME_EXISTS, ASCII, 1);
avsme AVSME_FALSE = AVSME_SET(AVSME_EXISTS, ASCII, 2);

hash __token_meta = 14695981039346656037ULL;

hash hash_function_fnv(char * str) {
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

hash * TL_enable_buffer;
size_t TL_enable_i;
size_t TL_enable_max;

char TL_token_list_static_search(hash * _buffer, size_t _size, hash _search) {
    for (int i = 0; i < _size; i++) if (_buffer[i] == _search) return 1;
    return 0;
}

char TL_token_list_dynamic_search(struct TL_hash_link _buffer, size_t _size, hash _search) {
    struct TL_hash_link * _first = _buffer.next;
    for (int i = 0; (i < _size) &&  _first; i++) {
        if (_first->value == _search) return 1;
        _first = _first->next;
    }
    return 0;
}

void TL_free_token_list(struct TL_hash_link * _first) {
    for (struct TL_hash_link * to_free; _first;) {
        _first = _first->next;
        to_free = _first;
        free(to_free);
    }
}

void CC_ccast_token_list_static(hash * _buffer, size_t _size, char * _iden) {
    printf("\nchar TL_%s_type = 0;\n", _iden);
    printf("char * TL_%s_iden = \"%s\";\n", _iden, _iden);
    printf("size_t TL_%s_size = %zu;\n", _iden, _size);
    printf("hash TL_%s_buffer[%zu] = {\n\t", _iden, _size);
    for (int i = 0; i < _size; i++) {
        printf("%zuULL", _buffer[i]);
        if ((i + 1) != _size) printf(", ");
        else printf("\n");
        if ((i + 1) % 4 == 0) printf("\n\t");
    }
    printf("};");
}

void CC_ccast_token_list_dynamic(struct TL_hash_link _buffer, size_t _size, char * _iden) {
    printf("\nchar TL_%s_type = 0;\n", _iden);
    printf("char * TL_%s_iden = \"%s\";\n", _iden, _iden);
    printf("size_t TL_%s_size = %zu;\n", _iden, _size);
    printf("hash TL_%s_buffer[%zu] = {\n\t", _iden, _size);
    struct TL_hash_link * _first = _buffer.next;
    for (int i = 0; i < _size; i++) {
        if (!_first) return;
        printf("%zuULL", _first->value);
        if ((i + 1) != _size) printf(", ");
        else printf("\n");
        if ((i + 1) % 4 == 0) printf("\n\t");
    }
    printf("};");
}


#endif


#if defined(_INC_V_STATAL) && !defined(_IMPL_V_STATAL)
#define _IMPL_V_STATAL

#ifdef DYNAMIC_CHARCLASS

avsme CHARCLASS_none_active(char _char) {
    printf("\033[31m<! No active Char Class. !>\033[0m");
    exit(-1);
}

avsme (* CHARCLASS_active)(char _char) = CHARCLASS_none_active;

#endif

#endif


#if defined(_INC_V_COLLECT)  && defined(_INIT_V_COLLECT) && !defined(_IMPL_V_COLLECT)
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

// Variation Collection

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
    hash_push *(_str);
    avsme new_char_class; 				    // Char class of the new char.
    char * current_token = _str; 			// Pointer to the current token (updates _str)
    avsme check_value;
    avsme check_key;
    int count = (countptr) ? * countptr + 1 : 0;

    int check_key_ascii;
    int check_value_ascii;
    int check_key_bool;
    int check_value_bool;
    int check_value_exists;

    int value_set_jump = 1;

    uintptr_t mode_intptr = 0;
    void (* mode_ptr) (void);
    mode_ptr = NULL;
    
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
                    check_value_bool = (AVSME_BOOLEAN(check_value) == AVSME_TRUE) && AVSME_GET(check_value, EXISTS);
                    check_value_ascii = AVSME_GET(check_value, ASCII);
                    check_value_exists = AVSME_EXISTENT(check_value);

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
                    
                    if (!check_value_exists); 
                    else if ( check_value_bool || AVSME_OVERLAP(check_value, new_char_class) )  {
                        /* If the entry contains the new char class. */

                        if (j != table[i*_stride + 1] + 1) {
                            if (!AVSME_EXISTENT(table[i*_stride + j + 1])) {
                                mode_ptr = NULL;
                                VS_buffer_to_mode(mode_ptr, table + i*_stride + j + 1);
                                mode_ptr();
                            }
                            else if ( !AVSME_EXISTENT(table[i*_stride + j + 2]) && (AVSME_BOOLEAN(table[i*_stride + j + 1]) == AVSME_FALSE) ) {
                                mode_ptr = NULL;
                                VS_buffer_to_mode(mode_ptr, table + i*_stride + j + 2);
                                mode_ptr();
                            }
                        }
                        
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

                            hash_push *(_str);

                            if (
                                ( AVSME_BOOLEAN(table[i*_stride + j + 1]) == AVSME_FALSE )
                                ||
                                (
                                    !AVSME_EXISTENT(table[i*_stride + j + 1])
                                    &&
                                    AVSME_BOOLEAN(table[i*_stride + j + 1 + VS_mode_span]) == AVSME_FALSE
                                )
                            )
                            {
                                #ifdef DB_collect_variation_in
                                    debug_print("charclass assigned : %d%d", AVSME_GET(new_char_class, MAINCLASS), AVSME_GET(new_char_class, SUBCLASS));
                                #endif
                                char_class = new_char_class;
                            }

                            goto VLU_NEXT_CHAR;
                        }; // else, continue.
                    }
                    
                    else if (j == table[i*_stride + 1] + 1) VLU_NO_VALUE_HANDLE; // goto VLU_ERROR_CHECK for safety

                    #ifdef DB_collect_variation_in
                        printf(" " debug_bar);
                    #endif

                }
            }

            else if (i == n_keys - 1) VLU_NO_KEY_HANDLE; // goto VLU_ERROR_CHECK for safety

            #ifdef DB_collect_variation_in
                printf(" " debug_bar);
            #endif

        VLU_NEXT_ENTRY:
        }
        
    VLU_NEXT_CHAR:
    count++;
    }

    /* Default behaviour of collect_variation */

    VLU_ERROR_CHECK:
    if (countptr) (*countptr) = count + 1;

    if (!*_str) {
        struct collect_out ret = {char_class, current_token, _str};
        return ret; // return.
    }

    VLU_ERROR_HANDLE;
    return VLU_ERROR_RETURN_HANDLE;
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
    
    hash_reset;
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


#if defined(_INC_V_DSL) && defined(_INIT_V_DSL) && !defined(_IMPL_V_DSL)
#define _IMPL_V_DSL

char * __recent_charclass_extend_esc_str = NULL;
char __charclass_extend_esc_n[4] = "\\n";
char __charclass_extend_esc_t[4] = "\\t";
char __charclass_extend_esc_r[4] = "\\r";
char __charclass_extend_esc_a[4] = "\\a";

size_t VS_active_VLU_n_keys;
avsme * VS_active_VLU_table;
size_t VS_active_VLU_stride;

struct VS_RECENT_VLU VS_RECENT_VLU;

void VS__free_recent_VLU(void) {
    for (int i = 0; i < VS_RECENT_VLU.n_keys; i++) 
    if (VS_RECENT_VLU.table[i]) free(VS_RECENT_VLU.table[i]), VS_RECENT_VLU.table[i] = NULL;
    VS_RECENT_VLU.table = NULL;
}

_Noreturn void VERLET_VLU_exit(void) {
    printf("\033[31m<! VLU case is not registered. !>\033[0m");
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

char VS_OFF_CHARCLASS_ERR = 0;

avsme VERLET_charclass_invalid_char_error(char c) {
    if (VS_OFF_CHARCLASS_ERR) return 0;
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
        return VERLET_charclass_invalid_char_error(c);    
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

void VS_RECENT_VLU_print(void) {
    
    for (int k = 0; ; ) {
        
        if (!AVSME_GET(VS_RECENT_VLU.table[k][0], EXISTS)) continue;

        if (AVSME_BOOLEAN(VS_RECENT_VLU.table[k][0]) == AVSME_TRUE) {
                style_bold;
                color_fore(200, 200, 0);
                printf("all");
                color_fore_reset;
                style_bold_reset;
        }
        else {
            color_back_grey(200 + (k%2)*40);
            VERLET_avsme_print(VS_RECENT_VLU.table[k][0]);
            color_back_reset;
        }
        
        printf(" :");
        
        avsme current;
        
        for (int v = 2; v < VS_RECENT_VLU.table[k][1] + 2; v++) {
            current = VS_RECENT_VLU.table[k][v];
            
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
        if (k == VS_RECENT_VLU.n_keys) break;
        putchar('\n');
        putchar('\n');
        
    }
}

void __VS_VLU_print   (
                            size_t n_keys,
                            avsme * table,
                            size_t _stride
                        )
{

    for (int k = 0; ; ) {
        
        if (AVSME_BOOLEAN(table[k * _stride]) == AVSME_TRUE) {
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


#if defined(_INC_V_FILEIO) && !defined(_IMPL_V_FILEIO)
#define _IMPL_V_FILEIO

FILE * RW_recent_open = NULL;

RW_init_Reader

RW_init_Writer

#endif


#if defined(_INC_V_CIMPL) && !defined(_IMPL_V_CIMPL)
#define _IMPL_V_CIMPL

uint16_t CIMPL_enclose_rgb[3] = {240, 100, 200};
int64_t CIMPL_scope_count = 0;

hash CIMPL_in_quotes = 0;
hash CIMPL_this_quote = 0;

int pre_processor_state = 1;

hash CIMPL_tkn_newline;
hash CIMPL_tkn_pre_processor_diretive;

hash CIMPL_tkn_include;
hash CIMPL_tkn_define;
hash CIMPL_tkn_defined;
hash CIMPL_tkn_undef;
hash CIMPL_tkn_ifdef;
hash CIMPL_tkn_ifndef;
hash CIMPL_tkn_elifdef;
hash CIMPL_tkn_elifndef;
hash CIMPL_tkn_endif;
hash CIMPL_tkn_error;
hash CIMPL_tkn_warning;
hash CIMPL_tkn_line;
hash CIMPL_tkn_pragma;
hash CIMPL_tkn_if;
hash CIMPL_tkn_elif;
hash CIMPL_tkn_else;
hash pre_processor_command;

token_list(DataTypes, 9) {
    keyword(void);
    keyword(char);
    keyword(short);
    keyword(int);
    keyword(long);
    keyword(float);
    keyword(double);
    keyword(signed);
    keyword(unsigned);
}

token_list(StorageClasses, 6) {
    keyword(auto);
    keyword(register);
    keyword(static);
    keyword(extern);
    keyword(thread_local);
    keyword(_Thread_local);
}

token_list(TypeQualifiers, 4) {
    keyword(const);
    keyword(volatile);
    keyword(restrict);
    keyword(_Atomic);
}

token_list(Controls, 13) {
    keyword(if);
    keyword(else);
    keyword(switch);
    keyword(case);
    keyword(default);
    keyword(for);
    keyword(while);
    keyword(do);
    keyword(break);
    keyword(continue);
    keyword(return);
    keyword(goto);
    keyword(typedef);
}

token_list(Structures, 4) {
    keyword(struct);
    keyword(union);
    keyword(enum);
}

token_list(Specifiers, 1) {
    keyword(inline);
}

token_list(CompileTimeOperators, 6) {
    keyword(sizeof);
    keyword(alignof);
    keyword(_Generic);
    keyword(alignas);
    keyword(_Alignas);
    keyword(_Alignof);
}

token_list(Assertion, 2) {
    keyword(static_assert);
    keyword(_Static_assert);
}

token_list(Functions);

token_list(Macros);

token_list(Pre_Processor_Commands, 15) {
    keyword_hash(CIMPL_tkn_include);
    keyword_hash(CIMPL_tkn_define);
    keyword_hash(CIMPL_tkn_undef);
    keyword_hash(CIMPL_tkn_ifdef);
    keyword_hash(CIMPL_tkn_ifndef);
    keyword_hash(CIMPL_tkn_elifdef);
    keyword_hash(CIMPL_tkn_elifndef);
    keyword_hash(CIMPL_tkn_endif);
    keyword_hash(CIMPL_tkn_error);
    keyword_hash(CIMPL_tkn_warning);
    keyword_hash(CIMPL_tkn_line);
    keyword_hash(CIMPL_tkn_pragma);
    keyword_hash(CIMPL_tkn_if);
    keyword_hash(CIMPL_tkn_elif);
    keyword_hash(CIMPL_tkn_else);
}

token_list(Pre_Processor_Operators, 4) {
    keyword_hash(CIMPL_tkn_defined);
    keyword(&&);
    keyword(||);
    keyword(!);
}

Handle_Pre_Processing {
    
    switch (pre_processor_state)
    {
        case 2:

            if token_is(idvalid) {
                CIMPL_DeColorise;
                    if (token_in(Pre_Processor_Commands)) {
                        pre_processor_command = hash_value;
                        pre_processor_state = 3;
                        CIMPL_Colorise_Red;
                    }
                    else {
                        pre_processor_state = 0;
                        CIMPL_Colorise_C_Tokens;
                    }
            }
            break;
            
        default: if (pre_processor_command)
        {

            if (pre_processor_command == CIMPL_tkn_include) {
                CIMPL_DeColorise;
                CIMPL_Colorise_Yellow;
            }

            else if (pre_processor_command == CIMPL_tkn_define)
            {
                CIMPL_DeColorise;
                switch (pre_processor_state)
                {
                    case 3:
                        if token_is(idvalid) {
                            pre_processor_state = 4;
                            append_token_list(Macros, hash_value);
                            CIMPL_Colorise_Green;
                        }
                        else if token_is_not(disjoin, space) {
                            pre_processor_state = 0;
                            CIMPL_Colorise_C_Tokens;
                        }
                        break;
                    case 4:
                        if token_is(enclose, open) {
                            pre_processor_state = 5;
                            CIMPL_Colorise_Green;
                        }
                        else {
                            pre_processor_state = 6;
                            CIMPL_Colorise_C_Tokens;
                        }
                        break;
                    case 5:
                        if token_is(enclose, close) {
                            pre_processor_state = 6;
                            CIMPL_Colorise_Green;
                        }
                        else {
                            CIMPL_Colorise_C_Tokens;
                        }
                        break;
                    case 6:
                        pre_processor_state = 0;
                        CIMPL_Colorise_C_Tokens;
                }
            }

            else if
            (
                (pre_processor_command == CIMPL_tkn_undef)
                ||
                (pre_processor_command == CIMPL_tkn_ifdef)
                ||
                (pre_processor_command == CIMPL_tkn_ifndef)
                ||
                (pre_processor_command == CIMPL_tkn_elifdef)
                ||
                (pre_processor_command == CIMPL_tkn_elifndef)
            )
            {
                CIMPL_DeColorise;
                if token_in(Macros) CIMPL_Colorise_Green;
                else CIMPL_Colorise_C_Tokens;
            }

            else if
            (
                (pre_processor_command == CIMPL_tkn_endif)
                ||
                (pre_processor_command == CIMPL_tkn_error)
                ||
                (pre_processor_command == CIMPL_tkn_warning)
                ||
                (pre_processor_command == CIMPL_tkn_line)
                ||
                (pre_processor_command == CIMPL_tkn_pragma)
            )
            {
                CIMPL_DeColorise;
                CIMPL_Colorise_C_Tokens;
            }

            else if
            (
                (pre_processor_command == CIMPL_tkn_if)
                ||
                (pre_processor_command == CIMPL_tkn_elif)
                ||
                (pre_processor_command == CIMPL_tkn_else)
            )
            {
                CIMPL_DeColorise;
                if token_in(Pre_Processor_Operators) CIMPL_Colorise_Red;
                else CIMPL_Colorise_C_Tokens;
            }

        }

    }

}

#endif

#if defined(_INC_V_CCAST) && !defined(_IMPL_V_CCAST)
#define _IMPL_V_CCAST


void CC_ccast_charclass_ASCII(void) {

    VS_OFF_CHARCLASS_ERR = 1;

    avsme _char_class;


    printf("\n#define charclass(c) STATE_charclass_table[c]\n");
    printf("avsme STATE_charclass_table[128] = {\n\t");

    for (int i = 0; i < 128; i++) {

        _char_class = charclass(i);
        printf("0X%.6x%c ", (AVSME_EXISTENT(_char_class)) ? _char_class : 0, ','*(i!=127));

        if (!((i + 1)%10)) printf("\n\t");
    }

    printf("\n};");

    VS_OFF_CHARCLASS_ERR = 0;

}

#ifdef DYNAMIC_CHARCLASS

void CC_ccast_charclass_ASCII_function(struct CHARCLASS_charclass_view _charclass) {

    VS_OFF_CHARCLASS_ERR = 1;

    avsme _char_class;

    printf("avsme STATE_charclass_%s_table[128] = {\n\t", _charclass.iden);

    for (int i = 0; i < 128; i++) {

        _char_class = _charclass.function(i);
        printf("0X%.6x%c ", (AVSME_EXISTENT(_char_class)) ? _char_class : 0, ','*(i!=127));

        if (!((i + 1)%10)) printf("\n\t");
    }

    printf("\n};");

    VS_OFF_CHARCLASS_ERR = 0;

}

#endif

void CC_ccast_VLU(void) {
    int x = 0;

    printf("avsme STATE_VLU_table[%zu] = {\n", VS_active_VLU_n_keys*VS_active_VLU_stride);

    for (int y = 0; y < VS_active_VLU_n_keys; y++) {
        
        printf("    %zu, %zu",
            VS_active_VLU_table[y *VS_active_VLU_stride],
            VS_active_VLU_table[y *VS_active_VLU_stride + 1]
        );

        for (x = 0; x < VS_active_VLU_table[y *VS_active_VLU_stride + 1]; x++)
        printf(", %zu", VS_active_VLU_table[y*VS_active_VLU_stride + 2 + x]);

        for (; x < VS_active_VLU_stride; x++)
        printf(", 0");

        if (y + 1 != VS_active_VLU_n_keys) putchar(',');
        putchar('\n');
    }

    printf("}; ");

    printf("struct VLU_VLU_view STATE_VLU = {");
    printf(" STATE_VLU_table");
    printf(" STATE_VLU,");
    printf(" %d,", VS_active_VLU_n_keys);
    printf(" %d,", VS_active_VLU_stride);
    printf(" %d,", VS_active_VLU_n_keys);
    printf(" %d };", VS_active_VLU_stride);

}

void CC_ccast_VLU_function(struct VLU_VLU_view _VLU) {

    int x = 0;

    char * _name = _VLU.iden;

    printf("char VLU_%s_iden[] = \"%s\";\n", _name, _name);
    printf("avsme VLU_%s_table[%zu] = {\n", _name, _VLU.max_keys*(2 + _VLU.max_values));

    for (int y = 0; y < _VLU.n_keys; y++) {

        printf("    %zu, %zu",
            _VLU.table[y *(2 + _VLU.max_values)],
            _VLU.table[y *(2 + _VLU.max_values) + 1]
        );

        for (x = 0; x < _VLU.table[y *(2 + _VLU.max_values) + 1]; x++)
        printf(", %zu", _VLU.table[y*(2 + _VLU.max_values) + 2 + x]);

        for (; x < _VLU.max_values; x++)
        printf(", 0");

        if (y + 1 != _VLU.n_keys) putchar(',');
        putchar('\n');
    }

    printf("}; ");

    printf("struct VLU_VLU_view %s = {", _name);
    printf(" VLU_%s_table,", _name);
    printf(" VLU_%s_iden,", _name);
    printf(" %d,", _VLU.n_keys);
    printf(" %d,", _VLU.n_values);
    printf(" %d,", _VLU.max_keys);
    printf(" %d };", _VLU.max_values);

}

// void CC_ccast_tokenlist_static(struct DSL_tokenlist_view _list) {

//     size_t len = _list.list.static_list.size;
//     printf("size_t VCo_enable_%s_size = %d;\n", _list.iden, len);
//     printf("hash %s[%d] =  {\n\t", _list.iden, len);

//     hash curr = 0;

//     for (int i = 0; (i < len) && printf("%c%c", i ? ',' : '\0', i ? ' ' : '\0'); i++) {
//         if ((i + 1)%5 == 0) printf("\n\t");
        
//         printf("%zuULL", hash_of(_list.list.static_list.buffer[i]));
//     }

//     putchar('\n');
//     putchar('}');
//     putchar(';');
// }

// void CC_ccast_tokenlist_dynamic(struct DSL_tokenlist_view _list) {

//     size_t len = _list.list.dynamic_list.size;
//     printf("size_t VCo_enable_%s_size = %d;\n", _list.iden, len);
//     printf("hash %s[%d] =  {\n\t", _list.iden, len);

//     struct VCo_hash_link * curr = _list.list.dynamic_list.buffer;

//     for (int i = 0; (i < len) && printf("%c%c", i ? ',' : '\0', i ? ' ' : '\0'); i++) {
//         if ((i + 1)%9 == 0) printf("\n\t");
        
//         printf("%zuULL", hash_of(curr->value));
//         curr = curr->next;
//     }

//     putchar('\n');
//     putchar('}');
//     putchar(';');
// }

void CC_ccast(void) {

    CC_Link Curr = CC_List;

    #ifdef DYNAMIC_CHARCLASS
    printf("\n#define charclass(c) (CHARCLASS_active[c])");
    printf("\navsme * CHARCLASS_charclass_active;");
    printf("\n#undef dynamic_charclass ");
    printf("\n#define dynamic_charclass(_charclass) CHARCLASS_charclass_active = mergetokens(STATE_charclass_, _charclass, _table)");
    #else
    CC_ccast_charclass_ASCII();
    #endif

    putchar('\n');

    while (Curr = Curr->next) {

        switch (Curr->type)
        {
        
        #ifdef DYNAMIC_CHARCLASS
        
        case 1:
            putchar('\n');
            struct CHARCLASS_charclass_view _charclass = *((struct CHARCLASS_charclass_view *)(Curr->object));
            CC_ccast_charclass_ASCII_function(_charclass);
            putchar('\n');
            break;

        #endif

        case 2:
            putchar('\n');
            struct VLU_VLU_view _VLU = *((struct VLU_VLU_view *)(Curr->object));
            CC_ccast_VLU_function(_VLU);
            putchar('\n');
            break;
        
        // case 3:
        //     putchar('\n');
        //     struct DSL_tokenlist_view static_list = *((struct DSL_tokenlist_view *)(Curr->object));
        //     CC_ccast_tokenlist_static(static_list);
        //     putchar('\n');
        //     break;

        // case 4:
        //     putchar('\n');
        //     struct DSL_tokenlist_view dynamic_list = *((struct DSL_tokenlist_view *)(Curr->object));
        //     CC_ccast_tokenlist_dynamic(dynamic_list);
        //     putchar('\n');
        //     break;
        
        default:
            printf("\033[31m<! Not a valid CC_Link type. !>\033[0m");
        }

    }

}

#endif
