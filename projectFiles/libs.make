libs:
	make lib_pistache

## This is for the Pistache library
PISTACHEPATH = lib/pistache
PISTACHEBUILDPATH = $(PISTACHEPATH)/build

.PHONY: lib_pistache
lib_ùpistache: $(PISTACHEBUILDPATH)/src/libpistache.a

$(PISTACHEBUILDPATH)/src/libpistache.a:
	mkdir -p $(PISTACHEPATH)/build
	cd $(PISTACHEBUILDPATH) ; cmake .. ; make
