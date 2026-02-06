# Copyright © 2026 Maximilian Jung
#
# Permission is hereby granted, free of charge, to any person
# obtaining a copy of this software and associated documentation files
# (the “Software”), to deal in the Software without restriction,
# including without limitation the rights to use, copy, modify, merge,
# publish, distribute, sublicense, and/or sell copies of the Software,
# and to permit persons to whom the Software is furnished to do so,
# subject to the following conditions:
#
# The above copyright notice and this permission notice shall be
# included in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
# BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
# ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
# CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#



LOCAL_DIR = $(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))
REPO_DIR = $(LOCAL_DIR)/../../..
TOOLS_DIR = $(REPO_DIR)/Tools

# Assembler
AS = $(TOOLS_DIR)/bin/i386-elf-as

# Archiver (for static libraries)
AR = $(TOOLS_DIR)/bin/i386-elf-ar

# Linker
LD = $(TOOLS_DIR)/bin/i386-elf-ld

# C compiler
CC = $(TOOLS_DIR)/bin/i386-elf-gcc


# Default C compiler flags
CFLAGS = -ffreestanding -O2 -Wall -Wextra -std=gnu99

# Use this version to enable debug symbols
DEBUG_CFLAGS = -ffreestanding -O2 -Wall -Wextra -std=gnu99 -g


BUILD_DIR = $(LOCAL_DIR)/Build

GFXTK_C = $(wildcard $(LOCAL_DIR)/GfxTk*.c)

GFXTK_SRC = $(GFXTK_C)

GFXTK_OBJ = $(patsubst 	$(LOCAL_DIR)/%.c, \
			$(BUILD_DIR)/%.o, \
			$(GFXTK_C))

GFXTK_OUT = $(BUILD_DIR)/GfxTk.o

GFXTK_LIB = $(BUILD_DIR)/LibGfxTk.a



# Source

.PHONY: gfxtk gfxtk-clean gfxtk-lib

gfxtk:  gfxtk-clean $(GFXTK_OUT)

gfxtk-lib: gfxtk-clean $(GFXTK_LIB)


$(GFXTK_LIB): $(GFXTK_OBJ) $(GFXTK_OUT)
	$(AR) -crs $(GFXTK_LIB) $(GFXTK_OBJ) $(GFXTK_OUT)


$(GFXTK_OUT): $(GFXTK_OBJ)
	$(LD) -r -o $@ $^

$(BUILD_DIR)/%.o: $(LOCAL_DIR)/%.c | $(BUILD_DIR)
	$(CC) -o $@ $(CFLAGS) -c $<


$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)


gfxtk-clean:
	rm -fr $(BUILD_DIR)/*.o
	rm -f $(BUILD_DIR)/*.a
	rm -f $(GFXTK_OUT)


# Tests

TESTS_DIR = $(LOCAL_DIR)/Tests

TESTS_BUILD_DIR = $(BUILD_DIR)/Tests

GFXTK_TEST_C = $(wildcard $(LOCAL_DIR)/Tests/*Test.c)

GFXTK_DEBUG_OBJ = $(patsubst	$(LOCAL_DIR)/%.c, \
				$(TESTS_BUILD_DIR)/%.dbg.o, \
				$(GFXTK_C))

GFXTK_DEBUG_OUT = $(TESTS_BUILD_DIR)/GfxTk.dbg.o	

GFXTK_TEST_OBJ = $(patsubst	$(TESTS_DIR)/%.c, \
				$(TESTS_BUILD_DIR)/%.o, \
				$(GFXTK_TEST_C))


TEST_CC = gcc

TEST_LD = ld

TEST_CFLAGS = -m32 -O0 -std=gnu99 -g



.PHONY: gfxtk-test-clean gfxtk-renderertest

gfxtk-renderertest: gfxtk-test-clean  $(GFXTK_DEBUG_OUT) $(TESTS_BUILD_DIR)/RendererTest



$(GFXTK_DEBUG_OUT): $(GFXTK_DEBUG_OBJ)
	$(TEST_LD) -m elf_i386 -r -o $@ $^

$(TESTS_BUILD_DIR)/%.dbg.o: $(LOCAL_DIR)/%.c | $(TESTS_BUILD_DIR)
	$(TEST_CC) -o $@ $(TEST_CFLAGS) -c $<

$(TESTS_BUILD_DIR)/%Test: $(TESTS_DIR)/%Test.c $(GFXTK_DEBUG_OUT) | $(TESTS_BUILD_DIR)
	$(TEST_CC) -o $@ $(TEST_CFLAGS) $(GFXTK_DEBUG_OUT) $<

$(TESTS_BUILD_DIR):
	mkdir -p $(TESTS_BUILD_DIR)

gfxtk-test-clean:
	rm -fr $(TESTS_BUILD_DIR)
