#include "Common.h"
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

#include "LoadingSpinner.h"

pthread_t AnimationThread;
bool continueSpinner = true;

void* Animate(void* arg)
{
    clearConsole();
    printf("Loading -");
    for (;;) {
        if (!continueSpinner)
            break;
        sleep(1);
        clearConsole();
        printf("Loading \\");
        sleep(1);
        clearConsole();
        printf("Loading |");
        sleep(1);
        clearConsole();
        printf("Loading /");
        sleep(1);
        clearConsole();
        printf("Loading -");
    }
    return NULL;
}

void StartSpinner()
{
    pthread_create(&AnimationThread, NULL, Animate, NULL);
    return;
}

void StopSpinner()
{
    continueSpinner = false;
    pthread_join(AnimationThread, NULL);
    pthread_cancel(AnimationThread);
    return;
}
