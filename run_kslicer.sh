#!/bin/bash
kslicer_directory="/home/frol/PROG/kernel_slicer"
current_directory=$(pwd)           # получаем текущую директорию и сохраняем в переменную
cd "$kslicer_directory" || exit 1  # переходим в директорию слайсера, запускать нужно из неё

$kslicer_directory/cmake-build-release/kslicer $current_directory/reinhard.cpp \
-mainClass "ReinhardTM" \
-pattern "ipv" \
-shaderCC "glsl" \
-stdlibfolder "$kslicer_directory/TINYSTL" \
-I$kslicer_directory/TINYSTL "ignore" \
-I$current_directory/external/LiteMath "ignore" \
-DKERNEL_SLICER \
-v 