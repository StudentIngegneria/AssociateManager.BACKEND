enabledLibs      += $(v_lib)
lib.obj          += $(lib_obj)
lib.cppflags     += $(lib_cppflags)
lib.ldflags      += $(lib_ldflags)
lib.cleanTargets += $(v_lib_clean)

# Lib target
.PHONY: $(v_lib)
$(v_lib): $(lib_obj)

.PHONY: $(v_lib_clean)
