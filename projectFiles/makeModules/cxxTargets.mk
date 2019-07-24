$(EXEPATH): $(ALL_OBJ)
	mkdir -p $(dir $@)
	$(CC) -o $@ $^ $(ALL_LDFLAGS)

# Implicit rules

$(COMPDIR)/%.d: $(SRCDIR)/%.cpp
	mkdir -p $(dir $@)
	@echo "Generating make rule for $(subst .d,.o,$@)"

	@# Generate recipe dependencies
	$(CC) $< -o $@ -MM -MT $(subst .d,.o,$@) $(ALL_CPPFLAGS)

	@# Inject compilation instructions
	@echo -e "\t$(CC) -c -o $(subst .d,.o,$@) $^ $(ALL_CPPFLAGS)" >> $@

-include $(dep)
