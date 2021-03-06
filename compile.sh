#!/bin/bash
mkdir build
cd build
mosek_inc_dir=/usr/local/mosek/7/tools/platform/linux64x86/h
mosek_lib_dir=-L/usr/local/mosek/7/tools/platform/linux64x86/bin -lmosek64 -lmosekxx7_0 -lmosekjava7_0 -lmosekscopt7_0 -liomp5
build_dir=$PWD
inc_dir=${build_dir%%/}/include
lib_dir=${build_dir%%/}/lib
pkg_dir=${lib_dir%%/}/pkgconfig
PKG_CONFIG_PATH=${pkg_dir}:$PKG_CONFIG_PATH
export CXXFLAGS="-std=c++11 -g"
# configure and install CoinUtils
mkdir CoinUtils
cd CoinUtils
../../CoinUtils/configure --prefix=$build_dir
make -j 10 install
cd ..
# configure and install OsiMsk
mkdir Osi
cd Osi
#../../Osi/configure --prefix=$build_dir --with-mosek-incdir=$mosek_inc_dir --with-mosek-lib=$mosek_lib_dir
../../Osi/configure --prefix=$build_dir --with-mosek-incdir=/usr/local/mosek/7/tools/platform/linux64x86/h --with-mosek-lib="-L/usr/local/mosek/7/tools/platform/linux64x86/bin -lmosek64 -lmosekxx7_0 -lmosekjava7_0 -lmosekscopt7_0 -liomp5"
make -j 10 install
cd ..
#configure and install OsiConic
mkdir OsiConic
cd OsiConic
../../OsiConic/configure --prefix=$build_dir
make -j 10 install
cd ..
#configure and install OsiMosek
mkdir OsiMosek
cd OsiMosek
../../configure --prefix=$build_dir
make -j 10 install
cd ..
