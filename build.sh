#!/usr/bin/env sh
cmake -Bbuild && cmake --build build
echo "[INFO] Files output to ./build/bin"