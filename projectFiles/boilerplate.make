CFLAGS := $(strip $(DFLAGS) $(IFLAGS) $(LFLAGS) $(LDFLAGS) $(CFLAGS_EXTRA) $(CFLAGS_CONFIGURE))

src := $(shell find src -name *.cpp)
obj := $(subst $(SRCDIR),$(COMPDIR),$(src:.cpp=.o))
dep := $(subst $(SRCDIR),$(COMPDIR),$(obj:.o=.d))

# PHONY targets

.PHONY: cleanobj
cleanobj:
	rm -f $(obj)

.PHONY: cleandep
cleandep:
	rm -f $(dep)

.PHONY: cleandist
cleandist:
	rm -rf build/dist

.PHONY: cleanall
cleanall:
	rm -rf build

# Implicit rules

$(COMPDIR)/%.d: $(SRCDIR)/%.cpp
	mkdir -p $(dir $@)
	@echo "Generating make rule for $(subst .d,.o,$@)"

	@# Generate recipe dependencies
	$(CC) $< -o $@ -MM -MT $(subst .d,.o,$@) $(CFLAGS)

	@# Inject compilation instructions
	@echo -e "\t$(CC) -c -o $(subst .d,.o,$@) $^ $(CFLAGS)" >> $@

# Include

-include $(dep)
