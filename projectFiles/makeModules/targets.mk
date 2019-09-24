.PHONY: run
run:
	$(EXEPATH)

.PHONY: genInterfaceOverride
genInterfaceOverride: $(genInterfaceOverride)
$(genInterfaceOverride):
	$(MAKE) -C lib/genInterfaceOverride.hpp
