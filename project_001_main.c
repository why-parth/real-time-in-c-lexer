#include "svh_06_readwrite.h"
#include v_implement

Decl(vlut_default, 8, 8);

char show_token = 1;

hash tkn_toggle_string;
hash tkn_toggle_char;

hash tkn_hash;
hash tkn_include;
hash tkn_define;
hash tkn_newline;

hash tkn_open_paran;
hash tkn_close_paran;

hash tkn_open_curly;
hash tkn_close_curly;

hash tkn_open_square;
hash tkn_close_square;

// color blue
hash tkn_char;
hash tkn_int;
hash tkn_long;
hash tkn_float;
hash tkn_double;

hash tkn_void;
hash tkn_struct;
hash tkn_enum;
hash tkn_union;

// color red
hash tkn_return;
hash tkn_if;
hash tkn_else;
hash tkn_switch;
hash tkn_break;
hash tkn_continue;
hash tkn_while;
hash tkn_for;
hash tkn_do;
hash tkn_goto;
hash tkn_auto;
hash tkn_static;
hash tkn_register;
hash tkn_extern;
hash tkn_typedef;

hash callables[100];
int callables_i = 0;

void init_all_vlut(void);
void init_all_token(void);
void init_pre_defined_callables(void);

void default_checks(void);

int main (int argc, char * argv[]) {

    if (argc == 1) {
        color_fore(200, 0, 0);
        printf("No file provided!");
        color_fore_reset;
        exit(-1);
    }

    OpenPtr(argv[1]) Reader

    if Opened {

        init_all_vlut();
        init_all_token();
        init_pre_defined_callables();

        ReadLine(ln, 1024) {

            Using(vlut_default) Collect (ln) {

                default_checks();

                if (show_token) token_print;

                show_token = 1;
            }

        }

        CloseReader;
    }

    else {
        color_fore(200, 0, 0);
        printf("Could not open the file '");
        color_fore(200, 200, 0);
        printf(argv[1]);
        color_fore(200, 0, 0);
        printf("'.");
        color_fore_reset;
        exit(-1);
    }
    
    return 0;
}


void init_all_vlut(void) {

    Vlut {

        For idvalid
            Join idvalid
            Join numeric
            Vary All;

        For numeric
            Join numeric
            Join '.' Char
            Vary All;
        
        For disjoin space
            Join disjoin space
            Vary All;
        
        For All
            Vary All;

        Save(vlut_default)

    }

}

void init_all_token(void) {
    
    tkn_toggle_string = fnv("\"");
    tkn_toggle_char = fnv("'");

    tkn_hash = fnv("#");
    tkn_include = fnv("include");
    tkn_define = fnv("define");
    tkn_newline = fnv("\n");

    tkn_open_paran = fnv("(");
    tkn_close_paran = fnv(")");

    tkn_char = fnv("char");
    tkn_int = fnv("int");
    tkn_long = fnv("long");
    tkn_float = fnv("float");
    tkn_double = fnv("double");
    
    tkn_void = fnv("void");
    tkn_struct = fnv("struct");
    tkn_enum = fnv("enum");
    tkn_union = fnv("union");

    tkn_return = fnv("return");
    tkn_if = fnv("if");
    tkn_else = fnv("else");
    tkn_switch = fnv("switch");
    tkn_break = fnv("break");
    tkn_continue = fnv("continue");
    tkn_while = fnv("while");
    tkn_for = fnv("for");
    tkn_do = fnv("do");
    tkn_goto = fnv("goto");
    tkn_auto = fnv("auto");
    tkn_static = fnv("static");
    tkn_register = fnv("register");
    tkn_extern = fnv("extern");
    tkn_typedef = fnv("typedef");

    tkn_open_curly = fnv("{");
    tkn_close_curly = fnv("}");

    tkn_open_square = fnv("[");
    tkn_close_square = fnv("]");

}

void init_pre_defined_callables(void) {

    callables[callables_i++] = fnv("printf");
    callables[callables_i++] = fnv("scanf");
    callables[callables_i++] = fnv("malloc");

}

