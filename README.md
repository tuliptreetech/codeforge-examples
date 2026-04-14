# CodeForge Examples

Simple C++ examples demonstrating libfuzzer-based fuzz testing techniques.

## Project Overview

This project contains two standalone C++ examples that show how libfuzzer can find bugs through automated fuzzing. Both examples use plain C++ with no external dependencies — just clang and libfuzzer.

The examples are intentionally designed so that fuzzers *will* find the bugs, demonstrating how different code structures affect fuzzer difficulty.

## Project Structure

```
├── README.md                   # This file
├── CMakeLists.txt              # Root CMake configuration
├── CMakePresets.json           # CMake build presets
└── cpp/                        # C++ examples
    ├── CMakeLists.txt          # Build configuration
    ├── if-statements.cpp       # Nested if-statement fuzzing example
    └── state-machine.cpp       # State machine fuzzing example
```

## Examples

### If-Statements ([`cpp/if-statements.cpp`](cpp/if-statements.cpp))

Demonstrates **nested conditional fuzzing**. A `ReceiveData()` method checks a byte buffer for the exact string `"CodeForge"` using nested `if` statements. When the string is found, an `assert` fires.

This structure is easy for libfuzzer to solve because coverage feedback guides it character by character.

### State Machine ([`cpp/state-machine.cpp`](cpp/state-machine.cpp))

Demonstrates **stateful fuzzing**. A simple state machine processes bytes one at a time, tracking progress toward the magic string `"CodeForge"`. When all characters are matched in sequence, an `assert` fires.

This is harder for fuzzers than the nested-if version because state is accumulated across multiple `LLVMFuzzerTestOneInput` calls — the input is processed byte-by-byte within a single call, requiring the fuzzer to discover the full sequence.

## CodeForge VS Code Extension

This project is designed to be used alongside the **CodeForge VS Code extension**. The extension provides a purpose-built UI for running and managing libfuzzer campaigns directly from your editor — no command-line required.

With the extension, you can:
- Build and launch fuzzers from the editor sidebar
- Monitor fuzzer progress and corpus growth in real time
- Inspect and replay crash-triggering inputs
- Manage multiple fuzzer targets across your project

The `.codeforge/` directory in this repo is the extension's working directory — build artifacts and fuzzer configuration live there.

## Prerequisites

- **CMake 3.22+**
- **Ninja**
- **clang** (with libfuzzer support — standard on macOS and most Linux distros)

## Building

```bash
# Configure
cmake --preset cpp-example

# Build
cmake --build --preset cpp-example
```

Build outputs (relative to the build directory):
- `codeforge-cpp-if-statements-fuzz`
- `codeforge-state-machine-fuzz`

## Running

```bash
# Run the if-statements fuzzer
./.codeforge/fuzzing/build-cpp-example/codeforge-cpp-if-statements-fuzz

# Run the state machine fuzzer
./.codeforge/fuzzing/build-cpp-example/codeforge-state-machine-fuzz
```

### Common libfuzzer Options

```bash
# Run for 60 seconds with stats
./codeforge-cpp-if-statements-fuzz -max_total_time=60 -print_final_stats=1

# Run with a corpus directory
mkdir corpus
./codeforge-state-machine-fuzz corpus/ -max_total_time=60
```

- `-max_total_time=N` — stop after N seconds
- `-max_len=N` — maximum input length
- `-print_final_stats=1` — print statistics on exit
- `-help=1` — show all options

## License

- **Default**: GPL (GNU General Public License)
- **Commercial**: Available — contact Ryan Torvik at [info@tuliptreetech.com](mailto:info@tuliptreetech.com)
- **Website**: [https://tuliptreetech.com](https://tuliptreetech.com)

## Acknowledgements

- **LLVM Project** — libfuzzer
