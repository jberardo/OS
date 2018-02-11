#include "timer_handler.h"

int main(void)
{
    struct timeval theTime;
    char p[256];

    gettimeofday(&theTime, NULL);

    printf("%ld\n", theTime.tv_sec);

    printf("Enter something: ");
    scanf("%s", p);
    printf("Ok. %s\n", p);

    return 0;
}