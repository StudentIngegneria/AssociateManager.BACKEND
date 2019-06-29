# External libraries makefile

# LIB SECTION: Pistache
libname := pistache

$(v_lib_dir)    := lib/pistache
$(v_lib_objdir) := $(lib_dir)/build/src
$(v_lib_obj)    := $(lib_objdir)/libpistache.a

$(v_lib_dflags) :=
$(v_lib_lflags) := -lpthread
$(v_lib_iflags) := -Ilib/pistache/include

$(lib.pistache.objdir)/libpistache.a:
	mkdir -p $(lib.pistache.dir)/build
	cd $(lib.pistache.dir)/build ; cmake .. ; make

-include $(libEnable)
