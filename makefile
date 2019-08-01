#!/usr/bin/env make

PACKAGE     := SI-AssociateManagerBackend
MAKEMODULES := projectFiles/makeModules

-include $(MAKEMODULES)/defaultEnv.mk
-include $(MAKEMODULES)/findutils.mk

.DEFAULT_GOAL := all
.SUFFIXES :=

# Default flags

CPPFLAGS ?= -DDEBUG
CXXFLAGS ?= -g
LDFLAGS  ?=

# Mandatory flags

cppflags := -I$(INCLUDEDIR)
cxxflags := -std=c++17
ldflags  :=

# Project settings

EXEPATH := $(DISTDIR)/main.elf

src := $(call findutils_gensrclist,-type f -name *.cpp -print)
dep := $(subst $(SRCDIR),$(COMPDIR),$(src:.cpp=.d))
obj := $(dep:.d=.o)

-include $(MAKEMODULES)/libConfig.mk

ALL_CPPFLAGS := $(sort $(cppflags) $(CPPFLAGS) $(lib.cppflags))
ALL_CXXFLAGS := $(sort $(cxxflags) $(CXXFLAGS))
ALL_LDFLAGS  := $(sort $(ldflags) $(LDFLAGS) $(lib.ldflags))
ALL_OBJ      := $(sort $(obj) $(lib.obj))

-include $(MAKEMODULES)/GNUTargets.mk
-include $(MAKEMODULES)/targets.mk
-include $(MAKEMODULES)/cxxTargets.mk
-include $(MAKEMODULES)/inspector.mk
