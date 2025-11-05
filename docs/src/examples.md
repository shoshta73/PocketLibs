# Integration Examples

The best way to learn how to use PocketLibs is through practical, real-world examples. Each integration example is a complete, standalone project that demonstrates how to integrate a library into your own codebase.

## Available Examples

### Expression Evaluator (Casting Library)

**Repository**: [`integration-example/casting/`](https://github.com/shoshta73/PocketLibs/tree/main/integration-example/casting)

A mathematical expression evaluator that builds and evaluates an Abstract Syntax Tree (AST). This example demonstrates:

- **LLVM-style RTTI setup** for a custom AST hierarchy
- **Type-safe casting** without runtime RTTI (`-fno-rtti`)
- **Build system integration** with both CMake and Meson
- **C++23 features** including custom `std::formatter` for enums
- **Real-world usage** of `isa<>`, `cast<>`, and `dyn_cast<>` functions

**Quick start:**
```bash
git clone https://github.com/shoshta73/PocketLibs.git
cd PocketLibs/integration-example/casting

# Build with CMake
cmake -B build
cmake --build build
./build/expr_eval

# Or build with Meson
meson setup build
meson compile -C build
./build/expr_eval
```

**What you'll learn:**
- How to set up LLVM-style RTTI for your own class hierarchies
- When to use `isa<>` vs `cast<>` vs `dyn_cast<>`
- How to configure your build system to use PocketLibs
- Patterns for traversing and working with type hierarchies

## Using Examples as Templates

Each integration example is designed to be copied and adapted for your own projects:

1. **Copy the structure**: Use the example's directory layout as a template
2. **Adapt the build files**: Modify `CMakeLists.txt` or `meson.build` for your project
3. **Replace the logic**: Keep the RTTI setup pattern, but implement your own classes
4. **Keep it simple**: The examples show minimal, focused implementations

## Example Structure

All integration examples follow this structure:

```
integration-example/<library-name>/
├── README.md              # Detailed explanation of the example
├── CMakeLists.txt         # CMake build configuration
├── meson.build            # Meson build configuration
├── include/
│   └── <library>.hxx      # Auto-downloaded during build
└── src/
    └── main.cxx           # Example source code
```

## Key Features

### Self-Contained

Each example includes everything needed to build and run:
- Complete build system configurations
- Automatic library download
- No external dependencies (beyond the C++ standard library)

### Educational

Examples are heavily commented to explain:
- Why certain patterns are used
- Common pitfalls to avoid
- Best practices for real-world usage

### Production-Ready

Examples demonstrate production-quality code:
- Proper error handling
- Clear separation of concerns
- Idiomatic modern C++ (C++20/C++23)
- Compiled with strict warning levels

## Building All Examples

From the repository root:

```bash
# CMake builds all integration examples
cmake -B build
cmake --build build

# Then run any example
./build/expr_eval
```

Or build examples individually by navigating to their directories.

## Contributing Examples

Have an interesting use case for PocketLibs? Consider contributing an integration example!

See the [integration examples README](https://github.com/shoshta73/PocketLibs/tree/main/integration-example) for guidelines on creating new examples.

## Next Steps

- **Try the examples**: Clone the repository and build them yourself
- **Read the code**: Each example is fully commented and self-explanatory
- **Adapt for your project**: Use the examples as templates for your own work
- **Share your experience**: Open an issue to share how you're using PocketLibs
