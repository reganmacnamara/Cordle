#include <stdio.h>
#include "Wordle.h"

int main()
{
    PlayWordle();
    
    printf("\nPress any key to exit.\n");
    int c = getchar();

    return 0;
}
