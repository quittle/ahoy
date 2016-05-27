// Example program
#include <iostream>
#include <string>

int main(const int argc, char const * const * const argv) {
    for (int i=0; i< argc; i++) {
        printf("arg%d=%s\n", i, argv[i]);
    }
}
