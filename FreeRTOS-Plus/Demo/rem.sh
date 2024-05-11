#!/bin/bash

mkdir -p deleted

for d in */; do
    if [[ $d != *"FreeRTOS_Plus_TCP_Echo_Qemu_mps2"* && $d != *"Common"* && $d != *"deleted"* ]]; then
        mv "$d" "deleted/$d"
    fi
done
