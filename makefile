#!/usr/bin/env make

CC = g++
EXENAME = main.elf

DFLAGS += -DDEBUG
LFLAGS += -lpthread
IFLAGS += -Ilib/pistache/include
LDFLAGS +=

CFLAGS_EXTRA += -std=c++17

SRCDIR  := src
DISTDIR := build/dist
COMPDIR := build/files
EXEPATH = $(DISTDIR)/$(EXENAME)

LIBOBJ += lib/pistache/build/src/libpistache.a

# Dummy target to select the real default target
.PHONY: default
default: build

-include projectFiles/boilerplate.make
-include projectFiles/libs.make

.PHONY: build
build: $(EXEPATH)

.PHONY: run
run:
	$(EXEPATH)

# Explicit rules

$(EXEPATH): $(obj)
	mkdir -p $(dir $(EXEPATH))
	$(CC) -o $@ $^ $(CFLAGS)
