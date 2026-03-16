#!/usr/bin/env bash
set -e

# Directory
PREFIX="/usr/local/i386elfgcc"
TARGET="i386-elf"
NPROC=$(nproc)

echo "Installing in: $PREFIX"
sudo mkdir -p $PREFIX
sudo chown $(whoami) $PREFIX

# Download source
mkdir -p ~/src
cd ~/src

echo ">>> Downloading binutils and gcc..."
wget -c https://ftp.gnu.org/gnu/binutils/binutils-2.40.tar.xz
wget -c https://ftp.gnu.org/gnu/gcc/gcc-13.2.0/gcc-13.2.0.tar.xz

tar -xf binutils-2.40.tar.xz
tar -xf gcc-13.2.0.tar.xz

# Build binutils
echo ">>> Building binutils..."
mkdir -p build-binutils
cd build-binutils
../binutils-2.40/configure --target=$TARGET --prefix=$PREFIX --disable-nls --disable-werror
make -j$NPROC
make install
cd ..

# Prepare gcc
cd gcc-13.2.0
./contrib/download_prerequisites
cd ..

# Build gcc (stage 1)
echo ">>> Building GCC (stage 1)..."
mkdir -p build-gcc
cd build-gcc
../gcc-13.2.0/configure --target=$TARGET --prefix=$PREFIX --disable-nls \
    --enable-languages=c --without-headers --disable-multilib
make -j$NPROC all-gcc
make install-gcc

echo ">>> Done! Toolchain installed in $PREFIX"
echo ""
echo "Now you must add the toolchain to your PATH manually:"
echo ""
echo "Bash:"
echo "  Add the following line to your ~/.bashrc"
echo "    export PATH=\"/usr/local/i386elfgcc/bin:\$PATH\""
echo ""
echo "Zsh:"
echo "  Add the following line to your ~/.zshrc"
echo "    export PATH=\"/usr/local/i386elfgcc/bin:\$PATH\""
echo ""
echo "Fish:"
echo "  Add the following line to ~/.config/fish/config.fish"
echo "    set -Ua fish_user_paths /usr/local/i386elfgcc/bin"
echo ""
echo "To verify everything is working:"
echo "    i386-elf-gcc --version"
echo ""


