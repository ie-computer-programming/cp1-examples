# macOS setup — Session 2 tooling

Step-by-step for **Terminal.app**, on both Apple Silicon (M1–M4) and Intel Macs.
Do this once; every session after Session 2 assumes it already works.

## 1. Command Line Tools

Gives you `git`, `make`, `clang`, and the system headers everything else needs.

```bash
xcode-select --install
```

A dialog pops up — click **Install**, accept the licence, wait for it to finish.
Already installed? The command tells you so and exits immediately.

```bash
git --version   # should now print a version, not "command not found"
```

## 2. Miniforge (Conda)

We use **Miniforge**, not the full Anaconda distribution — it defaults to the
`conda-forge` channel this course's `environment.yml` relies on, and ships a
native build for Apple Silicon (no Rosetta translation).

```bash
cd ~/Downloads
curl -L -O "https://github.com/conda-forge/miniforge/releases/latest/download/Miniforge3-$(uname)-$(uname -m).sh"
bash "Miniforge3-$(uname)-$(uname -m).sh"
```

Accept the licence, accept the default install location, and answer **yes**
when it asks to run `conda init`. Then **close and reopen Terminal** (or
`source ~/.zshrc`) so your shell picks up the change.

```bash
conda --version    # confirms the install
```

Your prompt should now start with `(base)`.

## 3. Get the course tooling

From the course project folder (the one with `environment.yml`):

```bash
conda env create -f environment.yml
conda activate programming
```

Your prompt now starts with `(programming)`. Check the pieces:

```bash
cmake --version
clang --version
gdb --version
```

<a id="gdb-macos"></a>
> **`gdb` on macOS needs one extra step before it can attach to a process** —
> macOS's System Integrity Protection blocks debuggers by default. See
> [§6](#6-if-you-plan-to-use-gdb-directly-read-this) before Session 5.
> If you only ever debug through VS Code's **Run and Debug** panel (`F5`),
> you can skip this: VS Code uses `lldb` on macOS automatically, and `lldb`
> does not hit this restriction.

## 4. VS Code

Download from <https://code.visualstudio.com> and drag it into `Applications`,
or, if you already use Homebrew:

```bash
brew install --cask visual-studio-code
```

Open it, then install these extensions (`Cmd+Shift+X`, search, **Install**):

- **C/C++** (Microsoft)
- **CMake Tools** (Microsoft)
- **GitLens**
- **GitHub Copilot**

Enable the integrated terminal shortcut you will use constantly:
`` Ctrl+` `` opens/closes it, already pre-configured.

## 5. Git and GitHub

Set your identity once, globally:

```bash
git config --global user.name  "Your Name"
git config --global user.email "you@student.ie.edu"
```

Sign in to GitHub **inside VS Code**: click the account icon in the bottom-left
corner → **Sign in with GitHub**. This one sign-in covers both:

- pushing/pulling over HTTPS without typing a password every time, and
- activating **GitHub Copilot** (see the GitHub Education slide — sign up for
  the Student Developer Pack with your `@student.ie.edu` address first).

## 6. If you plan to use `gdb` directly, read this

Skip this section if you only debug through VS Code's `F5`. It is only
needed for typing `gdb` yourself in the terminal (Session 5).

1. **Keychain Access** → menu **Certificate Assistant → Create a Certificate**
   - Name: `gdb-cert`
   - Identity Type: **Self Signed Root**
   - Certificate Type: **Code Signing**
2. Double-click the new certificate → **Trust** → set **Code Signing** to
   **Always Trust**. Enter your password to confirm.
3. Restart, so the trust change takes effect.
4. Codesign your `gdb` binary with the debugger entitlement:

   ```bash
   cat > /tmp/gdb-entitlement.xml <<'EOF'
   <?xml version="1.0" encoding="UTF-8"?>
   <!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN"
     "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
   <plist version="1.0">
   <dict>
     <key>com.apple.security.cs.debugger</key><true/>
   </dict>
   </plist>
   EOF
   codesign --entitlements /tmp/gdb-entitlement.xml -fs gdb-cert "$(which gdb)"
   ```

5. Test it: `gdb --batch -ex run --args ./build/hello` should run to
   completion instead of printing a permissions error.

If this ever stops working after a macOS update, that is expected — Apple
tightens these restrictions periodically. Fall back to `F5` in VS Code.

## 7. Verify everything

```bash
cd examples/session-02/hello-cmake
cmake -S . -B build
cmake --build build
./build/hello
```

```text
Hello, Computer Programming 1!
```

That line, printed with no errors above it, means you are ready for Session 3.

## Troubleshooting

| Symptom | Likely cause | Fix |
|---|---|---|
| `zsh: command not found: conda` | Terminal not reopened after install | Close and reopen Terminal, or `source ~/.zshrc` |
| `xcrun: error: invalid active developer path` | Command Line Tools missing/removed | `xcode-select --install` again |
| `CMake Error: ... compiler not found` | `programming` env not active | `conda activate programming` |
| Miniforge installer names an `x86_64` file on an M-series Mac | Terminal running under Rosetta | Quit Terminal, right-click → **Get Info** → uncheck "Open using Rosetta", reopen |
| `gdb` prints a permission/ptrace error | SIP blocking it | Follow [§6](#6-if-you-plan-to-use-gdb-directly-read-this), or just use `F5` in VS Code |
