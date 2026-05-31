#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "svh_06_readwrite.h"
#include v_implement

void detect_class(void) {

    static int class_state = 0;

    if token_is("class") token_show, class_state = 1;
    else token_print;

    expect("class") if (class_state) {

        if token_is("{") {
            class_state = 2;
            color_fore(240, 240, 100);
            printf("<block-starts>");
            color_fore(100, 240, 100);
        }

        else if (class_state && token_is("}")) {
            class_state = 0;
            color_fore(240, 240, 100);
            printf("<block-ends>");
            color_fore_reset;
            stop_expecting;
        }

        keep_expecting;
    }

}
   
Decl(vlut, 8, 8);

int main(void) {

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
        
        Save(vlut)
    
    }
    
    char code[][48] = {
        "#include <stdio.h>",
        "class Person {",
        "    int age = 45;",
        "    contact phoneNumber;",
        "    int dial(contact phoneNumber);",
        "}",
        "int main (void) {",
        "   printf(\"this is some C code!\");",
        "   return 0;",
        "}"
    };
    

    for (int i = 0; i < 10; i++) {

        Using (vlut) Collect(code[i]) {

            detect_class();

        }
        
        putchar('\n');
        
    }
    
    return 0;
}
