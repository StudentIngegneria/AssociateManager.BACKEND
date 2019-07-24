.PHONY: all
all: $(EXEPATH)

.PHONY: install
install: installdirs
	@ echo "FIXME: '$@' target not implemented" >&2
	@ exit 1

.PHONY: uninstall
uninstall:
	@ echo "FIXME: '$@' target not implemented" >&2
	@ exit 1

.PHONY: install-strip
install-strip:
	@ echo "FIXME: '$@' target not implemented" >&2
	@ exit 1

.PHONY: clean
clean: mostlyclean $(lib.cleanTargets)

.PHONY: distclean
distclean: mostlyclean
	rm -f compile_commands.json
	rm -rf lib/*

.PHONY: mostlyclean
mostlyclean:
	rm -rf $(BUILDDIR)

.PHONY: maintainer-clean
maintainer-clean: distclean

.PHONY: dist
dist:
	@ echo "FIXME: '$@' target not implemented" >&2
	@ exit 1

.PHONY: check
check:
	@ echo "FIXME: '$@' target not implemented" >&2
	@ exit 1

.PHONY: installcheck
installcheck:
	@ echo "FIXME: '$@' target not implemented" >&2
	@ exit 1

.PHONY: installdirs
installdirs:
	@ echo "FIXME: '$@' target not implemented" >&2
	@ exit 1
