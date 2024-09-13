#!/bin/bash

# Append a row to the output table
function print_row {
    printf "  %-10s \t %-18s \t %s\n" "$1" "$2" "$3"
}

# Check whether $1 is an installed package using pacman and print the results
function print_dependency_info {
    PACKAGE_NAME=$1
    PACKAGE_STATE=
    PACKAGE_INFO=$(pacman -Q $PACKAGE_NAME 2>/dev/null)

    if [ -z "$PACKAGE_INFO" ]; then
        PACKAGE_STATE=$(echo -e "\e[1;31mMISSING\e[0m")
        PACKAGE_INFO="-"
    else
        PACKAGE_STATE=$(echo -e "\e[1;32mFOUND\e[0m")
    fi

    print_row "$PACKAGE_NAME" "$PACKAGE_INFO" "$PACKAGE_STATE"
}


# Package names for Arch Linux
REQUIRED_PACKAGES=("make" "bison" "flex" "mtools" "gmp" "libmpc" "mpfr" "texinfo" "libisoburn")

# Generate output table
print_row "DEPENDENCY" "PACKAGE" "STATE" 
for PACKAGE in ${REQUIRED_PACKAGES[@]}; do
    print_dependency_info $PACKAGE
done
