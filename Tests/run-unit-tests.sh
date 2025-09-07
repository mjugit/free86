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

for TESTPRG in $(ls *.Tests); do
    if [ -x $TESTPRG ]; then
	echo "[${TESTPRG}]"
	./$TESTPRG
    fi
done

echo -e "==> FINISH UNIT TESTS\n\n"
