#!/usr/bin/env make

MAKEMODULES = projectFiles/makeModules

CC = g++
EXENAME = main.elf

DFLAGS += -DDEBUG
LFLAGS +=
IFLAGS +=
LDFLAGS +=

CFLAGS_EXTRA += -std=c++17

SRCDIR  := src
DISTDIR := build/dist
COMPDIR := build/files
EXEPATH = $(DISTDIR)/$(EXENAME)

.DEFAULT_GOAL := build

-include $(MAKEMODULES)/loadModules.mk

.PHONY: build
build: $(EXEPATH)

.PHONY: run
run:
	$(EXEPATH)

# Explicit rules

$(EXEPATH): $(OBJ)
	mkdir -p $(dir $(EXEPATH))
	$(CC) -o $@ $^ $(CFLAGS)
