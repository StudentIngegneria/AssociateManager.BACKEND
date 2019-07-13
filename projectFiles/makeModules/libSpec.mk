# External libraries makefile

# LIB SECTION: Pistache
libname := pistache

$(v_lib_dir)    := lib/pistache
$(v_lib_objdir) := $(lib_dir)/build/src
$(v_lib_obj)    := $(lib_objdir)/libpistache.a

$(v_lib_dflags) :=
$(v_lib_lflags) := -lpthread
$(v_lib_iflags) := -I$(lib_dir)/include

$(lib.pistache.objdir)/libpistache.a:
	mkdir -p $(lib.pistache.dir)/build
	cd $(lib.pistache.dir)/build ; cmake .. ; make

-include $(libEnable)

# LIB SECTION: Json for Modern C++
# NOTE: HEADER-ONLY LIBRARY

libname := nlohmann-json

$(v_lib_dir)    := lib/json
$(v_lib_objdir) :=
$(v_lib_obj)    :=

$(v_lib_dflags) :=
$(v_lib_lflags) :=
$(v_lib_iflags) := -I$(lib_dir)/single_include

-include $(libEnable)
