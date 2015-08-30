# Makefile for C/C++ Program
#
# Author:  
# Description:
# ------------
# This is an easily customizable makefile template. The purpose is to
# provide an instant building environment for C/C++ programs.
#
# It searches all the C/C++ source files in the specified directories,
# makes dependencies, compiles and links to form an executable.
#
# Besides its default ability to build C/C++ programs which use only
# standard C/C++ libraries, you can customize the Makefile to build
# those using other libraries. Once done, without any changes you can
# then build programs using the same or less libraries, even if source
# files are renamed, added or removed. Therefore, it is particularly
# convenient to use it to build codes for experimental or study use.
#
# GNU make is expected to use the Makefile. Other versions of makes
#
#
# Include the make variables (CC, etc...)
#

#TOPDIR := $(shell pwd)
# load ARCH, BOARD, and CPU configuration

ARCH   = arm
CPU    = stm32
CPU_VARIANT = cortex-m3
BOAED  = comic
VENDOR = none
PROJECT = quadcopter
SOC    = stm32f103

export  ARCH CPU BOARD VENDOR SOC CPU_VARIANT
 
ifndef CROSS_COMPILE
ifeq ($(HOSTARCH),ppc)
CROSS_COMPILE =
else
ifeq ($(ARCH),ppc)
CROSS_COMPILE = powerpc-linux-
endif
ifeq ($(ARCH),arm)
CROSS_COMPILE = arm-none-eabi-
#CROSS_COMPILE = arm-none-linux-gnueabi-
endif
ifeq ($(ARCH),i386)
ifeq ($(HOSTARCH),i386)
CROSS_COMPILE =
else
CROSS_COMPILE = i386-linux-
endif
endif
ifeq ($(ARCH),mips)
CROSS_COMPILE = mips_4KC-
endif
ifeq ($(ARCH),nios)
CROSS_COMPILE = nios-elf-
endif
ifeq ($(ARCH),nios2)
CROSS_COMPILE = nios2-elf-
endif 
endif
endif
AS  = $(CROSS_COMPILE)as
LD  = $(CROSS_COMPILE)ld
CC  = $(CROSS_COMPILE)gcc
CPP = $(CC) -E
AR  = $(CROSS_COMPILE)ar
NM  = $(CROSS_COMPILE)nm
STRIP    = $(CROSS_COMPILE)strip
OBJCOPY  = $(CROSS_COMPILE)objcopy
OBJDUMP  = $(CROSS_COMPILE)objdump
RANLIB   = $(CROSS_COMPILE)RANLIB
READEIF  = $(CROSS_COMPILE)readelf
READSIZE = $(CROSS_COMPILE)size 
NM       = $(CROSS_COMPILE)nm
