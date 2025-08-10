#!/bin/bash

SCRIPT=$(realpath "$0")
SCRIPTPATH=$(dirname "$SCRIPT")

cd $SCRIPTPATH

make clean
make

if [ $? -ne 0 ]; then
    exit 1
fi

echo -e "\n\n==> START UNIT TESTS..."

echo "[LibMemory]"
./LibMemory.Tests

echo "[LibBitmap]"
./LibBitmap.Tests

echo "[LibHeap]"
./LibHeap.Tests

echo -e "==> FINISH UNIT TESTS\n\n"
