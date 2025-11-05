# Overview

PocketLibs is a collection of lightweight, single-file C++ libraries designed to solve common problems with minimal overhead.

## Why PocketLibs?

Modern C++ development often faces a trade-off: include a heavy dependency for one feature, or reinvent the wheel. PocketLibs provides a third option: **drop in a single header file** and get exactly what you need.

Each library is:
- **Single-file**: One `.hxx` file per library - no build system integration required
- **Header-only**: No linking, no compilation units, just include and use
- **Zero dependencies**: Only requires the C++ standard library
- **Production-ready**: Battle-tested patterns used in large-scale projects like LLVM

## Casting Library

### The Problem

When compiling C++ with `-fno-rtti` (common in embedded systems, game engines, and performance-critical applications), you lose `dynamic_cast` and `typeid`:

```cpp
// This doesn't work with -fno-rtti:
if (auto* triangle = dynamic_cast<Triangle*>(shape)) {
    // Do something with triangle
}
```

Implementing type checking yourself is error-prone and boilerplate-heavy.

### The Solution

The Casting library provides `isa<T>()`, `cast<T>()`, and `dyn_cast<T>()` functions that work perfectly with `-fno-rtti`:

```cpp
#include "casting.hxx"

// Type checking
if (isa<Triangle>(shape)) {
    auto* triangle = cast<Triangle>(shape);  // Safe cast
    // Do something with triangle
}

// Safe nullable cast
if (auto* triangle = dyn_cast<Triangle>(shape)) {
    // Returns nullptr if shape is not a Triangle
}
```

### Benefits

- **Performance**: Zero overhead compared to manual type checking - compiles down to simple enum comparisons
- **Type-safe**: Compile-time checks prevent invalid casts
- **Flexible**: Works with pointers, references, smart pointers (`std::unique_ptr`, `std::shared_ptr`), and `std::optional`
- **LLVM-compatible**: Uses the same RTTI pattern as LLVM, making it familiar to compiler developers

### Requirements

- **C++ Standard**: C++20 or newer
- **RTTI Setup**: Your classes need LLVM-style RTTI (see [Getting Started](./guides/LLVM-style-RTTI.md) for a step-by-step guide)

### Quick Example

```cpp
class Shape {
  public:
    enum class ShapeKind { SK_Circle, SK_Rectangle };
    ShapeKind GetKind() const { return kind; }
  protected:
    Shape(ShapeKind kind) : kind(kind) {}
  private:
    const ShapeKind kind;
};

class Circle : public Shape {
  public:
    Circle(double radius) : Shape(ShapeKind::SK_Circle), radius(radius) {}
    static bool classof(const Shape* s) { return s->GetKind() == ShapeKind::SK_Circle; }
  private:
    double radius;
};

// Now you can use type-safe casting without RTTI:
Shape* shape = GetShape();
if (isa<Circle>(shape)) {
    auto* circle = cast<Circle>(shape);
    // Work with circle
}
```

Ready to get started? Check out the [LLVM-style RTTI Setup Guide](./guides/LLVM-style-RTTI.md) for a complete walkthrough.
