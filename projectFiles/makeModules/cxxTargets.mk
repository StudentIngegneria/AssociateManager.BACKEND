.PHONY: cleanobj
cleanobj:
	rm -f $(obj)

.PHONY: cleandep
cleandep:
	rm -f $(dep)

.PHONY: cleandist
cleandist:
	rm -rf $(DISTDIR)

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

-include $(dep)
