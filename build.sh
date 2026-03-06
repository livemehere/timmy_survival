#!/bin/bash

if [ ! -d "build" ]; then
  cmake -B build -G Ninja
fi

echo "🚀 Build Starting..."

if cmake --build build; then
  echo "✅ Build Successful!"
  ./build/timmy-survival
else
  echo "❌ Build Failed!"
fi