void default_checks(void) {

    static char define_state = 0;
    static char defined = 0;

    static char recent_iden[32];
    static hash line_mode = 0;

    static int in_scope = 0;

    static int reading_string = 0;
    static int reading_char = 0;

    if token_is(tkn_toggle_string) {
        reading_string = !reading_string;
        
        if (reading_string)
            color_fore(220, 220, 100);
        else {
            token_print;
            show_token = 0;
            color_fore_reset;
        }

        return;
    }

    else if (reading_string) return;

    if token_is(tkn_toggle_char) {
        reading_char = !reading_char;
        
        if (reading_char)
            color_fore(220, 220, 100);
        else {
            token_print;
            show_token = 0;
            color_fore_reset;
        }

        return;
    }

    else if (reading_char) return;
    
    else if ( token_is(numeric) ) {
        color_fore(200, 100, 200);
        token_print;
        color_fore_reset;
        show_token = 0;
    }

    else if (   token_is(tkn_char) || token_is(tkn_int) || token_is(tkn_long) || token_is(tkn_float) || token_is(tkn_double) ||   \
                token_is(tkn_void) || token_is(tkn_struct) || token_is(tkn_enum) || token_is(tkn_union)     ) {
        color_fore(120, 180, 240);
        token_print;
        color_fore_reset;
        show_token = 0;
        
        if (line_mode != 2 && !in_scope) line_mode = get_fnv;
    }

    else if (   token_is(tkn_return) || token_is(tkn_if) || token_is(tkn_else) || token_is(tkn_switch) || token_is(tkn_break) ||        \
                token_is(tkn_continue) || token_is(tkn_while) || token_is(tkn_for) || token_is(tkn_do) || token_is(tkn_goto) ||         \
                token_is(tkn_auto) || token_is(tkn_static) || token_is(tkn_register) || token_is(tkn_extern) || token_is(tkn_typedef)   ) {
        color_fore(240, 0, 0);
        token_print;
        color_fore_reset;
        show_token = 0;
    }

    else if (token_is(tkn_open_paran) || token_is(tkn_open_square) || token_is(tkn_open_curly)) {
        in_scope++;
        if ((!line_mode || line_mode == 2) || in_scope > 1 ) {
            color_fore((50*(in_scope + 1)) % 100 + 100, 0, (70*(in_scope * in_scope)) % (150*in_scope) + 40);
            token_print;
            color_fore_reset;
        }
        show_token = 0;
    }
    else if (token_is(tkn_close_paran) || token_is(tkn_close_square) || token_is(tkn_close_curly)) {
        color_fore((50*(in_scope + 1)) % 100 + 100, 0, (70*(in_scope * in_scope)) % (150*in_scope) + 40);
        token_print;
        color_fore_reset;
        in_scope--;
        show_token = 0;
    }

    else if (token_is(special) || token_is(operate)  || token_is(assign)) {
        color_fore(240, 0, 0);
        token_print;
        color_fore_reset;
        show_token = 0;
    }

    else {
        for (int i = 0; i < callables_i; i++) {
            if token_is(callables[i]) {
                color_fore(100, 180, 100);
                token_print;
                color_fore_reset;
                show_token = 0;
            }
        }
    }

    if ((line_mode == 2) || in_scope > 1) goto scanned;
    expect(line_mode) {
        
        show_token = 0;

        if (token_is(tkn_open_paran) || token_is(tkn_open_square) || token_is(tkn_open_curly)) {

                show_token = 1;
                line_mode = 2;color_fore(100, 180, 100);
                printf(" %s ", recent_iden);
                
                color_fore((50*(in_scope + 1)) % 100 + 100, 0, (70*(in_scope * in_scope)) % (150*in_scope) + 40);

                callables[callables_i++] = fnv(recent_iden);
                
            }

            
        keep_expecting;
    }
    scanned:


    expect(tkn_hash) {

        if token_is(tkn_newline) {

            color_fore_reset;
            show_token = 0;
            putchar('\n');

            define_state = 0;
            defined = 0;
            stop_expecting;
        }
        else {

            if token_is(tkn_include) {
                token_print;
                color_fore(220, 220, 100);
                show_token = 0;
            }
            
            expect(tkn_define) {

                if ((define_state == 0) && token_is(idvalid)) define_state = 1;
                else if ((define_state == 1) && token_is(disjoin, space)) define_state = 3;
                else if ((define_state == 1) && token_is(tkn_open_paran)) define_state = 2;
                else if ((define_state == 2) && token_is(tkn_close_paran)) define_state = 3;

                switch (define_state) {
                    case 1:
                        color_fore(100, 180, 100);
                        break;

                    case 2:
                        color_fore_reset;
                        break;

                    case 3:
                        color_fore_reset;
                        break;
                }

                if (define_state && !defined) {
                    defined = 1;
                    callables[callables_i++] = get_fnv;
                }
                
                keep_expecting;
            }

        }

        keep_expecting;
    }

    if token_is(tkn_hash) {
        color_fore(240, 0, 0);
    }

    if token_is(idvalid) {
        token_paste(recent_iden);
    }

    if token_is(tkn_newline) line_mode = 0;

}
