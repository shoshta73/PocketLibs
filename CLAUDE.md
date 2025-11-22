# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

PocketLibs is a collection of personal single-file, header-only C++ libraries. Currently contains:
- **casting.hxx**: Type casting and type checking utilities compatible with LLVM-style RTTI (Run-Time Type Information)

This is a header-only library project with extensive documentation built using mdBook.

## Requirements

- **C++ Standard**: C++20 or newer (C++23 for integration examples)
- **CMake**: Version 3.25 or higher
- **Documentation**: mdBook 0.4.52 with mdbook-alerts 0.8.0 plugin

## Common Commands

### Building Integration Examples

```bash
# Configure and build from repository root
cmake -B build
cmake --build build

# The example executable will be in build/expr_eval

# Or build a specific integration example
cd integration-example/casting
cmake -B build
cmake --build build
```

### Running Integration Examples

```bash
# After building from repository root
./build/expr_eval

# Or from the integration example directory
cd integration-example/casting
./build/expr_eval
```

### Documentation

```bash
# Build documentation locally (requires mdBook and mdbook-alerts)
cd docs
mdbook build

# The output will be in docs/book/

# Build graphs for documentation (requires Python 3.13+ and graphviz)
python ./docs/build_graphs.py
```

### Code Formatting

```bash
# Format all C++ files using clang-format
find . -name "*.cxx" -o -name "*.hxx" | xargs clang-format -i
```

## GitHub Workflows

The project uses GitHub Actions for continuous integration and deployment. All workflows use path-based triggers and only run on the `main` branch.

### Documentation Workflow (`.github/workflows/docs.yaml`)

**Triggers:**
- Push to `main` branch when files in `docs/**`, `docs/graphs/**`, or the workflow file itself change
- Manual dispatch via `workflow_dispatch`

**What it does:**
1. **Build graphs**: Generates SVG diagrams from Graphviz `.dot` files (with caching)
2. **Build documentation**: Compiles mdBook documentation with mdbook-alerts plugin
3. **Deploy**: Publishes documentation to GitHub Pages

**Caching:**
- Graphs are cached based on hash of graph sources
- Cargo tools (mdbook, mdbook-alerts) are cached

### Build Examples Workflow (`.github/workflows/build-examples.yaml`)

**Triggers:**
- Push to `main` branch when files in `integration-example/**`, `casting.hxx`, `CMakeLists.txt`, or the workflow file itself change
- Manual dispatch via `workflow_dispatch`

**What it does:**
Builds and runs integration examples on multiple platforms and compilers:

**Linux:**
- GCC 13
- Clang 18

**macOS:**
- Apple Clang (latest)

**Windows:**
- MSVC (latest)

All builds:
- Compile with C++23
- Use `-fno-rtti` flag (MSVC: `/GR-`)
- Run in Release mode
- Execute the compiled example to verify it works

**Purpose:**
Ensures integration examples compile and run successfully across all major platforms and compilers, validating that the library works correctly in real-world scenarios.

## Code Architecture

### Library Structure

The project follows a single-file library pattern where each library is a standalone `.hxx` header file in the repository root:
- **casting.hxx**: The main library file containing all casting utilities
  - Uses C++20 concepts and templates extensively
  - Requires `CASTING_NAMESPACE` macro to be defined by users
  - Provides `isa<T>()`, `cast<T>()` and `dyn_cast<T>()` functions
  - Works with pointers, references, smart pointers, and optionals
  - Compatible with LLVM-style RTTI pattern (requires `classof()` static method in classes)

### Documentation Structure

Documentation is organized in `docs/src/`:
- **overview.md**: Introduction to the library collection
- **usage/**: Step-by-step usage guides for each library
- **api/**: Detailed API reference documentation
- **guides/**: Educational guides (e.g., LLVM-style RTTI implementation tutorial)
- **assets/**: Generated graphs and images for documentation

The documentation uses mdBook with custom preprocessing:
- Python script (`docs/build_graphs.py`) generates SVG graphs from Graphviz `.dot` files
- GitHub Actions workflow builds and deploys to GitHub Pages
- Includes custom CSS for task lists and dark theme support

#### Documentation Best Practices

When writing or updating documentation:

**Use `{{#include}}` syntax wherever possible:**
- Include code snippets directly from source files using mdBook's `{{#include file:start:end}}` syntax
- This ensures documentation stays in sync with actual code
- Prefer including from integration examples over hardcoding examples
- Use line ranges to include specific sections: `{{#include ../../../integration-example/casting/src/main.cxx:12:30}}`

**Benefits:**
- **Accuracy**: Documentation always reflects actual working code
- **Maintainability**: Code changes automatically update documentation
- **Validation**: Included code compiles and runs in CI, preventing outdated examples
- **Single source of truth**: Integration examples serve as both tests and documentation

**Examples:**
```markdown
<!-- Good: Include from integration example -->
\`\`\`cpp
{{#include ../../../integration-example/casting/src/main.cxx:91:107}}
\`\`\`

<!-- Avoid: Hardcoded code that can drift out of sync -->
\`\`\`cpp
class Example {
    // This might become outdated...
};
\`\`\`
```

**When NOT to use `{{#include}}`:**
- Small, illustrative pseudo-code snippets that don't need to compile
- Code that would require extensive context to understand
- Tutorial sections teaching concepts step-by-step with incremental changes

### Integration Examples

Each library has practical integration examples in `integration-example/`:
- **integration-example/casting/**: Expression evaluator demonstrating real-world usage
  - Shows LLVM-style RTTI setup for an AST hierarchy
  - Demonstrates `isa<>`, `cast<>`, and `dyn_cast<>` in practice
  - Includes both CMake and Meson build configurations
  - Uses C++23 features (custom `std::formatter`, `std::println`)
  - Compiles with `-fno-rtti` to prove library works without runtime RTTI
  - Serves as a template for integrating PocketLibs into projects

## Code Style

- **C++ Files**: 4-space indentation, Google-style with modifications (see `.clang-format`)
- **CMake Files**: 8-space indentation
- **Line Length**: 120 characters maximum
- **Pointer/Reference Style**: Right-aligned (e.g., `Type *ptr`, `Type &ref`)
- **Naming**: Uses auto return types with trailing return type syntax for modern C++

## Key Patterns

### LLVM-Style RTTI Pattern

Libraries in this collection follow LLVM-style RTTI conventions:
1. Base class contains a type discriminator enum (e.g., `ShapeKind`)
2. Each class stores its kind in a const member
3. Each derived class implements static `classof(const Base*)` method
4. Type discriminators use range-based values for hierarchies

Example structure:
```cpp
class Base {
  public:
    enum class Kind { DerivedA, DerivedB };
    Kind GetKind() const { return kind; }
  private:
    const Kind kind;
};

class Derived : public Base {
  public:
    static auto classof(const Base *b) -> bool {
        return b->GetKind() == Kind::DerivedA;
    }
};
```

### Single-File Library Pattern

Each library:
- Is completely self-contained in a single `.hxx` file
- Uses header guards and namespace macros for user customization
- Checks C++ version at compile time
- Includes comprehensive Doxygen documentation
- Has no external dependencies beyond STL

## Testing Approach

The project uses integration examples rather than formal unit tests. When adding functionality:
1. Create or update integration examples in `integration-example/<library-name>/`
2. Ensure examples demonstrate real-world use cases and compile with both CMake and Meson
3. Examples should compile with `-fno-rtti` to validate the library's core value proposition
4. Update corresponding documentation in `docs/src/usage/` and `docs/src/api/`
5. Link to integration examples from the documentation for hands-on learning
