#include <stdio.h>
#define VERLET_LEXER "Verlet Lexer v1.1.0"

#define add(a, b) (a + b)
#define mul (a, b) (a * b)

# if defined(mul) && !defined (NONE)

struct Person {
    int age;
    char name[32];
};

int main (int argc, char * argv[]) {

    printf(VERLET_LEXER);

    struct Person john = {23, "John"};

    int a = 34;

    _Generic((a),
        int : printf("\n a is an int.")
    );

    printf("\n 3 + 5 = %d ", add(3, 5));

    char string[] = "\n macro mul expands to '(a, b) (a * b)'.";

    return 0;
}

# endif
