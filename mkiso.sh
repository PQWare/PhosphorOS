#!/bin/bash
#check PhOS.bin file is x86 multiboot file or not
grub-file --is-x86-multiboot PhOS.bin

#building the iso file
mkdir -p .isodir/boot/grub
cp PhOS.bin .isodir/boot/PhOS.bin
cp grub.cfg .isodir/boot/grub/grub.cfg
grub-mkrescue -o PhOS.iso .isodir

