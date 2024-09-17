TOOLS_PATH = $(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))
REPOSITORY_PATH = $(shell dirname $(TOOLS_PATH))
BUILD_PATH = $(REPOSITORY_PATH)/build/tools

ifndef TOOLS_PATH
  $(error TOOLS_PATH is not defined. Check the configuration in $(lastword $(MAKEFILE_LIST)))
endif

ifndef REPOSITORY_PATH
  $(error REPOSITORY_PATH is not defined. Check the configuration in $(lastword $(MAKEFILE_LIST)))
endif

ifndef BUILD_PATH
  $(error BUILD_PATH is not defined. Check the configuration in $(lastword $(MAKEFILE_LIST)))
endif



TOOLCHAIN_PREFIX = $(TOOLS_PATH)
TOOLCHAIN_TARGET = i386-elf

ifndef TOOLCHAIN_PREFIX
  $(error TOOLCHAIN_PREFIX is not defined. Check the configuration in $(lastword $(MAKEFILE_LIST)))
endif

ifndef TOOLCHAIN_TARGET
  $(error TOOLCHAIN_TARGET is not defined. Check the configuration in $(lastword $(MAKEFILE_LIST)))
endif
