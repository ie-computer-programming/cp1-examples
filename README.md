# Computer Programming 1 — code from the sessions

Runnable code for each session, plus reference material. Clone it once, then
`git pull` before each class.

```bash
git clone https://github.com/ie-computer-programming/cp1-examples
cd cp1-examples
git pull            # before each session, to get the latest
```

Everything here builds with the course environment:

```bash
conda activate programming
```

Not set up yet, or missing a C++ compiler? See
[session-02/SETUP-MACOS.md](session-02/SETUP-MACOS.md).

Building with VS Code's **CMake Tools** extension rather than the terminal?
Copy [.vscode/settings.json.example](.vscode/settings.json.example) to
`.vscode/settings.json`. Each session folder is a separate CMake project, and
without those settings they fight over a single build folder.

## What is here

| | |
|---|---|
| [python-to-c.md](python-to-c.md) | **Python → C bridge.** Syntax lookup, the traps a Python background sets for you, and the errors that will confuse you first. Keep it open for the first few weeks. |
| [session-02/](session-02/) | Toolchain setup and the smallest useful CMake project |
| [session-03/](session-03/) | Error handling and assertions — ten programs in bad/good pairs |
| [session-04/](session-04/) | Unit testing with GTest, plus the sanitizers |
| [session-06/](session-06/) | Pointers |
| [session-07/](session-07/) | Dynamic memory — `malloc`/`free`, leaks, use-after-free, and the sanitizer that finds them |
| [session-08/](session-08/) | Structures and unions — padding, ownership, shallow vs deep copy, tagged unions |
| [session-09/](session-09/) | File I/O — `fopen`/`fgets`/`fclose`, streams, `argv`, exit codes |
| [session-11/](session-11/) | Preprocessor and macros — include guards, parenthesisation, `#ifdef`, a debug logger |
| [session-12/](session-12/) | Bit manipulation — masks, shift traps, two's complement, flags, Morton codes, Quake's rsqrt |
| [session-13/](session-13/) | Enums and typedef — enums are ints, X-macros, unit-safe structs, function pointers |
| [session-14/](session-14/) | *Optional, not examined* — linked lists, covered fully in Data Structures & Algorithms |
| [session-15/](session-15/) | Recursion and backtracking — stack frames, runaway recursion, memoisation, permutations, N-queens |
| [session-16/](session-16/) | Threads — create/join, lifecycle, the `&i` bug, measured speed-up, a race to fix next time |
| [session-17/](session-17/) | Advanced threads — mutexes, atomics, check-then-act, deadlock, semaphores, barriers, OpenMP |

Each folder has its own `README.md` explaining what to run and what to look
for. Start there.

## How these are meant to be used

Most folders are **pairs**: a program that does something wrong, and the same
program done properly. Run both. The point is rarely visible in the source —
it shows up in the output, the exit code, or what a sanitizer says.

```bash
cd session-03
cmake -S . -B build && cmake --build build
./run-all.sh
```

Several programs **fail on purpose** — they segfault, abort, or leak. That is
the lesson, not a mistake.

## Nothing here is graded

This is code to read, run, break, and copy from. Your graded work lives in your
own assignment repositories.
