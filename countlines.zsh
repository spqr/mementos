#!/bin/zsh

echo "LLVM:"
wc -l llvm/**/*.{cpp,h}

echo "Non-LLVM:"
wc -l mementos.c include/mementos.h.tmpl
