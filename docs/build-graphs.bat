@echo off

if not exist "src/guides/assets/llvm-style-rtti" mkdir "src/guides/assets/llvm-style-rtti"

dot -Tsvg -o "src/guides/assets/llvm-style-rtti/inheritance-tree-starting-point.svg" "graphs/guides/llvm-style-rtti/inheritance-tree-starting-point.dot"
dot -Tsvg -o "src/guides/assets/llvm-style-rtti/inheritance-tree-adding-types-no-subtree.svg" "graphs/guides/llvm-style-rtti/inheritance-tree-adding-types-no-subtree.dot"
dot -Tsvg -o "src/guides/assets/llvm-style-rtti/inheritance-tree-adding-types-new-subtree.svg" "graphs/guides/llvm-style-rtti/inheritance-tree-adding-types-new-subtree.dot"
dot -Tsvg -o "src/guides/assets/llvm-style-rtti/inheritance-tree-adding-types-new-subtree-exercise.svg" "graphs/guides/llvm-style-rtti/inheritance-tree-adding-types-new-subtree-exercise.dot"
