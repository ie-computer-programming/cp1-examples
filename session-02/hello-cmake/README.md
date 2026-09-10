# Session 2 — `hello-cmake`

The reference project from Session 2. It is deliberately tiny, but it has every
piece you will use for the rest of the course: a library, an executable that
links it, warnings turned up, and an out-of-source build.

```text
hello-cmake/
├── CMakeLists.txt      how to build
├── environment.yml     what to build it with
├── include/greet.h     the library's public interface
└── src/
    ├── greet.c         the library's implementation
    └── main.c          the program
```

## 1. Create the environment (once)

Don't have Conda installed yet? macOS: follow `examples/session-02/SETUP-MACOS.md`
first. Windows/Linux: install [Miniforge](https://github.com/conda-forge/miniforge)
for your OS, then continue below.

```bash
conda env create -f environment.yml
conda activate programming
```

Check it worked:

```bash
cmake --version
gcc --version      # cl.exe on Windows
```

## 2. Configure and build

```bash
cmake -S . -B build          # read CMakeLists.txt, generate the build system
cmake --build build          # actually compile
```

`-S .` is the source directory, `-B build` is where all generated files go.
Everything CMake produces stays inside `build/`, which is why `build/` is in
`.gitignore` — **never commit it**.

## 3. Run

```bash
./build/hello                # -> Hello, Computer Programming 1!
./build/hello Ada            # -> Hello, Ada!
```

On Windows the binary is at `build\Debug\hello.exe`.

## 4. Start over

```bash
rm -rf build
cmake -S . -B build && cmake --build build
```

Deleting `build/` is always safe and fixes most "CMake is behaving strangely"
problems.

## Try it

1. Add a `farewell()` function to the `greet` library (`greet.h` + `greet.c`)
   and call it from `main.c`.
2. Break the build on purpose: remove `#include <stdio.h>` from `greet.c` and
   read the error. Which stage of the toolchain is complaining?
3. Introduce an unused variable in `main.c`. `-Werror` turns that warning into
   a hard failure — that is the point.
