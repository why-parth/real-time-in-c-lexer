#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "svh_08_cimpl.h"
#include v_implement

Decl(vlut, 8, 8);

int main(void) {

    Enable_C_Token_Lists;

    append_token_list(Functions, "printf");

    Vlut {
    
        Scan_Identifiers_3;
        
        Scan_Spaces_2;

        Scan_Numbers_2_4;
        
        Else_Vary_1;
        
        Save(vlut)
    
    }

    Open("project_002_code.c") Reader

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

    return 0;
}
