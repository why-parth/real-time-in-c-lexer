#include <stdio.h>
#include <stdlib.h>

#include "svh_00_utils.h"
#include "_07_ccast.h"

#define charclass(c) STATE_charclass_table[c]
avsme STATE_charclass_table[128] = {
        0X000000, 0X000000, 0X000000, 0X000000, 0X000000, 0X000000, 0X000000, 0X000791, 0X000000, 0X000951, 
        0X000a31, 0X000000, 0X000000, 0X000d71, 0X000000, 0X000000, 0X000000, 0X000000, 0X000000, 0X000000, 
        0X000000, 0X000000, 0X000000, 0X000000, 0X000000, 0X000000, 0X000000, 0X000000, 0X000000, 0X000000, 
        0X000000, 0X000000, 0X00206d, 0X002127, 0X00220b, 0X00230f, 0X00240f, 0X002547, 0X002647, 0X00270b, 
        0X002829, 0X002949, 0X002a47, 0X002b47, 0X002c2d, 0X002d47, 0X002e4d, 0X002f47, 0X003005, 0X003105, 
        0X003205, 0X003305, 0X003405, 0X003505, 0X003605, 0X003705, 0X003805, 0X003905, 0X003a4d, 0X003b2d, 
        0X003c29, 0X003d13, 0X003e49, 0X003f0f, 0X00400f, 0X004163, 0X004263, 0X004363, 0X004463, 0X004563, 
        0X004663, 0X004763, 0X004863, 0X004963, 0X004a63, 0X004b63, 0X004c63, 0X004d63, 0X004e63, 0X004f63, 
        0X005063, 0X005163, 0X005263, 0X005363, 0X005463, 0X005563, 0X005663, 0X005763, 0X005863, 0X005963, 
        0X005a63, 0X005b29, 0X005c11, 0X005d49, 0X005e47, 0X005f23, 0X00600b, 0X006143, 0X006243, 0X006343, 
        0X006443, 0X006543, 0X006643, 0X006743, 0X006843, 0X006943, 0X006a43, 0X006b43, 0X006c43, 0X006d43, 
        0X006e43, 0X006f43, 0X007043, 0X007143, 0X007243, 0X007343, 0X007443, 0X007543, 0X007643, 0X007743, 
        0X007843, 0X007943, 0X007a43, 0X007b29, 0X007c47, 0X007d49, 0X007e27, 0X000000 
};

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

    CC_ccast();

    // CC_ccast_charclass_ASCII(charclass);

    return 0;
}