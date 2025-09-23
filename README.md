# CodeForge QP/C++ Examples

A comprehensive collection of examples demonstrating the CodeForge QP/C++ host-based testing framework for fuzzing and testing QP/C++ active objects.

## Project Overview

This project showcases the **CodeForge QP/C++ testing framework** - a powerful host-based testing solution that enables fast, deterministic fuzzing and testing of QP/C++ Real-Time Embedded Framework applications. The framework provides libfuzzer integration while using the real QP/C++ framework rather than mocks or stubs, ensuring that tested behavior matches target behavior.

The examples demonstrate how to create fuzzable QP/C++ active objects that can uncover bugs, assertion failures, and other issues through automated fuzzing techniques.

## Architecture Overview

The project consists of several key components:

- **Example Applications**: Two demonstration fuzzers ([`if-statements.cpp`](qpcpp/if-statements.cpp) and [`magic-key.cpp`](qpcpp/magic-key.cpp)) showing different fuzzing approaches
- **CodeForge QP/C++ Framework**: A complete host-based testing framework located in [`qpcpp/codeforge-qpcpp/`](qpcpp/codeforge-qpcpp/)
- **Build System**: CMake-based build configuration with presets for easy compilation
- **External Dependencies**: QP/C++ framework and supporting libraries

## Features

### Core Testing Framework Features
- **libfuzzer-compatible port** of the QP/C++ framework for host-based fuzz testing
- **Complete QF lifecycle control** for deterministic, repeatable testing
- **Real QP/C++ framework integration** - no mocking surprises
- **Memory leak detection** and comprehensive event recording capabilities
- **Time acceleration** - test seconds of behavior in microseconds of host CPU time
- **Fast host-based testing** - thousands of test cases in seconds
- **CI/CD friendly** - easy integration with continuous integration systems

### Example Applications Features
- **Multiple fuzzing strategies** demonstrated through different examples
- **State machine testing** with QP/C++ hierarchical state machines
- **Event-driven testing** using QP/C++ publish-subscribe and direct posting
- **Assertion-based bug detection** for finding logic errors

## Project Structure

```
├── README.md                           # This file
├── CMakeLists.txt                      # Root CMake configuration
├── CMakePresets.json                   # CMake build presets
├── .gitmodules                         # Git submodule configuration
└── qpcpp/                              # QP/C++ examples directory
    ├── CMakeLists.txt                  # QP/C++ examples build configuration
    ├── if-statements.cpp               # If-statement fuzzing example
    ├── magic-key.cpp                   # Magic key sequence fuzzing example
    └── codeforge-qpcpp/                # CodeForge QP/C++ framework (submodule)
        ├── README.md                   # Framework documentation
        ├── LICENSE.txt                 # Dual-license information
        ├── CMakeLists.txt              # Framework build configuration
        ├── codeforge-qpcpp-lib/        # Core testing library
        │   ├── include/                # Public headers
        │   ├── src/                    # Implementation files
        │   └── cmake/                  # CMake support files
        ├── example/                    # Framework usage example
        └── externals/                  # External dependencies
            ├── qpcpp/                  # QP/C++ framework
            └── qassert-meta/           # Assertion metadata support
```

### Key Directories

- **[`qpcpp/codeforge-qpcpp-lib/`](qpcpp/codeforge-qpcpp/codeforge-qpcpp-lib/)**: Core testing framework library
- **[`qpcpp/codeforge-qpcpp-lib/include/`](qpcpp/codeforge-qpcpp/codeforge-qpcpp-lib/include/)**: Public API headers
- **[`qpcpp/codeforge-qpcpp-lib/cmake/`](qpcpp/codeforge-qpcpp/codeforge-qpcpp-lib/cmake/)**: CMake integration support
- **[`qpcpp/codeforge-qpcpp/externals/qpcpp/`](qpcpp/codeforge-qpcpp/externals/qpcpp/)**: QP/C++ framework source

## Prerequisites

### Required Tools
- **CMake 3.22+** and associated build tools (Ninja recommended)
- **clang compiler** (version 18.1.3+ recommended, required for libfuzzer support)
- **C++17 support** (required by the framework implementation)
- **Git** (for submodule management)

### System Requirements
- **Linux** (Ubuntu 20.04/22.04 tested and proven)
- **macOS** (should work with appropriate clang installation)
- **Windows** (with appropriate Unix-like environment)

### QP/C++ Framework
The framework automatically handles QP/C++ dependency in two ways:
- **Automatic**: Leave `QPCPP_TOP_DIR` undefined and CMake will fetch the appropriate QP/C++ repository
- **Manual**: Define `QPCPP_TOP_DIR` to point to your existing QP/C++ installation

## Building the Project

### Quick Start

1. **Clone the repository with submodules**:
   ```bash
   git clone --recursive https://github.com/your-repo/codeforge-examples.git
   cd codeforge-examples
   ```

2. **Configure the build**:
   ```bash
   cmake --preset qpcpp-example
   ```

3. **Build the examples**:
   ```bash
   cmake --build --preset qppcpp-example
   ```

### Manual Build Steps

If you prefer manual configuration:

