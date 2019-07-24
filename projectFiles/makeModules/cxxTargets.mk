$(EXEPATH): $(ALL_OBJ)
	mkdir -p $(dir $@)
	$(CXX) -o $@ $^ $(ALL_LDFLAGS)

# Implicit rules

$(COMPDIR)/%.d: $(SRCDIR)/%.cpp
	mkdir -p $(dir $@)
	@echo "Generating make rule for $(subst .d,.o,$@)"

	@# Generate recipe dependencies
	$(CXX) $< -o $@ -MM -MT $(subst .d,.o,$@) $(ALL_CPPFLAGS) $(ALL_CXXFLAGS)

	@# Inject compilation instructions
	@echo -e "\t$(CXX) -c -o $(subst .d,.o,$@) $^ $(ALL_CPPFLAGS) $(ALL_CXXFLAGS)" >> $@

-include $(dep)
