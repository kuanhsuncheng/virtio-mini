#!/bin/bash

TOOLCHAIN=${TOOLCHAIN:-"aarch64-linux-gnu-"}
HOST="aarch64-linux-gnu"

CFLAGS_SETTING="-fPIC -Os"
CPPFLAGS_SETTING="-fPIC -Os"
LDFLAGS_SETTING=""
BUILD="x86_64-linux-gnu"
export CC="${TOOLCHAIN}gcc"
export CXX="${TOOLCHAIN}g++"
export LD="${TOOLCHAIN}ld -EL"
export AR="${TOOLCHAIN}ar"
export RANLIB="${TOOLCHAIN}ranlib"
export STRIP="${TOOLCHAIN}strip"


ROOT_DIR=`pwd`
CPU_CORE=32


TOOLCHAIN_LIBC_USR_LIB=`which $CC`/../$HOST/libc/usr/lib
TOOLCHAIN_LIBC_USR_LIB=`echo $TOOLCHAIN_LIBC_USR_LIB | sed 's/arm-linux-gnueabihf-gcc\///g'`
TOOLCHAIN_LIBC_LIB=`which $CC`/../$HOST/libc/lib
TOOLCHAIN_LIBC_LIB=`echo $TOOLCHAIN_LIBC_LIB | sed 's/arm-linux-gnueabihf-gcc\///g'`


# source download from -https://XXXXXXXXX
# Build for XXXXX library
Folder=""


#TEMPDIR=$ROOT_DIR/tmp_install
PREBUILD_DIR=$ROOT_DIR/../prebuilt/arm64


#cd $ROOT_DIR/$XXX_PACKAGE
mkdir -p install
cd $ROOT_DIR
echo -e "######## \e[01;33m building $XXX_PACKAGE \e[00m ########"

make clean

make CC=$CC ARCH=arm64 LIBFDT_DIR=$PREBUILD_DIR lkvm-static

#make --quiet install || exit 1
#mkdir -p $TEMPDIR/
#cp -rf $ROOT_DIR/$XXX_PACKAGE/install/bin/* $TEMPDIR

echo -e "######## \e[01;33m End building $XXX_PACKAGE \e[00m ########"

