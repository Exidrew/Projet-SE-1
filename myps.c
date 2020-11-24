#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    int i;
    printf("Myps lancé\n");
    printf("Avec les arguments :\n");

    for (i=0; i < argc; i++) printf("- %s\n", argv[i]);
    return 0;
}