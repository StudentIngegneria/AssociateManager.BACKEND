# External libraries makefile

# TARGET: Build all libraries
.PHONY: libs
libs: lib_pistache

# LIB SECTION: Pistache

PISTACHEPATH := lib/pistache
PISTACHEBUILDPATH := $(PISTACHEPATH)/build

.PHONY: lib_pistache
lib_pistache: $(PISTACHEBUILDPATH)/src/libpistache.a

$(PISTACHEBUILDPATH)/src/libpistache.a:
	mkdir -p $(PISTACHEPATH)/build
	cd $(PISTACHEBUILDPATH) ; cmake .. ; make
