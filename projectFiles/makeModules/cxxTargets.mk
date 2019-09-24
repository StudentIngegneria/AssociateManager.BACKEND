$(EXEPATH): $(ALL_OBJ)
	mkdir -p $(dir $@)
	$(CXX) -o $@ $^ $(ALL_LDFLAGS)

# Implicit rules

$(COMPDIR)/%.d: $(COMPDIR)/%.cpp
	mkdir -p $(dir $@)
	echo "Generating make rule for $(subst .d,.o,$@)"

# Generate recipe dependencies
	$(CXX) -o $@ $(subst $(COMPDIR),$(SRCDIR),$<) \
		-MM -MT $(subst .d,.o,$@) $(ALL_CPPFLAGS) $(ALL_CXXFLAGS)

# Patch the generated recipe to request source files from $(COMPDIR)
	tr '[:space:]' '\n' < '$@' |                  \
	sed -e 's!\\$$!!' -e '/^$$/d' -e 's!$$! \\!'  \
	  -e 's!$(SRCDIR)/!$(COMPDIR)/!' > '$@.new'

	mv '$@.new' '$@'

# Inject compilation instructions
	echo -e '\n\t$(CXX) -c -o $(subst .d,.o,$@) $^ $(ALL_CPPFLAGS) $(ALL_CXXFLAGS)' >> $@

$(COMPDIR)/%: $(SRCDIR)/%
	@ mkdir -p $(dir $@)
	@ ln -sf $(realpath $<) $@

# Do not load object rules if not required by the current targets
# also silence the implicit rule for dep file creation
noObjDepTargets := install% uninstall %clean dist check

ifneq (,$(dep))
  ifeq (,$(filter $(noObjDepTargets),$(MAKECMDGOALS)))
    .SILENT: $(dep)
    include $(dep)
  endif
endif

undefine noObjDepTargets
