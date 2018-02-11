#include <stdio.h>
#include <string.h>

#define MAX_REMIND 50
#define MSG_LEN 60

int read_line(char str[], int n);

int main (void)
{
    char reminders[MAX_REMIND][MSG_LEN +3];
    char day_str[3];
    char msg_str[MSG_LEN + 1];
    int day, i, j, num_remind = 0;

    for (;;)
    {
        if (num_remind == MAX_REMIND)
        {
            printf("-- No space left --\n");
            break;
        }

        printf("Enter day and reminder: ");
        scanf("%2d", &day);

        if (day == 0)
        {
            break;
        }

        sprintf(day_str, "%2d", day);
    }

    return 0;
}