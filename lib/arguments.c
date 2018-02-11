#include <stdio.h>

int main(int argc, char *argv[])
{
    int i;
    char **p;

    printf("\n---Using array ---\n");
    for (i = 0; i < argc; i++)
    {
        printf("%s\n", argv[i]);
    }

    printf("\n---Using pointer ---\n");
    for (p = &argv[1]; *p != NULL; p++)
    {
        printf("%s\n", *p);
    }

    return 0;
}