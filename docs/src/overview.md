# Overview

In this section, we will provide a basic overview of each library and its features.

## Casting Library

- [x] Standalone
- [x] Header-only

The Casting library is a header-only library that provides type casting and type checking utilities. It includes functions for checking if a value is of any of the specified types and performing dynamic casting in environments where source code was compiled with the `fno-rtti` flag.

The Casting library has the following requirements:

- C++20 or a newer standard
- LLVM-style RTTI setup