```bash
# Create build directory
mkdir build && cd build

# Configure with CMake
cmake .. -G Ninja -DCMAKE_BUILD_TYPE=DEBUG -DCMAKE_QPCPP_EXAMPLE=true

# Build
ninja
```

### Build Outputs

After successful build, you'll find the fuzzer executables:
- `build/qpcpp/if-statements` - If-statement fuzzing example
- `build/qpcpp/magic-key` - Magic key sequence fuzzing example

## Running Examples

### Basic Execution

Run the fuzzers directly to test with random inputs:

```bash
# Run if-statements fuzzer
./build/qpcpp/if-statements

# Run magic-key fuzzer  
./build/qpcpp/magic-key
```

### Fuzzing with libfuzzer

For comprehensive fuzzing with libfuzzer options:

```bash
# Run with specific options
./build/qpcpp/if-statements -max_total_time=60 -print_final_stats=1

# Run with corpus directory
mkdir corpus
./build/qpcpp/magic-key corpus/ -max_total_time=60
```

### Common libfuzzer Options

- `-max_total_time=N`: Run for N seconds
- `-max_len=N`: Maximum input length
- `-print_final_stats=1`: Print statistics at end
- `-help=1`: Show all available options

## Example Applications

### If-Statements Example ([`if-statements.cpp`](qpcpp/if-statements.cpp))

This example demonstrates **nested conditional fuzzing** - a common pattern for finding deep logic bugs.

**Key Features**:
- **Deep nested if-statements** checking for the magic string "CodeForge"
- **Direct event posting** when magic string is detected
- **State machine transition** to assertion-triggering state
- **Simple fuzzing target** that processes raw byte arrays

**How it works**:
1. Fuzzer provides random byte array input
2. [`ReceiveData()`](qpcpp/if-statements.cpp:23) method checks for exact "CodeForge" sequence
3. If found, posts [`GO_ASSERT`](qpcpp/if-statements.cpp:7) event to active object
4. State machine transitions to [`be_assertive`](qpcpp/if-statements.cpp:51) state
5. Assertion failure is triggered, which libfuzzer detects as a crash

**Usage Pattern**:
```cpp
extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size)
{
    std::unique_ptr<Example> underTest = std::make_unique<Example>();
    underTest->ReceiveData(const_cast<uint8_t*>(data), size);
    return 0;
}
```

### Magic-Key Example ([`magic-key.cpp`](qpcpp/magic-key.cpp))

This example demonstrates **stateful fuzzing** using QP/C++ publish-subscribe events.

**Key Features**:
- **Byte-by-byte processing** through published events
- **Buffer accumulation** in active object state
- **Event-driven architecture** using QP/C++ publish-subscribe
- **Stateful bug detection** requiring specific sequence

**How it works**:
1. Fuzzer provides byte array, processes each byte individually
2. [`ReceiveByte()`](qpcpp/magic-key.cpp:30) creates and publishes [`ReceivedByte`](qpcpp/magic-key.cpp:6) events
3. Active object subscribes to [`PROCESS_BYTE`](qpcpp/magic-key.cpp:14) events
4. Each byte is accumulated in internal buffer
5. When buffer matches magic key "CodeForge", transitions to assertion state
6. Demonstrates more realistic event-driven embedded patterns

**Event Structure**:
```cpp
struct ReceivedByte : QP::QEvt
{
    uint8_t data;
};
```

**State Machine Behavior**:
- **Initial State**: Subscribe to events, transition to active
- **Active State**: Process bytes, compare with magic key
- **Be_Assertive State**: Trigger assertion failure

## Integration Guide

### Adding CodeForge QP/C++ to Your Project

1. **Add as Git Submodule**:
   ```bash
   git submodule add https://github.com/your-repo/codeforge-qpcpp.git external/codeforge-qpcpp
   ```

2. **Update CMakeLists.txt**:
   ```cmake
   add_subdirectory(external/codeforge-qpcpp)
   
   set(CODEFORGE_CMAKE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/codeforge-qpcpp/codeforge-qpcpp-lib/cmake")
   include(${CODEFORGE_CMAKE_DIR}/codeforge_qpcpp.cmake)
   
   # Create your fuzzer
   create_a_codeforge_fuzzer(my_fuzzer my_fuzzer.cpp)
   ```

3. **Write Your Fuzzer**:
   ```cpp
   #include "qpcpp.hpp"
   #include "codeforge_qpcpp.hpp"
   
   // Your active object implementation
   class MyActiveObject : public QP::QActive {
       // ... implementation
   };
   
   extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size)
   {
       // Setup framework
       cms::test::qf_ctrl::Setup();
       
       // Create and test your active object
       MyActiveObject underTest;
       underTest.start(1, nullptr, 0, nullptr, 0);
       
       // Process your input data
       // ... your testing logic
       
       // Cleanup
       cms::test::qf_ctrl::Teardown();
       return 0;
   }
   ```

### Framework Control API

The [`cms::test::qf_ctrl`](qpcpp/codeforge-qpcpp/codeforge-qpcpp-lib/include/codeforge_qf_ctrl.hpp) class provides precise control:

