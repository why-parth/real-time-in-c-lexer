//
// Standard Verlet Header Implemetation
//

#if defined(VERLET_COLLECT) && !defined(VERLET_COLLECT_INIT)
#define VERLET_COLLECT_INIT

struct collect_out COLLECT_OUT_NULL = {AVSME_NULL, NULL, NULL};
/* Represents nothing in terms of struct collect_out. */

avsme AVSME_NONE = AVSME_SET(0, ASCII, 0);
avsme AVSME_TRUE = AVSME_SET(0, ASCII, 1);
avsme AVSME_FALSE = AVSME_SET(0, ASCII, 2);
/* Boolean AVSMEs. */

hash __token_meta = 14695981039346656037ULL;

struct collect_out collect_immediate(char * _str) {
    if(!_str) return COLLECT_OUT_NULL;
    /* If the given string is NULL, char class is -1 and the pointers to tokens are NULL. */
    else if(!*_str) return COLLECT_OUT_NULL;
    /* If the given string is empty, char class is 0 and the pointers to tokens are NULL. */

    uint_least16_t char_class = charclass(_str[0]); // Char class of the first char.
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

struct collect_out __collect_variation  (
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

struct collect_out __collect_variation_in   (
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
    out = __collect_variation(out.new_token, n_keys, table, _stride, &count);
    if (out.old_token == out.new_token) out.new_token += 1;
    // Collection of token and pointers to tokens

    return out; // Returning collect_out
}

size_t __print_str_collective_variation (
                                            char * 	_str,
                                            size_t 	n_keys,
                                            avsme * table,
                                            size_t 	_stride
                                        ) /* Actual Function */
{
    struct collect_out out; /* To store the return value of collect. */
    size_t count; /* To store the number of char(s). */
    size_t ret; /* To store the number of printed char(s). */

    while (( out __collecting(_str, n_keys, table, _stride) )) {
        // number of char(s)
        ret += (count = (size_t)(out.new_token - out.old_token));

        // printing of charclass
        for (int i = 0; i < count; i++) printf("%d",  AVSME_GET(out.char_class, MAINCLASS));
    }

    __collect_variation_in(NULL, 0, NULL, 0); // resetting collect_in
    return ret; // returning the length of printed string
}

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