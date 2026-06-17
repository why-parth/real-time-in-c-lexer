// #define SELF_PRECOMPILATION
#include "svh_10_ccast.h"
#include v_implement


/*
#include <..\..\this is fine>
#include <../../this is fine>
#include <>
*/



#define premain 


void PC_process(char * path) {

    hash tkn_include = fnv("#include");
    hash tkn_quote = fnv("\"");

    char including = 0;
    char file_name[1024];

    OpenPtr(path) Reader

    if Opened {

        Vlut {

            For '#' Char
                Join idvalid
                Vary All;

            For idvalid
                Join disjoin Set
                Join '/' Char
                Join '\\' Char
                Join numeric
                Join idvalid
                Vary All;
            
            For disjoin
                Join disjoin
                Join '/' Char
                Join '\\' Char
                Join numeric
                Join idvalid Set
                Vary All;

            For All
                Vary All;

            Make(PreProcessingTokens, 8, 8);

        }
        
        Using(PreProcessingTokens);
        
        size_t tkn_count = 0;

        ReadLine(ln, 1000) {

            Collect(ln) {
                if (!tkn_count) if token_is(tkn_include) including = 1;
                
                if (including == 1) {
                    if token_is(tkn_quote) including = 2;
                }
                else if (including == 2) {
                    token_paste(file_name);
                    break;
                }
                
                if token_is_not(disjoin, space) tkn_count++;
            }

            if (including == 2) printf("including : %s\n", file_name);
            tkn_count = 0;
            including = 0;
        }

    }
       
    else
        color_fore(240, 100, 100),
        printf("<! Could not open the file \"%s\". !>", path),
        color_fore_reset;

    CloseReader;

}



int main (int argc, const char * argv[]) {

    printf("this is the pre-main running.\n");

    PC_process(".\\test2.c");
    
    return 0;
}