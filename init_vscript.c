#if defined(VERLET_SCRIPT) && !defined(VERLET_SCRIPT_INIT)
#define VERLET_SCRIPT_INIT

char * __recent_charclass_extend_esc_str = NULL;
char __charclass_extend_esc_n[4] = "\\n";
char __charclass_extend_esc_t[4] = "\\t";
char __charclass_extend_esc_r[4] = "\\r";
char __charclass_extend_esc_a[4] = "\\a";

size_t VS_active_vlut_n_keys;
avsme * VS_active_vlut_table;
size_t VS_active_vlut_stride;

struct VS_RECENT_VLUT VS_RECENT_VLUT;

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

    case '>'    :
    case ')'    :
    case ']'    :
    case '}'    :
    RET_SUBCLASS = 1; goto MAINCLASS_4;
    case '<'    :
    case '('    :
    case '['    :
    case '{'    :
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
    
    printf("this");
    
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
                        ) /* Actual Function */
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