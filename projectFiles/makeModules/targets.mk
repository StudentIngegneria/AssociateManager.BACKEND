.PHONY: run
run:
	$(EXEPATH) $(RUNOPT)

.PHONY: genInterfaceOverride
genInterfaceOverride: $(genInterfaceOverride)
$(genInterfaceOverride):
	$(MAKE) -C lib/genInterfaceOverride.hpp
