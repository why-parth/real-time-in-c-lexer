#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "svh_07_context.h"
#include v_implement

token_list(dtypes, 10) {

    keyword(int);
    keyword(char);
    keyword(float);

}

token_list(List);

token_list(control, 10) {

    keyword(if);
    keyword(else);
    keyword(return);

}


Decl(vlut, 8, 8);

int main(void) {

    enable_token_set(dtypes);
    enable_token_set(control);

    append_token_list(List, "typeof");

    Vlut {
        
        For idvalid
            Join idvalid
            Vary All;
        
        For disjoin space
            Join disjoin space
            Vary All;
        
        For All
            Vary All;
        
        Save(vlut)
    
    }

    
    char code[] = "if (typeof(a) == int) return 0;";


    Using (vlut) Collect(code) {

        if token_in(dtypes) color_fore(100, 150, 200);
        else if token_in(control) color_fore(200, 100, 100);
        else if token_in(List) color_fore(100, 200, 100);
        else if token_is(numeric) color_fore(200, 100, 200);
        else color_fore_reset;

        token_print;

    }
    
    return 0;
}
