FEATURETOOL = opp_featuretool
FEATURES_H = src/inet/features.h

.PHONY: all clean cleanall makefiles makefiles-so makefiles-lib makefiles-exe checkmakefiles doxy doc submodule-init

all: checkmakefiles $(FEATURES_H)
	cd src && $(MAKE)
#	cd tutorials/queueing && $(MAKE)

clean: checkmakefiles
	cd src && $(MAKE) clean
	cd tutorials/queueing && $(MAKE) clean

cleanall: checkmakefiles
	@cd src && $(MAKE) MODE=release clean
	@cd src && $(MAKE) MODE=debug clean
	@rm -f src/Makefile $(FEATURES_H)
	@cd tutorials/queueing && $(MAKE) MODE=release clean
	@cd tutorials/queueing && $(MAKE) MODE=debug clean

MAKEMAKE_OPTIONS := -f --deep -o INET -O out -pINET -I.

makefiles: makefiles-so

makefiles-so: $(FEATURES_H)
	@FEATURE_OPTIONS=$$($(FEATURETOOL) options -f -l) && cd src && opp_makemake --make-so $(MAKEMAKE_OPTIONS) $$FEATURE_OPTIONS
	cd tutorials/queueing && opp_makemake -s -I $(INET_ROOT)/src -o inetqueueing -f

makefiles-lib: $(FEATURES_H)
	@FEATURE_OPTIONS=$$($(FEATURETOOL) options -f -l) && cd src && opp_makemake --make-lib $(MAKEMAKE_OPTIONS) $$FEATURE_OPTIONS
	cd tutorials/queueing && opp_makemake -s -I $(INET_ROOT)/src -o inetqueueing -f

makefiles-exe: $(FEATURES_H)
	@FEATURE_OPTIONS=$$($(FEATURETOOL) options -f -l) && cd src && opp_makemake $(MAKEMAKE_OPTIONS) $$FEATURE_OPTIONS
	cd tutorials/queueing && opp_makemake -s -I $(INET_ROOT)/src -o inetqueueing -f

checkmakefiles: submodule-init
	@if [ ! -f src/Makefile ]; then \
	echo; \
	echo '========================================================================'; \
	echo 'src/Makefile does not exist. Please use "make makefiles" to generate it!'; \
	echo '========================================================================'; \
	echo; \
	exit 1; \
	fi

submodule-init:
	@if [ -d .git ]; then \
	if [ ! -f tutorials/package.ned ]; then \
	echo 'Fetching git submodules (tutorials, showcases)...'; \
	git submodule update --init; \
	fi \
	fi

# generate an include file that contains all the WITH_FEATURE macros according to the current enablement of features
$(FEATURES_H): $(wildcard .oppfeaturestate) .oppfeatures
	@$(FEATURETOOL) defines >$(FEATURES_H)

doc:
	cd doc/src && $(MAKE)
	doxygen doxy.cfg

ddoc:
	cd doc/src && ./docker-make html && echo "===> file:$$(pwd)/_build/html/index.html"


