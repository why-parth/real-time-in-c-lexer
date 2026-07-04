#include <stdio.h>
#include <stdlib.h>

#include <inttypes.h>

#define DYNAMIC_CHARCLASS

// #include "_07_cimpl.h"
#include "_06_ccast.h"

#undef charclass
#define charclass(c) (CHARCLASS_active[c])
avsme * CHARCLASS_charclass_active;
#undef dynamic_charclass 
#define dynamic_charclass(_charclass) CHARCLASS_charclass_active = mergetokens(STATE_charclass_, _charclass, _table)

avsme STATE_charclass_VERLET_charclass_table[128] = {
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

size_t VCo_enable_dtypes_size = 10;
hash dtypes[10] =  {
        15618947606512183472ULL, 2075867503335625021ULL, 17040593615128859721ULL, 0ULL, 
        0ULL, 0ULL, 0ULL, 0ULL, 0ULL, 
        0ULL
};

size_t VCo_enable_fs_size = 4;
hash fs[4] =  {
        10095453526808016325ULL, 10095453526808016325ULL, 590633490285535661ULL, 10095453526808016325ULL
};

size_t VCo_enable_DataTypes_size = 9;
hash DataTypes[9] =  {
        8771555764879172219ULL, 2075867503335625021ULL, 3860673601219843277ULL, 15618947606512183472ULL, 
        15016842284610189427ULL, 17040593615128859721ULL, 17980864930209666310ULL, 8499491795517106057ULL, 11985981782024586248ULL
};

size_t VCo_enable_StorageClasses_size = 6;
hash StorageClasses[6] =  {
        3367080083099074500ULL, 11804469591762768134ULL, 1961956309352788919ULL, 17192341199092675163ULL, 
        16715185031018741417ULL, 3906545778016713408ULL
};

size_t VCo_enable_TypeQualifiers_size = 4;
hash TypeQualifiers[4] =  {
        104687472692041790ULL, 12976474715163151613ULL, 17285007422211945341ULL, 4663619741248688771ULL
};

size_t VCo_enable_Controls_size = 13;
hash Controls[13] =  {
        590638987843676708ULL, 992083289751259946ULL, 8681100555297145957ULL, 2069114302916477617ULL, 
        16045561405319332056ULL, 15621798640163566894ULL, 113386091357634992ULL, 590644485401817774ULL, 3642835673441132614ULL, 
        941944122196744434ULL, 15247327299469644559ULL, 18139724688254833868ULL, 1051526405614789266ULL
};

size_t VCo_enable_Structures_size = 4;
hash Structures[4] =  {
        11039378071988874634ULL, 4091457719956660210ULL, 990106367844114946ULL, 0ULL
};

size_t VCo_enable_Specifiers_size = 1;
hash Specifiers[1] =  {
        2049136130262917362ULL
};

size_t VCo_enable_CompileTimeOperators_size = 6;
hash CompileTimeOperators[6] =  {
        6670039686589634269ULL, 5499162105642233271ULL, 17613526343828494687ULL, 5499155508572464108ULL, 
        16007573857825948151ULL, 16007562862709666016ULL
};

size_t VCo_enable_Assertion_size = 2;
hash Assertion[2] =  {
        8195886507573286236ULL, 5593587975483156543ULL
};

size_t VCo_enable_Pre_Processor_Commands_size = 15;
hash Pre_Processor_Commands[15] =  {
        18098855654928731743ULL, 17589041507092134308ULL, 4086679242421401341ULL, 4762949693224356033ULL, 
        14948090594069199551ULL, 10128900223766879656ULL, 3532856657788363644ULL, 10107636285869900921ULL, 9120109919684165173ULL, 
        15492660516509781155ULL, 15011177600702781647ULL, 11019417144907626317ULL, 590638987843676708ULL, 992076692681490811ULL, 
        992083289751259946ULL
};

size_t VCo_enable_Pre_Processor_Operators_size = 4;
hash Pre_Processor_Operators[4] =  {
        11108684926725206472ULL, 590712655122766909ULL, 590618097122740613ULL, 12638153115695167486ULL
};

char VLU_read_things_iden[] = "read_things";
avsme VLU_read_things_table[15] = {
    3, 3, 3, 5, 385,
    5, 3, 5, 11777, 385,
    257, 1, 385, 0, 0
}; struct VLU_VLU_view read_things = { VLU_read_things_table, VLU_read_things_iden, 3, 0, 3, 3 };


#include "_initialize_.h"
#include "_implement_.c"



void func (void) {
    printf("this is the func");
}

int main (void) {

    printf("\n\n");

    dynamic_charclass(VERLET_charclass);

    // Enable_C_Token_Lists;

    char string[] = "int main;";

    // VLU {

    //     For idvalid
    //         Join idvalid
    //         Join numeric
    //         Vary All;

    //     For numeric
    //         Join numeric
    //         Join '.' Char
    //         Vary All;

    //     For All
    //         Vary All;

    //     Make(read_things, 4, 10);

    // }

    

    Using(read_things);

    hash tkn_this = hash_function("this");

    Collect(string) {

        
        Colorise_C_Tokens;

        // token_show;
        if token_is(tkn_this) printf("this is here ->");

        if token_in(dtypes) printf("<data type>");

        if (!token_is(disjoin, space)) token_show;


    }

    printf("\n\n");

    return 0;
}




// to-add -> STATIC_VLU
// to-add -> DYNAMIC_CHARCLASS
// to-add -> CCast for token_list



/*
what are we able to do using the CCast

append dynamic charclass ()
...
append static tokenlist
append dynamic tokenlist
append dynamic VLU
...

issue to solve:
solve the charclass issue


ccast.dynamic.tokenlist()

*/