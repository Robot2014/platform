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

.PHONY: all clean version

# sofeware version information
VERSION = 1
PROJECT = x4
SUBLEVEL = 4
YEAL = 2015
RELEASE_VERSION = $(PROJECT).$(YEAL).$(VERSION).$(SUBLEVEL)

TOPDIR := $(shell pwd)

export TOPDIR
include $(TOPDIR)/config.mk

BSP := bsp

BUILD_DIRS += $(TOPDIR)/cpu/stm32/stm32lib \
			  $(TOPDIR)/cpu/stm32/cmsis \
			  $(TOPDIR)/cpu/stm32/cmsis/startup/gnu \
              $(TOPDIR)/cpu/stm32/sample

OBJS       += $(TOPDIR)/cpu/stm32/cmsis/startup/gnu/objs/startup_stm32f10x_md.o \
		      $(TOPDIR)/cpu/stm32/sample/objs/led.o

INCS        = -I$(TOPDIR)/stm32lib/inc \
			  -I$(TOPDIR)/cmsis

OUTPUT      = $(TOPDIR)/out/bin
OUT_FILE    = $(OUTPUT)/quadcopter
DIRS        += $(OUTPUT)

LINK_DIR    = $(TOPDIR)/out/libs/
LINK_DIR := $(strip $(LINK_DIR))
LINK_DIR := $(addprefix -L,$(LINK_DIR))

LINK_LIBS  := stm32 \
		      cmsis

LINK_LIBS  := $(addprefix -l,$(LINK_LIBS))

#SHARE_LIBS  = 

RMS += $(TOPDIR)/out/*

LINK_FILE   = $(TOPDIR)/cpu/stm32/cmsis/startup/gnu/$(CPU).lds


LDFLAGS =-fno-exceptions -ffunction-sections -fdata-sections -nostartfiles -Wl,--gc-sections,-T$(LINK_FILE)

all : $(DIRS) $(BSP) 

	$(CC) $(INCS) $(CFLAGS) -o $(OUT_FILE).elf $(OBJS) $(LINK_DIR) -static $(LINK_LIBS) $(LDFLAGS)
	$(OBJCOPY) -O ihex $(OUT_FILE).elf $(OUT_FILE).hex
	$(OBJCOPY) -O binary $(OUT_FILE).elf $(OUT_FILE).bin
# Extract info contained in ELF to readable text-files:
	$(READEIF) -a $(OUT_FILE).elf > $(OUT_FILE).info_elf
	$(READSIZE) -d -B -t $(OUT_FILE).elf > $(OUT_FILE).info_size
	$(OBJDUMP) -S $(OUT_FILE).elf > $(OUT_FILE).info_code
	$(NM) -t d -S --size-sort -s $(OUT_FILE).elf > $(OUT_FILE).info_symbol

$(DIRS) :
	mkdir -p $@

#$(OUT_FILE) :  
#	$(CC) $(INCS) $(CFLAGS) -o  $@  $(OBJS) $(LINK_DIR) -static $(LINK_LIBS) $(LDFLAGS)

$(BSP) :
	@echo "  ** version information: " $(RELEASE_VERSION) " **  "
	@echo "  ...... building ...... "
	@set -e;\
	for dir in $(BUILD_DIRS); do cd $$dir && $(MAKE); done 	
	@echo ""
	@echo "\33[35m ~@^_^@~ \33[m"
	@echo ""
	@echo "   Build Completed   "

clean:
	@echo "cleaning ...."
	for dir in $(BUILD_DIRS);\
	do\
		cd $$dir && $(MAKE) clean;\
	done
	rm -rf $(RMS)
	@echo ""
	@echo "Clean Completed"

version:
	@echo $(TOPDIR)
	@echo $(RELEASE_VERSION)
	@echo $(BUILD_DIRS)
	cd $(BUILD_DIRS) && make show
