#include "Wordle.h"
#include "Common.h"
#include "LoadingSpinner.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#endif

#define WORD_LEN 5

// ANSI Colour Codes
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"

#define redNum 0
#define yellowNum 1
#define greenNum 2

typedef struct
{
    char letter;
    int colour;
} WordleLetter;

typedef struct
{
    WordleLetter letters[5];
} Guess;

int currentGuess = 0;
Guess guesses[6];
char (*words)[WORD_LEN + 1] = NULL;
size_t wordCount =0;

int compareWords(const void *a, const void *b)
{
    return strcmp(a, b);
}

// only matters for windows
void enableWindowsVirtualTerminal()
{
    #ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode = 0;
    if (GetConsoleMode(hOut, &mode))
        SetConsoleMode(hOut, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    #endif
}

bool isValidWord(const char *guess)
{
    return bsearch(guess, words, wordCount, sizeof *words, compareWords) != NULL;
}

bool loadWords()
{
    FILE *file = fopen("wordslist.txt", "r"); // r for read only
    if (file == NULL)
    {
        printf(RED "Error: could not find or open wordslist.txt.\n" RESET);
        return false;
    }

    size_t capacity = 1024;
    words = malloc(capacity * sizeof *words);
    if (words == NULL)
    {
        fclose(file);
        printf(RED "Error: Word List was empty.\n" RESET);
        return false;
    }

    char line[64];
    while(fgets(line, sizeof line, file) != NULL)
    {
        line[strcspn(line, "\r\n")] = '\0';
        if (strlen(line) != WORD_LEN)
            continue;
        
        if (wordCount == capacity)
        {
            capacity *= 2;
            void *bigger = realloc(words, capacity * sizeof *words);
            if (bigger == NULL)
            {
                fclose(file);
                return false;
            }
            words = bigger;
        }
        strcpy(words[wordCount++], line);
    }

    fclose(file);
    return wordCount > 0;
}

void handleGuess(char *guess, char *word)
{
    for (int i = 0; i < 5; i++)
    {
        char current = guess[i];
        if (word[i] == current)
        {
            guesses[currentGuess].letters[i].letter = guess[i];
            guesses[currentGuess].letters[i].colour = greenNum;
            continue;
        }

        bool wordContainsLetter = false;
        for (int j = 0; j < 5; j++)
        {
            if (current == word[j])
            {
                wordContainsLetter = true;
                break;
            }
        }

        if (wordContainsLetter)
        {
            guesses[currentGuess].letters[i].letter = guess[i];
            guesses[currentGuess].letters[i].colour = yellowNum;
        }
        else
        {
            guesses[currentGuess].letters[i].letter = guess[i];
            guesses[currentGuess].letters[i].colour = redNum;
        }
    }
}

void printGuesses(bool withInvalidMessage)
{
    clearConsole();
    if (withInvalidMessage)
    {
        printf("Guess any 5 letter word to start. You get 6 guesses to win. \n\n");
        printf(RED "Please guess a valid 5 letter word. \n\n" RESET);

    }
    else
    {
        printf("Guess any 5 letter word to start. You get 6 guesses to win. \n\n\n\n");
    }

    for (int i = 0; i < currentGuess; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            switch(guesses[i].letters[j].colour)
            {
                case greenNum:
                    printf(GREEN "[%c]" RESET, guesses[i].letters[j].letter);
                    break;
                case yellowNum:
                    printf(YELLOW "[%c]" RESET, guesses[i].letters[j].letter);
                    break;
                case redNum:
                    printf(RED "[%c]" RESET, guesses[i].letters[j].letter);
                    break;
            }
        }
        printf("\n");
    }
    printf("\n");
}

void PlayWordle()
{
    StartSpinner();
    char input[6] = "";
    char word[6] = "";

    #ifdef _WIN32
      enableWindowsVirtualTerminal();
    #endif

    srand(time(NULL));

    loadWords();
    strcpy(word, words[rand() % wordCount]);

    StopSpinner();
    clearConsole();

    if (strlen(word) != 5)
    {
        printf(RED "Error: could not generate word.\n" RESET);
        return;
    }

    printf("Guess any 5 letter word to start. You get 6 guesses to win. \n\n");

    while(true)
    {
        scanf("%s", input);
        bool isInvalidGuess = strlen(input) != 5 || !isValidWord(input);
        if (isInvalidGuess)
        {
            printGuesses(isInvalidGuess);
            continue;
        }
        handleGuess(input, word);
        currentGuess++;
        printGuesses(isInvalidGuess);

        if (!isInvalidGuess && compareWords(word, input) == 0)
        {
            printf(GREEN "You win in %d %s!\n" RESET, currentGuess, currentGuess == 1 ? "guess" : "guesses");
            break;
        }

        if (currentGuess == 6)
        {
            printf(RED "You lose! The word was '%s' \n" RESET, word);
            break;
        }
        memset(input, 0, sizeof(input));
    }
    return;
}
