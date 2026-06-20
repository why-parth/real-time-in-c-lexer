#include "svh_10_ccast.h"
#include v_implement



// Decl(vlut, 8, 8);

avsme VLUT_vlut_table[80] = {
    2, 3, 3, 5, 385, 0, 0, 0, 0, 0,
    108, 2, 109, 385, 0, 0, 0, 0, 0, 0,
    12288, 4, 12289, 25089, 30721, 385, 0, 0, 0, 0,
    4, 3, 5, 11777, 385, 0, 0, 0, 0, 0,
    256, 1, 385, 0, 0, 0, 0, 0, 0, 0
}; struct VLUT_vlut_veiw vlut = { 5, 0, 8, 8, VLUT_vlut_table };

int main(int argc, char * argv[]) {


    // CCAST_cast_state("project_002_code.c");

    Enable_C_Token_Lists;

    append_token_list(Functions, "printf");

    printf("\n\n\n");

    Vlut {
    
        Scan_Identifiers_3;
        
        Scan_Numbers_2_4;

        Scan_Spaces_2;

        Else_Vary_1;
        
        Save(vlut)
    
    }

    Open("project_002_code.c") Reader

    // CCAST_cast_VLUT(vlut);

    // printf("\n");

    if Opened {

        ReadLine(ln, 1024) {

            Using(vlut) Collect (ln) {

                Colorise_C_Tokens;
                
                token_print;

            }

        }

        CIMPL_DeColorise;
        CloseReader;
    }



    printf("\n\n\n");
    
    return 0;

}   




// fixed : VLU max value segmentaion bug
// added : formatted_string_of_collected
// fixed : COLLECT_OUT in the collected macros
// things to add : Modes
// fixed : VLU casting resolved
// fixed : For Join Vary now correctly have EXIST mask on by default
// added : Call keyword
// to-fix : VLU print functions
// added : VS_OFF_CHARCLASS_ERR, now we can toggle the ivalid case exit, 0 by default;

// to-add : static charclass and vlu flags -> currently charclass is only static and VLU is only dynamic, 



/*

    For idvalid
        Join Vary Mode(func)
        AVSME
        ASCII     8
        Variance  1
        SubClass  2
        MainClass 4
        Exists    1

*/