#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "vutils.h"     // Vutils

#include v_script       // Vscript
#include v_implement    // Required implementations

int main(void) {

    Vlut {  // Lexer's Behavior

        For idvalid
            Join idvalid
            Join numeric
            Vary All;

        For numeric
            Join numeric
            Vary All;

        For All
            Vary All;

        Make(vlut, 8, 8); // Storing Lexer's Behavior

    }
    
    char string_sample[] = "int Ta4_5 = 56;";   // Sample

    Using(vlut) Collect(string_sample) {    // Token Scanning Block

        token_show; // Showing the tokens

    }

    return 0;
}
