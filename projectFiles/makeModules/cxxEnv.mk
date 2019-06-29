# Global flags
CFLAGS = $(cflags) $(lib.cflags)
OBJ    = $(obj) $(lib.obj)

# Standalone flags
cflags = $(DFLAGS) $(IFLAGS) $(LFLAGS) $(LDFLAGS) $(CFLAGS_EXTRA) $(CFLAGS_CONFIGURE)

src := $(shell find src -name *.cpp)
dep := $(subst $(SRCDIR),$(COMPDIR),$(src:.cpp=.d))
obj := $(dep:.d=.o)
