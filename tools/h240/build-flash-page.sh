#!/bin/sh
set -eu

if [ "$#" -ne 1 ]; then
    echo "Usage: sh $0 OUTPUT_DIRECTORY" >&2
    exit 2
fi

source_dir=$(dirname "$0")
output_dir=$1
mkdir -p -- "$output_dir"
clang --target=arm-none-eabi -mcpu=cortex-m0 -mthumb -Os -ffreestanding -nostdlib -fuse-ld=lld \
    "-Wl,--build-id=none,-T,$source_dir/flash-page.ld,-Map,$output_dir/flash-page.map" \
    "$source_dir/flash-page.c" -o "$output_dir/flash-page.elf"
llvm-objcopy -O binary "$output_dir/flash-page.elf" "$output_dir/flash-page.bin"
(
    cd -- "$output_dir"
    sha256sum flash-page.bin flash-page.elf flash-page.map > SHA256SUMS
)
