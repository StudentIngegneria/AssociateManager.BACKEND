# External libraries makefile

# LIB SECTION: Pistache
libname := pistache

$(v_lib_dir)      := lib/pistache
$(v_lib_objdir)   := $(lib_dir)/build/src
$(v_lib_obj)      := $(lib_objdir)/libpistache.a

$(v_lib_cppflags) := -I$(lib_dir)/include
$(v_lib_ldflags)  := -lpthread

$(lib_objdir)/libpistache.a:
	mkdir -p $(lib.pistache.dir)/build
	cd $(lib.pistache.dir)/build ; cmake .. ; make

$(v_lib_clean):
	cd $(lib.pistache.dir)/build ; make clean

-include $(libEnable)

# LIB SECTION: Json for Modern C++
# NOTE: HEADER-ONLY LIBRARY

libname := nlohmann-json

$(v_lib_dir)      := lib/json
$(v_lib_objdir)   :=
$(v_lib_obj)      :=

$(v_lib_cppflags) := -I$(lib_dir)/single_include
$(v_lib_ldflags)  :=

$(v_lib_clean):
# Nothing to clean

-include $(libEnable)
