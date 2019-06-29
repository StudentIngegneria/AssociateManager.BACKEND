$(v_lib_cflags) += $(lib_dflags) $(lib_lflags) $(lib_iflags)
lib.cflags += $(lib_cflags)
lib.obj += $(lib_obj)
enabledLibs += $(v_lib)

# Lib target
.PHONY: $(v_lib)
$(v_lib): $(lib_obj)
