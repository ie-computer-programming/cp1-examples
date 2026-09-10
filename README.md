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

## What is here

| | |
|---|---|
| [python-to-c.md](python-to-c.md) | **Python → C bridge.** Syntax lookup, the traps a Python background sets for you, and the errors that will confuse you first. Keep it open for the first few weeks. |
| [session-02/](session-02/) | Toolchain setup and the smallest useful CMake project |
| [session-03/](session-03/) | Error handling and assertions — ten programs in bad/good pairs |
| [session-04/](session-04/) | Unit testing with GTest, plus the sanitizers |
| [session-06/](session-06/) | Pointers |

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
