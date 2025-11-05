# Integration Examples

This directory contains practical, real-world examples demonstrating how to integrate PocketLibs into your projects.

Each example is a complete, standalone project with its own build system configuration (CMake and Meson) that you can use as a template for your own work.

## Available Examples

### [Casting Library](./casting/)

A mathematical expression evaluator that demonstrates:
- LLVM-style RTTI setup for an AST hierarchy
- Type-safe casting without runtime RTTI (`-fno-rtti`)
- Integration with CMake and Meson build systems
- Practical usage of `isa<>`, `cast<>`, and `dyn_cast<>` functions
- C++23 features including custom `std::formatter` for enums

**Quick start:**
```bash
cd casting
cmake -B build -DCMAKE_CXX_STANDARD=23
cmake --build build
./build/expr_eval
```

## Using These Examples

Each example is designed to be:
1. **Self-contained**: Everything needed to build and run is included
2. **Educational**: Code is well-commented to explain the concepts
3. **Template-ready**: Copy and adapt for your own projects
4. **Build-agnostic**: Includes both CMake and Meson configurations

## Adding Your Own Example

If you've created an interesting use case for PocketLibs and want to contribute an example:

1. Create a new subdirectory: `integration-example/your-example/`
2. Include both CMake and Meson build files
3. Add a detailed README explaining what the example demonstrates
4. Keep dependencies minimal (ideally just the PocketLibs header)
5. Make sure it builds with `-fno-rtti` to showcase the library's value

See the [casting example](./casting/) as a reference.
