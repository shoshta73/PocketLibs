# Casting Guide

For API reference, please refer to the [Casting API Reference](../api/casting.md).

## Requirements

Source code must be compiled with C++20 or a newer standard.

Your codebase must have LLVM-style RTTI set up.

## Usage Instructions

1. Obtain the `casting.h` header file and place it in your compiler's include path.

   ```terminal
   $ wget https://raw.githubusercontent.com/shoshta73/PocketTools/main/casting.h -O include/casting.h
   ```

2. Define the `CASTING_NAMESPACE` macro in your build system.

   ```cmake
   target_compile_definitions(my_target PUBLIC CASTING_NAMESPACE=MyNamespace)
   ```

3. If you are not already using LLVM-style RTTI, adapt your code to use it. If you're unsure how to do this, please refer to the [LLVM-style RTTI Guide](../guides/LLVM-style-RTTI.md).

4. Include the header file where you need it.
