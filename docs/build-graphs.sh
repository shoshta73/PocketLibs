if [ ! -d "src/guides/assets/llvm-style-rtti" ]; then
  mkdir -p "src/guides/assets/llvm-style-rtti"
fi

time dot -Tsvg -o "src/guides/assets/llvm-style-rtti/inheritance-tree-starting-point.svg" "graphs/guides/llvm-style-rtti/inheritance-tree-starting-point.dot"
time dot -Tsvg -o "src/guides/assets/llvm-style-rtti/inheritance-tree-adding-types-no-subtree.svg" "graphs/guides/llvm-style-rtti/inheritance-tree-adding-types-no-subtree.dot"
time dot -Tsvg -o "src/guides/assets/llvm-style-rtti/inheritance-tree-adding-types-new-subtree.svg" "graphs/guides/llvm-style-rtti/inheritance-tree-adding-types-new-subtree.dot"
time dot -Tsvg -o "src/guides/assets/llvm-style-rtti/inheritance-tree-adding-types-new-subtree-exercise.svg" "graphs/guides/llvm-style-rtti/inheritance-tree-adding-types-new-subtree-exercise.dot"
