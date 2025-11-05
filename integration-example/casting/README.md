# Integration Example: Expression Evaluator

This example demonstrates how to integrate the PocketLibs casting library into a real project using CMake or Meson.

## What This Example Does

This is a simple mathematical expression evaluator that builds an Abstract Syntax Tree (AST) and uses the casting library to traverse and evaluate it. It demonstrates:

- **LLVM-style RTTI** setup for an AST node hierarchy
- **Type-safe casting** without runtime RTTI (`-fno-rtti`)
- **Integration** with CMake and Meson build systems
- **Practical usage** of `isa<>`, `cast<>`, and `dyn_cast<>` functions
- **C++23 features** including `std::format` and `std::println`

## Project Structure

```
integration-example/casting/
├── README.md              # This file
├── CMakeLists.txt         # CMake build configuration
├── meson.build            # Meson build configuration
├── include/
│   └── casting.hxx        # PocketLibs casting library (auto-downloaded)
└── src/
    └── main.cxx           # Expression evaluator example
```

## Building with CMake

```bash
# Configure
cmake -B build -DCMAKE_CXX_STANDARD=20

# Build
cmake --build build

# Run
./build/expr_eval
```

### CMake Features Demonstrated

- Downloading the casting library automatically
- Setting the `CASTING_NAMESPACE` macro
- Compiling with `-fno-rtti` to prove it works without runtime RTTI

## Building with Meson

```bash
# Configure
meson setup build

# Build
meson compile -C build

# Run
./build/expr_eval
```

### Meson Features Demonstrated

- Downloading dependencies
- Setting compile definitions
- Modern Meson best practices

## Example Output

```
Evaluating: (2 + 3) * 4
Result: 20

Evaluating: 10 / (5 - 3)
Result: 5

Tree structure:
  Binary Op: *
    Binary Op: +
      Literal: 2
      Literal: 3
    Literal: 4
```

## How It Works

### Node Hierarchy

The example creates an AST with these node types:

```
Expr (base class)
├── Literal (numbers like 42, 3.14)
└── BinaryOp (operations like +, -, *, /)
```

### Casting in Action

**Type checking with `isa<>`:**
```cpp
if (isa<Literal>(expr)) {
    // Handle literal
} else if (isa<BinaryOp>(expr)) {
    // Handle binary operation
}
```

**Safe casting with `dyn_cast<>`:**
```cpp
if (auto* lit = dyn_cast<Literal>(expr)) {
    return lit->GetValue();
}
```

**Asserting cast with `cast<>`:**
```cpp
// After verifying with isa<>
auto* binOp = cast<BinaryOp>(expr);
```

## Key Takeaways

1. **Single-file integration**: Just download `casting.hxx` - no complex dependencies
2. **Build system agnostic**: Works with CMake, Meson, or any build system
3. **Zero overhead**: The casting compiles to simple enum comparisons
4. **Type-safe**: Compiler catches casting errors at compile time

## Adapting to Your Project

To use the casting library in your own project:

1. Download `casting.hxx` to your include directory
2. Define `CASTING_NAMESPACE` in your build configuration
3. Set up LLVM-style RTTI in your classes (see main.cxx for examples)
4. Use `isa<>`, `cast<>`, and `dyn_cast<>` instead of `dynamic_cast<>`

## Next Steps

- **Full Documentation**: https://shoshta73.github.io/PocketLibs/
- **LLVM-style RTTI Guide**: https://shoshta73.github.io/PocketLibs/guides/LLVM-style-RTTI.html
- **API Reference**: https://shoshta73.github.io/PocketLibs/api/casting.html
