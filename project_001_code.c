#include <stdio.h>
#define DEFINED "defined"
#define mul(a, b) (a*b)

struct Person {
    int age;
    char name[32];
};

int main (int argc, char * argv[]) {

    printf("This is all I have.");

    printf("2 * 4 = %d", mul(2, 4));

    char a = 'c';

    int list[] = {34, 6, 3};

    struct Person John = {18.5, "John"};

    return 0;
}
