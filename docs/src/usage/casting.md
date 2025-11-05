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
auto GetPoints(const Shape *shape) -> int {
    if (isa<Triangle>(shape)) {
        return 3;
    } else if (isa<Parallelogram>(shape)) {
        return 4;
    } else if (isa<Ellipse>(shape)) {
        return 0;  // Ellipses don't have a fixed number of points
    }
    return 0;
}
```

**Key points:**
- Returns `true` if the object is of type `T` (or derived from `T`)
- Works with pointers, references, and smart pointers
- Zero overhead - compiles to simple enum comparisons

### Safe Casting with `dyn_cast<T>()`

The `dyn_cast<T>()` function performs a nullable cast - returns `nullptr` if the cast fails:

```cpp
std::unique_ptr<Shape> shape = std::make_unique<Rectangle>(5.0, 10.0);

// Safe cast - returns nullptr if shape is not a Rectangle
if (auto* rectangle = dyn_cast<Rectangle>(shape.get())) {
    std::println("Rectangle area: {}", rectangle->ComputeArea());
} else {
    std::println("Not a rectangle");
}
```

**When to use `dyn_cast<T>()`:**
- When you want to cast and check in one operation
- When failure is a valid outcome (you expect some casts to fail)
- Similar to `dynamic_cast` but works with `-fno-rtti`

### Asserting Cast with `cast<T>()`

The `cast<T>()` function performs a cast that **must succeed** (undefined behavior if it fails):

```cpp
// Only use if you're certain shape is a Rectangle
auto* rectangle = cast<Rectangle>(shape.get());
std::println("Rectangle area: {}", rectangle->ComputeArea());
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
std::unique_ptr<Shape> shape = std::make_unique<Square>(5.0);

// Check type
if (isa<Square>(shape)) {
    std::println("It's a square!");
}

// Safe cast with raw pointer
if (auto* square = dyn_cast<Square>(shape.get())) {
    std::println("Square area: {}", square->ComputeArea());
}
```

## Practical Example: Shape Hierarchy

Here's how the casting functions work together in a real scenario:

```cpp
int main() {
    // Create shapes and store in base class pointer
    std::unique_ptr<Shape> shape = std::make_unique<Rectangle>(5.0, 10.0);

    // Type checking
    std::println("Number of points: {}", GetPoints(shape.get()));

    // Safe casting
    if (auto* rectangle = dyn_cast<Rectangle>(shape.get())) {
        std::println("Rectangle area: {}", rectangle->ComputeArea());
    }

    // Works with derived types too
    shape = std::make_unique<Square>(5.0);

    // Square is a Rectangle, so this succeeds
    if (isa<Rectangle>(shape)) {
        std::println("Square is also a Rectangle!");
    }

    return 0;
}
```

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

## Complete Example

For a comprehensive example showing the full shape hierarchy with all casting functions, see the [complete example code](https://github.com/shoshta73/PocketLibs/blob/main/examples/casting_example.cxx).

## Next Steps

- **API Reference**: See the [Casting API Reference](../api/casting.md) for detailed function signatures
- **LLVM-style RTTI**: Learn how to set up your classes in the [LLVM-style RTTI Guide](../guides/LLVM-style-RTTI.md)
