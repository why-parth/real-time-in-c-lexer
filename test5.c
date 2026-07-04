#include <stdlib.h>
#include "_06_ccast.h"

#include "_initialize_.h"
#include "_implement_.c"


token_list(DATA_TYPES, 10) (

    token_append() ("int");
    token_append() ("char");
    token_append() ("float");

)


token_list(FUNCTIONS);



// char TL_DATA_TYPES_type = 0;
// char * TL_DATA_TYPES_iden = "DATA_TYPES";
// size_t TL_DATA_TYPES_size = 10;
// hash TL_DATA_TYPES_buffer[10] = {
//         15618947606512183472ULL, 2075867503335625021ULL, 17040593615128859721ULL, 0ULL, 
//         0ULL, 0ULL, 0ULL, 0ULL, 
//         0ULL, 0ULL
// };

// char TL_FUNCTIONS_type = 0;
// char * TL_FUNCTIONS_iden = "FUNCTIONS";
// size_t TL_FUNCTIONS_size = 3;
// hash TL_FUNCTIONS_buffer[3] = {
//         14274269514499928794ULL, 14274269514499928794ULL, 14274269514499928794ULL
// };




int main (void) {

    // token_list_enable(DATA_TYPES);

    char string[] = "this23 is a valid iden but 34this is not.";

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


        Make(sentence_reader, 3, 3)

    }



    token_append(FUNCTIONS) ("main");
    // token_append(FUNCTIONS) ("printf");
    // token_append(FUNCTIONS) ("scanf");

    // CC_ccast_token_list(DATA_TYPES);

    CC_List_append(CC_Type_token_list(DATA_TYPES)) ; 

    // TL_token_list_view_of(DATA_TYPES);



    hash_set("main");



    // CC_ccast_token_list_static(
    //     TL_TOKENLIST_BUFFER(DATA_TYPES),
    //     TL_TOKENLIST_SIZE(DATA_TYPES),
    //     TL_TOKENLIST_IDEN(DATA_TYPES)
    // );

    // CC_ccast_token_list_dynamic(
    //     TL_TOKENLIST_BUFFER(FUNCTIONS),
    //     TL_TOKENLIST_SIZE(FUNCTIONS),
    //     TL_TOKENLIST_IDEN(FUNCTIONS)
    // );


    // free_token_list(FUNCTIONS);

    if token_in(FUNCTIONS) printf("this is all");


    // Using(sentence_reader);


    // Collect(string) {


    //     token_show;
        
    //     if token_is("iden") printf("<correction : identifier>");

    //     if token_is(numeric) printf("<NUMBER>");

    //     if 
        
    //     token_in(LIST) 
        
        
    //     printf("<LIST>");

    // }


    return 0;
}