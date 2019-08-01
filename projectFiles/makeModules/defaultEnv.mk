# Default paths

PREFIX         ?= /usr/local
EXEC_PREFIX    ?= $(PREFIX)
BINDIR         ?= $(EXEC_PREFIX)/bin
SBINDIR        ?= $(EXEC_PREFIX)/sbin
LIBDIR         ?= $(EXEC_PREFIX)/lib
LIBEXECDIR     ?= $(LIBDIR)
DATAROOTDIR    ?= $(PREFIX)/share
DATADIR        ?= $(DATAROOTDIR)
SYSCONFDIR     ?= $(PREFIX)/etc
SHAREDSTATEDIR ?= $(PREFIX)/com
LOCALSTATEDIR  ?= $(PREFIX)/var
RUNSTATEDIR    ?= $(LOCALSTATEDIR)/run
INCLUDEDIR     ?= $(PREFIX)/include
DOCDIR         ?= $(datarootdir)/doc/$(PACKAGE)

SUBMODDIR      ?= lib
SRCDIR         ?= src
BUILDDIR       ?= build
DISTDIR        ?= $(BUILDDIR)/dist
COMPDIR        ?= $(BUILDDIR)/files

# Defaults apps

SHELL      ?= /bin/sh
MAKE       ?= make
CC         ?= cc
CXX        ?= g++
LD         ?= ld
INSTALL    ?= install
PKG_CONFIG ?= pkg-config
FIND       ?= find

INSTALL_PROGRAM ?= $(INSTALL) -m 755
INSTALL_DATA    ?= $(INSTALL) -m 644
