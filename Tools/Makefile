# This Makefile is intended to simplify the rather complex process of toolchain creation. Obtaining the source code,
# the build and the installation (within the repository) are fully automated.
#
# USAGE:
#	- Use 'make install-tools' to download the source code, build and install the compiled files into the
# 	  configured installation folder.
#   - Use 'make uninstall-tools' to remove an existing toolchain installation. This is especially useful when you
#	  have customized the configuration and want to create a fresh build.
#   - Use 'make clean' to remove the toolchain build directories without uninstalling the toolchain.
#



# Configure the amount of parallel jobs here
export MAKEFLAGS += -j4

# Variables for source code retrieval
include source-config.mk
# Variables related to the build
include build-config.mk


# Path to the directory where the binutils sources are extracted
BINUTILS_BUILD_PATH = $(BUILD_PATH)/$(BINUTILS_SRC_NAME)
# Path to the binutils tarball
BINUTILS_TARBALL_PATH = $(BUILD_PATH)/$(BINUTILS_SRC_TARBALL)
# Path to the installed assembler
AS_INSTALL_PATH = $(TOOLCHAIN_PREFIX)/bin/$(TOOLCHAIN_TARGET)-as

# Path to the directory where the gcc sources are extracted
GCC_BUILD_PATH = $(BUILD_PATH)/$(GCC_SRC_NAME)
# Path to the gcc tarball
GCC_TARBALL_PATH = $(BUILD_PATH)/$(GCC_SRC_TARBALL)
# Path to the installed c compiler
GCC_INSTALL_PATH = $(TOOLCHAIN_PREFIX)/bin/$(TOOLCHAIN_TARGET)-gcc



.PHONY: install-tools uninstall-tools clean


install-tools: $(AS_INSTALL_PATH) $(GCC_INSTALL_PATH)


uninstall-tools:
	rm -fr $(TOOLCHAIN_PREFIX)/{bin,include,lib,libexec,share}
	rm -fr $(TOOLCHAIN_PREFIX)/$(TOOLCHAIN_TARGET)


clean:
	rm -fr $(BUILD_PATH)


$(BINUTILS_TARBALL_PATH):
	mkdir -p $(BUILD_PATH)
	curl $(BINUTILS_SRC_URL) -o $(BUILD_PATH)/$(BINUTILS_SRC_TARBALL)

$(AS_INSTALL_PATH): $(BINUTILS_TARBALL_PATH)
	rm -fr $(BINUTILS_BUILD_PATH)
	tar -xf $(BINUTILS_TARBALL_PATH) -C $(BUILD_PATH) --xz
	mkdir -p $(BINUTILS_BUILD_PATH)/build
	cd $(BINUTILS_BUILD_PATH)/build && \
		../configure \
			--target="$(TOOLCHAIN_TARGET)" \
			--prefix="$(TOOLCHAIN_PREFIX)" \
			--with-sysroot \
			--disable-nls \
			--disable-werror && \
		$(MAKE) && \
		$(MAKE) install


$(BUILD_PATH)/$(GCC_SRC_TARBALL):
	mkdir -p $(BUILD_PATH)
	curl $(GCC_SRC_URL) -o $(BUILD_PATH)/$(GCC_SRC_TARBALL)

$(GCC_INSTALL_PATH): $(AS_INSTALL_PATH) $(GCC_TARBALL_PATH)
	rm -fr $(GCC_BUILD_PATH)
	tar -xf $(GCC_TARBALL_PATH) -C $(BUILD_PATH) --xz
	mkdir -p $(GCC_BUILD_PATH)/build
	cd $(GCC_BUILD_PATH)/build && \
		PATH="$(TOOLCHAIN_PREFIX)/bin:$(PATH)" && \
		../configure \
			--target="$(TOOLCHAIN_TARGET)" \
			--prefix="$(TOOLCHAIN_PREFIX)" \
			--disable-nls \
			--enable-languages=c \
			--without-headers && \
		$(MAKE) all-gcc && \
		$(MAKE) all-target-libgcc && \
		$(MAKE) install-gcc && \
		$(MAKE) install-target-libgcc
