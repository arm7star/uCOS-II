# Introduction
ARM Versatile/AB (ARM926EJ-S) μCOS-II V286u.

# μC/OS-II

µC/OS-II is based on µC/OS, The Real-Time Kernel which was first published in 1992. Thousands of people around the world are using µC/OS in all kinds of applications such as cameras, medical instruments, musical instruments, engine controls, network adapters, highway telephone call boxes, ATM machines, industrial robots, and many more. Nu merous colleges and Universities have also used µC/OS to teach students about real-time systems.

# Toolchain
## linux
gcc-arm-none-eabi-4_9-2015q3-20150921-linux
## Cygwin
Cygwin gcc-arm-none-eabi-4_9-2015q3-20150921-win32 make.exe

# Clean
make clean; make distclean; make clobber

# Build
make all

# Run
qemu-system-arm -nographic -M versatilepb -m 128M -kernel ucos2

```
this is from task2 0
this is from task1 0
this is from task0 0
Delay 1 second and print
this is from task0 1
Delay 1 second and print
this is from task1 1
this is from task0 2
Delay 1 second and print
this is from task2 1
this is from task0 3
Delay 1 second and print
```
