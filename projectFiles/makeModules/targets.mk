.PHONY: run
run:
	$(EXEPATH)

.PHONY: inspect
inspect:
	@ echo "Starting inspector mode"
	@ $(if $(INSPECT),$(foreach var,$(INSPECT),echo -e "$(var) = $($(var))" ;),echo "Need to set INSPECT first")
