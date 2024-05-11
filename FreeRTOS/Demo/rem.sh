#!/bin/bash

mkdir -p deleted

for d in */; do
    if [[ $d != *"CORTEX_MPS2_QEMU_IAR_GCC"* && $d != *"Common"* && $d != *"deleted"* ]]; then
        mv "$d" "deleted/$d"
    fi
done
