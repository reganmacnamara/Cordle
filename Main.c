#include <stdio.h>
#include "Wordle.h"

int main()
{
    PlayWordle();
    
    printf("\nPress any key to exit.");
    int c = getchar();

    return 0;
}
