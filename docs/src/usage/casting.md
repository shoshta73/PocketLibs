# Using the Casting Library

For API reference, please refer to the [Casting API Reference](../api/casting.md).

## Prerequisites

Before using the Casting library, make sure:

- Your code compiles with **C++20 or newer**
- Your classes use **LLVM-style RTTI** (see [Setup Guide](../guides/LLVM-style-RTTI.md) if you haven't set this up yet)

## Installation

### Step 1: Download the Header

Get the `casting.hxx` header file and place it in your include directory:

```bash
wget https://raw.githubusercontent.com/shoshta73/PocketTools/main/casting.hxx -O include/casting.hxx
```

### Step 2: Configure Your Namespace

Define the `CASTING_NAMESPACE` macro in your build system:

**CMake:**

```cmake
target_compile_definitions(my_target PUBLIC CASTING_NAMESPACE=MyNamespace)
```

**Compiler flag:**

```bash
g++ -DCASTING_NAMESPACE=MyNamespace -std=c++20 main.cpp
```

### Step 3: Include the Header

```cpp
#include "casting.hxx"
```

## Basic Usage

### Type Checking with `isa<T>()`

The `isa<T>()` function checks if a value is of a specific type without performing a cast:

```cpp
{{#include ../../../integration-example/casting/src/main.cxx:143:153}}
```

**Key points:**

- Returns `true` if the object is of type `T` (or derived from `T`)
- Works with pointers, references, and smart pointers
- Zero overhead - compiles to simple enum comparisons

### Safe Casting with `dyn_cast<T>()`

The `dyn_cast<T>()` function performs a nullable cast - returns `nullptr` if the cast fails:

```cpp
{{#include ../../../integration-example/casting/src/main.cxx:127:140}}
```

**When to use `dyn_cast<T>()`:**

- When you want to cast and check in one operation
- When failure is a valid outcome (you expect some casts to fail)
- Similar to `dynamic_cast` but works with `-fno-rtti`

### Asserting Cast with `cast<T>()`

The `cast<T>()` function performs a cast that **must succeed** (undefined behavior if it fails):

```cpp
// Only use if you're certain the value is of the specified type
// After checking with isa<>, we could use cast<> (which asserts in debug mode)
auto *binOp = cast<BinaryOp>(expr);
std::println("Operation: {}", binOp->GetOpString());
```

**When to use `cast<T>()`:**

- When you've already verified the type with `isa<T>()`
- In debug builds, this may include assertions
- More efficient than `dyn_cast<T>()` (no null checks)

> [!WARNING]
> Using `cast<T>()` on the wrong type is **undefined behavior**. Use `dyn_cast<T>()` if you're not certain.

## Working with Smart Pointers

The casting functions work seamlessly with smart pointers:

```cpp
{{#include ../../../integration-example/casting/src/main.cxx:159:167}}
```

## Putting It All Together

Here's how the three casting functions work together in practice:

```cpp
void ProcessExpression(const Expr *expr) {
    // 1. Check the type with isa<>
    if (isa<Literal>(expr)) {
        // 2. Safe cast with dyn_cast<> - handles the check + cast
        if (auto *lit = dyn_cast<Literal>(expr)) {
            std::println("Found literal: {}", lit->GetValue());
        }
    } else if (isa<BinaryOp>(expr)) {
        // 3. Assert cast with cast<> - we already verified with isa<>
        auto *binOp = cast<BinaryOp>(expr);
        std::println("Binary operation: {}", binOp->GetOpString());

        // Process children recursively
        ProcessExpression(binOp->GetLeft());
        ProcessExpression(binOp->GetRight());
    }
}
```

**Best practices:**
- Use `isa<T>()` when you only need to check the type
- Use `dyn_cast<T>()` when you need both check and cast in one operation
- Use `cast<T>()` only after verifying the type with `isa<T>()`, for better performance

## Complete Working Example

For a full, working example showing LLVM-style RTTI setup and real-world usage, see the [expression evaluator integration example](https://github.com/shoshta73/PocketLibs/tree/main/integration-example/casting).

## Understanding Type Hierarchies

The casting functions respect inheritance hierarchies. For example, with this hierarchy:

```
Shape
├── Parallelogram
│   ├── Rhombus
│   └── Rectangle
│       └── Square
├── Triangle
│   ├── EquilateralTriangle
│   ├── IsoscelesTriangle
│   └── ScaleneTriangle
└── Ellipse
```

A `Square*` will pass these checks:

- `isa<Square>(square)` → `true`
- `isa<Rectangle>(square)` → `true` (Square inherits from Rectangle)
- `isa<Parallelogram>(square)` → `true` (Rectangle inherits from Parallelogram)
- `isa<Shape>(square)` → `true` (all shapes inherit from Shape)
- `isa<Triangle>(square)` → `false` (not related to Triangle)

## Next Steps

- **API Reference**: See the [Casting API Reference](../api/casting.md) for detailed function signatures
- **LLVM-style RTTI**: Learn how to set up your classes in the [LLVM-style RTTI Guide](../guides/LLVM-style-RTTI.md)
