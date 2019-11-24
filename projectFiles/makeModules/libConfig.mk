enabledLibs      :=
lib.obj          :=
lib.cppflags     :=
lib.ldflags      :=
lib.cleanTargets :=

v_lib          = lib.$(libname)
v_lib_obj      = $(v_lib).obj
v_lib_objdir   = $(v_lib_obj)dir
v_lib_dir      = $(v_lib).dir
v_lib_cppflags = $(v_lib).cppflags
v_lib_cflags   = $(v_lib).cflags
v_lib_ldflags  = $(v_lib).ldflags
v_lib_clean    = $(v_lib).clean

lib_obj      = $($(v_lib_obj))
lib_objdir   = $($(v_lib_objdir))
lib_dir      = $($(v_lib_dir))
lib_cppflags = $($(v_lib_cppflags))
lib_cflags   = $($(v_lib_cflags))
lib_ldflags  = $($(v_lib_ldflags))

libEnable := $(MAKEMODULES)/libEnable.mk

include $(MAKEMODULES)/libSpec.mk

# TARGET: Build all libraries
.PHONY: libs
libs: $(enabledLibs)
