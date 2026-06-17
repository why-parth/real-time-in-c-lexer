#include <stdio.h>
#include <stdlib.h>

#include "svh_00_utils.h"
#include "_06_cimpl.h"

#include "_initialize_.h"
#include "_implement_.c"

token_list(dtypes, 10) {
    keyword(int);
    keyword(char);
    keyword(float);
}

void func (void) {
    printf("this is the func");
}

int main (void) {

    enable_token_list(dtypes);

    // uintptr_t mode_ptr = (uintptr_t)MODE;

    char string[] = "int main;";

    Enable_C_Token_Lists;


    VLU {

        For idvalid
            Join idvalid
            Join numeric
            Vary All;

        For numeric
            Join numeric
            Join '.' Char
            Vary All;

        For All
            Vary All;

        Show;

        Make(read_things, 4, 10);

    }

    Using(read_things);

    hash tkn_this = hash_function("this");

    Collect(string) {

        
        Colorise_C_Tokens;

        // token_show;
        if token_is(tkn_this) printf("this is here ->");

        if token_in(dtypes) printf("<data type>");

        if (!token_is(disjoin, space)) token_show;


    }

    return 0;
}