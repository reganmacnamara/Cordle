#include <stdio.h>
#include "Common.h"

void clearConsole()
{
    printf("\e[1;1H\e[2J");
    return;
}
