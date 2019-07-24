#!/usr/bin/env make

PACKAGE     := SI-AssociateManagerBackend
MAKEMODULES := projectFiles/makeModules

-include $(MAKEMODULES)/defaultEnv.mk

.DEFAULT_GOAL := all
.SUFFIXES :=

# Default flags

CC := g++
CPPFLAGS ?= -DDEBUG -I$(INCLUDEDIR)
CFLAGS   ?= -g -std=c++17
LDFLAGS  ?=

# Project settings

EXEPATH := $(DISTDIR)/main.elf

src := $(shell find $(SRCDIR) -name *.cpp)
dep := $(subst $(SRCDIR),$(COMPDIR),$(src:.cpp=.d))
obj := $(dep:.d=.o)

-include $(MAKEMODULES)/libConfig.mk

ALL_CPPFLAGS := $(sort $(CPPFLAGS) $(lib.cppflags))
ALL_LDFLAGS  := $(sort $(LDFLAGS)  $(lib.ldflags))
ALL_OBJ      := $(sort $(obj) $(lib.obj))

-include $(MAKEMODULES)/GNUTargets.mk
-include $(MAKEMODULES)/targets.mk
-include $(MAKEMODULES)/cxxTargets.mk

-include $(MAKEMODULES)/inspector.mk
