# Cordle

Wordle in your terminal, written in C. Each game picks a random answer from a list of 14,855 five-letter words, so there's a new puzzle every time you run it.

## How to play

You have **6 guesses** to find the hidden five-letter word. After each guess, every letter is coloured:

| Colour | Meaning |
| --- | --- |
| 🟩 Green | Right letter, right spot |
| 🟨 Yellow | The letter is in the word, but somewhere else |
| 🟥 Red | The letter is not in the word |

The board is redrawn after every guess, so you can see all your earlier guesses. Guess the word to win. If you run out of guesses, the game shows you the answer.

Each guess must be a real word from the word list, typed in **lowercase**. Anything else is rejected and doesn't use up a turn, whether it's a typo, a made-up word or the wrong length.

### Differences from the original Wordle

- Letters that aren't in the word are shown in red, not grey.
- Repeated letters aren't counted. Every copy of a letter that appears in the answer turns yellow (or green), even if the answer has only one. For example, guessing `eerie` when the answer is `crane` shows the first two `e`s as yellow and the last one as green.
- The answer can be any word in the list, including rare ones like `joram` or `flawy`. The original picks its answers from a shorter list of common words.

## Requirements

- **GCC or Clang.** MSVC won't work, because the code uses POSIX threads and the `\e` escape.
- **POSIX threads** (`pthread.h`) and `unistd.h`, which the loading spinner uses. Linux and macOS have both. On Windows, use GCC from [MSYS2](https://www.msys2.org/) (MinGW-w64).
- **A terminal that supports ANSI colours.** On Windows, the game turns on colour support in the console automatically.

## Building

```bash
git clone https://github.com/reganmacnamara/Cordle.git
cd Cordle
gcc Main.c Wordle.c LoadingSpinner.c Common.c -pthread -o cordle
```

To use Clang, replace `gcc` with `clang`.

## Running

Run the game from the repository folder, because it loads `wordslist.txt` from the current directory:

```bash
./cordle
```

If you see "Error: unable to load words.", the game couldn't find `wordslist.txt`. Move into the repository folder and run it again.

A loading spinner shows for a few seconds while the word list loads. Then type a guess and press Enter. When the game ends, press Enter again to exit.

## Project layout

| File | What it does |
| --- | --- |
| `Main.c` | Entry point. Starts a game. |
| `Wordle.c` / `Wordle.h` | Game logic: loads the word list, picks the answer, checks and scores guesses, and draws the board |
| `LoadingSpinner.c` / `LoadingSpinner.h` | Loading animation that runs on a background thread |
| `Common.c` / `Common.h` | Shared helpers (clearing the screen) |
| `wordslist.txt` | The word list, used for both answers and valid guesses |

## Changing the word list

You can edit `wordslist.txt` to add or remove words. The game counts the words when it loads the list, so there's nothing to change in the code. A few rules apply:

- **One lowercase five-letter word per line.** Lines of any other length are skipped.
- **Keep the file in alphabetical order.** Guesses are looked up with a binary search, so a word that's out of order may be rejected even though it's in the list. To re-sort the file:

  ```bash
  LC_ALL=C sort -o wordslist.txt wordslist.txt
  ```

- **Every word can be an answer.** The same list supplies the answers and the valid guesses, so anything you add can come up as the hidden word.
- **On Windows, only the first 32,767 words can be answers.** That's the highest number `rand()` returns there. Words further down the list are still accepted as guesses, but they'll never be picked as the answer.
