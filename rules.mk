#Generic Makefile for C/C++ Program
# 
# Author:  jundy.chen  
# Description: ------------
# This is an easily customizable makefile template. The purpose is to
# provide an instant building environment for C/C++ programs.
#
# It searches all the C/C++ source files in the specified directories,
# makes dependencies, compiles and links to form an executable.
#
# Besides its default ability to build C/C++ programs which use only
# standard C/C++ libraries, you can customize the Makefile to build # those using other libraries. Once done, without any changes you can
# then build programs using the same or less libraries, even if source
# files are renamed, added or removed. Therefore, it is particularly
# convenient to use it to build codes for experimental or study use.
#
# GNU make is expected to use the Makefile. Other versions of makes
#

.PHONY : all clean

# Top directory  Makefile

#MACRO_DEFINE += -DEBUGALL
CFLAGS +=$(COMPAILFLAGS) -g -O0 -Wall -Werror $(MACRO_DEFINE) 
# AR = ar
ARFLAGS = crv

# default output bin directory
ifeq ($(OUTPUTBIN),)
OUTPUTBIN = $(TOPDIR)/out/bin
endif

# defaulet libaray creat directory
ifeq ($(LIBSDIRS),)
LIBSDIR = $(TOPDIR)/out/libs
endif

# directory
DIRS = $(OBJSDIR) $(DEPS_DIR) $(OUTPUTBIN) $(LIBSDIR)

# include directory 
ifneq ($(INCS),"")
INCS := $(strip $(INCS))
INCS := $(addprefix -I,$(INCS))
endif

# when build execute file 
ifneq ($(EXES),)
EXES := $(addprefix $(EXESDIR)/,$(EXES))
RMS += $(EXES)
LIBSDIR := $(strip $(LIBSDIR))
LIBSDIR := $(addprefix -L,$(LIBSDIR))
endif

# when build static libaray file 
ifneq ($(LIBS),"")
LIBS := $(addprefix $(LIBSDIR)/,$(LIBS))
RMS += $(LIBS)
endif

# default source code file directory
ifeq ($(SRCSDIR),)
SRCSDIR = .
endif 

ifneq ($(ASM_FILE),y)
SRCS = $(wildcard $(SRCSDIR)/*.c)
OBJS = $(patsubst %.c, %.o,$(notdir $(SRCS)))
endif

OBJS := $(addprefix $(OBJSDIR)/,$(OBJS))
RMS += $(OBJS) $(OBJSDIR)

DEPS = $(patsubst %.c, %.dep,$(notdir $(SRCS)))
DEPS := $(addprefix $(DEPS_DIR)/,$(DEPS))
RMS += $(DEPS) $(DEPS_DIR)

ifneq ($(EXES),"")
all : $(EXES)
endif

ifneq ($(LIBS),"")
all : $(LIBS)
endif

ifneq ($(LINK_LIBS),"")
LINK_LIBS := $(strip $(LINK_LIBS))
LINK_LIBS := $(addprefix -l,$(LINK_LIBS))
endif

# include dependent files 
ifneq ($(MAKECMDGOALS), clean)
-include $(DEPS)
endif

$(DIRS):
	mkdir -p $@

# creat execute file
$(EXES) : $(OBJSDIR) $(OBJS) $(EXESDIR)
	$(CC) $(INCS) $(CFLAGS) -o $@ $(OBJS) $(LIBSDIR) $(LINK_LIBS)

# creat libaray file
$(LIBS) : $(LIBSDIR) $(OBJSDIR) $(OBJS)
# library type is static
ifeq ($(LIB_TYPE),static)
	$(AR) $(ARFLAGS) $@ $(OBJS)
endif

# library type is dynamic
ifeq ($(LIB_TYPE),dynamic)
	$(CC) -shared -o $@ $(OBJS)
endif

# creat object file 
ifeq ($(ASM_FILE),y)
$(OBJSDIR)/%.o : $(SRCSDIR)/%.s
	@echo "source asm file :" $<
	@echo "object file :" $@
	$(AS) $(INCS) $(COMPAILFLAGS) -o $@ $<
else

$(OBJSDIR)/%.o : $(SRCSDIR)/%.c
	@echo "source files:" $<
	@echo "object files:" $@

ifeq ($(LIB_TYPE),static)
	$(CC) $(INCS) $(CFLAGS) -o $@ -c $<
else
ifeq ($(LIB_TYPE),dynamic)
	$(CC) $(INCS) $(CFLAGS) -fPIC -o $@ -c $<
endif
endif
endif

# creat depandant file
$(DEPS_DIR)/%.dep : $(SRCSDIR)/%.c $(DEPS_DIR)
	@echo "creating depend file ..." $@
	@set -e;\
	$(CC) $(INCS) $(CFLAGS) -MM $< > $@.tmp;\
	sed 's,\($*\)\.o[ :]*,$(OBJS)/\1.o $@ : ,g' < $@.tmp > $@ \
	rm -f $@.tmp;

clean:
	rm -rf $(RMS)

show:
	@echo "execute file is "$(EXES)
	@echo $(SRCS)
	@echo $(OBJS)
	@echo $(DEPS)
	@echo $(LIBS)
	@echo $(DIR_LIBS)
	@echo $(DIR_DEPS)
	@echo $(TOPDIR)
	@echo $(LINK_LIBS)
	@echo $(DIR_LIBS)
	@echo $(DIR_INCS)
	@echo $(DIR_EXES)
	@echo $(MACRO_DEFINE)
	@echo $(CFLAGS)
