#!/bin/sh
cd `dirname $0`/..
mkdir -p speex_build_armeabi
cd speex_build_armeabi

cmake -DANDROID_ABI=armeabi -DCMAKE_TOOLCHAIN_FILE=../../cmake/android.toolchain.cmake $@ ../jni

make

cd ..

rm -rf ./libs/armeabi

mv ./jni/libs/armeabi ./libs/armeabi

rm -rf speex_build_armeabi

rm -rf ./jni/libs
