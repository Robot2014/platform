# Generic Makefile for C/C++ Program
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

include $(TOPDIR)/config.mk

CURDIR   = $(shell pwd)
SRCS     = $(CURDIR)/led.c
INCS     = $(TOPDIR)/cpu/stm32/cmsis/ \
		   $(TOPDIR)/cpu/stm32/stm32lib/inc/

OBJSDIR  = objs
DEPS_DIR = deps
LIBSDIR  = $(TOPDIR)/out/libs/static
LIB_TYPE = static
LIBS     = libled.a

MACRO_DEFINE += -DUSE_STDPERIPH_DRIVER -DSTM32F10X_MD
COMPAILFLAGS = -c -mcpu=cortex-m3 -mthumb -D__MICROLIB

include $(TOPDIR)/rules.mk