- **[`Setup(...)`](qpcpp/codeforge-qpcpp/codeforge-qpcpp-lib/include/codeforge_qf_ctrl.hpp:69)**: Initialize QF framework with configurable memory pools
- **[`Teardown()`](qpcpp/codeforge-qpcpp/codeforge-qpcpp-lib/include/codeforge_qf_ctrl.hpp:77)**: Clean shutdown with optional memory leak detection
- **[`ProcessEvents()`](qpcpp/codeforge-qpcpp/codeforge-qpcpp-lib/include/codeforge_qf_ctrl.hpp:81)**: Give CPU time to active objects
- **[`MoveTimeForward(...)`](qpcpp/codeforge-qpcpp/codeforge-qpcpp-lib/include/codeforge_qf_ctrl.hpp:88)**: Advance framework time, triggering timers
- **[`PublishEvent(...)`](qpcpp/codeforge-qpcpp/codeforge-qpcpp-lib/include/codeforge_qf_ctrl.hpp:96)**: Publish events into the framework

### Testing Patterns

**Pattern 1: Direct API Testing**
```cpp
// Test direct method calls
underTest->ProcessData(data, size);
```

**Pattern 2: Event-Driven Testing**
```cpp
// Test through published events
auto event = Q_NEW(MyEvent, MY_SIGNAL);
cms::test::qf_ctrl::PublishAndProcess(event);
```

**Pattern 3: Time-Based Testing**
```cpp
// Test timer behavior
cms::test::qf_ctrl::MoveTimeForward(std::chrono::seconds(5));
cms::test::qf_ctrl::ProcessEvents();
```

## Contributing

We welcome contributions to improve the CodeForge QP/C++ examples and framework!

### Getting Started

1. **Fork the repository**
2. **Create a feature branch**: `git checkout -b feature/my-new-feature`
3. **Make your changes** following the coding standards
4. **Add tests** for new functionality
5. **Commit your changes**: `git commit -am 'Add some feature'`
6. **Push to the branch**: `git push origin feature/my-new-feature`
7. **Create a Pull Request**

### Coding Standards

- **C++17** standard compliance
- **QP/C++ conventions** for active objects and state machines
- **Clear documentation** for public APIs
- **Comprehensive examples** for new features
- **libfuzzer compatibility** for all fuzzing examples

### Testing Guidelines

- All examples should compile without warnings
- Fuzzers should be able to find their target bugs within reasonable time
- New features should include both simple and complex usage examples
- Documentation should be updated for any API changes

## License

This project uses a **dual-license approach**:

### CodeForge QP/C++ Framework
- **Default**: GPL (GNU General Public License)
- **Commercial**: Available for commercial use
- **Contact**: Ryan Torvik at [info@tuliptreetech.com](mailto:info@tuliptreetech.com)
- **Website**: [https://tuliptreetech.com](https://tuliptreetech.com)

### External Dependencies
- **QP/C++**: Licensed by Quantum Leaps - see [https://www.state-machine.com/licensing](https://www.state-machine.com/licensing)
- **Other libraries**: Retain their respective licenses

### Commercial License
For commercial use of the CodeForge QP/C++ framework, please contact:
- **Ryan Torvik** - [info@tuliptreetech.com](mailto:info@tuliptreetech.com)
- **Tulip Tree Technology** - [https://tuliptreetech.com](https://tuliptreetech.com)

### Referral Program
If this project inspires your team to select QP/C++ for commercial use, please mention:
- "Matthew Eshleman"
- "Cove Mountain Software" 
- "Ryan Torvik"
- "Tulip Tree Technology"

in the referral field when acquiring a commercial license from Quantum Leaps. Referrals encourage and support efforts like this!

## Acknowledgements

- **Matt Eshleman** - Key influence and inspiration for this project
- **Cove Mountain Software** - Original CppUTest port for QP/C++: [https://github.com/covemountainsoftware/cpputest-for-qpcpp](https://github.com/covemountainsoftware/cpputest-for-qpcpp)
- **Quantum Leaps** - QP/C++ Real-Time Embedded Framework
- **LLVM Project** - libfuzzer fuzzing engine

## References

### Related Projects
- **Original inspiration**: [Unit Testing Active Objects and State Machines](https://covemountainsoftware.com/2020/04/17/unit-testing-active-objects-and-state-machines/)
- **Associated repository**: [https://github.com/covemountainsoftware/activeObjectUnitTestingDemo](https://github.com/covemountainsoftware/activeObjectUnitTestingDemo)

### Technical References
- **Sutter, Herb**: "Prefer Using Active Objects Instead of Naked Threads" - Dr. Dobbs, June 2010
- **Samek, Miro**: "Practical UML Statecharts in C/C++: Event-Driven Programming for Embedded Systems"
- **QP/C++ Documentation**: [https://www.state-machine.com/qpcpp](https://www.state-machine.com/qpcpp)
- **libfuzzer Documentation**: [https://llvm.org/docs/LibFuzzer.html](https://llvm.org/docs/LibFuzzer.html)

---

**Happy Fuzzing!** 🚀

For questions, issues, or contributions, please visit our project repository or contact the maintainers.