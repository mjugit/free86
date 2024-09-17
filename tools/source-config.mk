BINUTILS_SRC_NAME = binutils-2.43
BINUTILS_SRC_TARBALL = $(BINUTILS_SRC_NAME).tar.xz
BINUTILS_SRC_MIRROR = https://ftp.gnu.org/gnu/binutils
BINUTILS_SRC_URL = $(BINUTILS_SRC_MIRROR)/$(BINUTILS_SRC_TARBALL)

ifndef BINUTILS_SRC_NAME
  $(error BINUTILS_SRC_NAME is not defined. Check the configuration in $(lastword $(MAKEFILE_LIST)))
endif

ifndef BINUTILS_SRC_MIRROR
  $(error BINUTILS_SRC_MIRROR is not defined. Check the configuration in $(lastword $(MAKEFILE_LIST)))
endif



GCC_SRC_NAME = gcc-14.2.0
GCC_SRC_TARBALL = $(GCC_SRC_NAME).tar.xz
GCC_SRC_MIRROR = https://ftp.gnu.org/gnu/gcc/$(GCC_SRC_NAME)
GCC_SRC_URL = $(GCC_SRC_MIRROR)/$(GCC_SRC_TARBALL)

ifndef GCC_SRC_NAME
  $(error GCC_SRC_NAME is not defined. Check the configuration in $(lastword $(MAKEFILE_LIST)))
endif

ifndef GCC_SRC_MIRROR
  $(error GCC_SRC_MIRROR is not defined. Check the configuration in $(lastword $(MAKEFILE_LIST)))
endif
