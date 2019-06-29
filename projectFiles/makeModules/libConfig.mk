enabledLibs :=
lib.obj     :=
lib.cflags  :=

v_lib        = lib.$(libname)
v_lib_obj    = $(v_lib).obj
v_lib_objdir = $(v_lib_obj)dir

lib_obj    = $($(v_lib_obj))
lib_objdir = $($(v_lib_objdir))

# For consistency

v_lib_dir    = $(v_lib).dir
v_lib_dflags = $(v_lib).dflags
v_lib_lflags = $(v_lib).lflags
v_lib_iflags = $(v_lib).iflags
v_lib_cflags = $(v_lib).cflags

lib_dir    = $($(v_lib_dir))
lib_dflags = $($(v_lib_dflags))
lib_lflags = $($(v_lib_lflags))
lib_iflags = $($(v_lib_iflags))
lib_cflags = $($(v_lib_cflags))

libEnable := $(MAKEMODULES)/libEnable.mk

-include $(MAKEMODULES)/libSpec.mk

# TARGET: Build all libraries
.PHONY: libs
libs: $(enabledLibs)
