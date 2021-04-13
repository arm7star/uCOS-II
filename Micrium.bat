@echo off
title Micrium
qemu-system-arm.exe -nographic -M versatilepb -m 128M -kernel ucos2 2>nul
