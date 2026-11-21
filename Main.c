#include <stdio.h>
#include "Wordle.h"

int main()
{
    PlayWordle();
    
    printf("\nPress Enter to exit.");
    fflush(stdout);
    getchar();

    return 0;
}
