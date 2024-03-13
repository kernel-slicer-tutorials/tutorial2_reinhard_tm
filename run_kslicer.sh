#!/bin/bash
if [ ! -f config.txt ]; then
  echo "File config.txt not found! Run 'setup.sh path/to/kslicer.exe path/to/kslicer/folder' first!"
  exit 1
fi
unset -v kslicer_exe kslicer_directory
for var in kslicer_exe kslicer_directory; do
  IFS= read -r "$var" || break
done < config.txt
current_directory=$(pwd)           # получаем текущую директорию и сохраняем в переменную
cd "$kslicer_directory" || exit 1  # переходим в директорию слайсера, запускать нужно из неё

$kslicer_exe $current_directory/reinhard.cpp \
-mainClass "ReinhardTM" \
-pattern "ipv" \
-shaderCC "glsl" \
-stdlibfolder "$kslicer_directory/TINYSTL" \
-I$kslicer_directory/TINYSTL "ignore" \
-I$current_directory/external/LiteMath "ignore" \
-DKERNEL_SLICER \
-v
