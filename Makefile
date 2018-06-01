PYTHON ?= python3
VERBOSE ?=
DESTDIR ?= /usr/local

SRCS = $(wildcard src/libinsane/*.c)
HEADERS = $(wildcard include/libinsane/*.h)

all: build check test

build: build_c build_py

install: install_py install_c

uninstall: uninstall_py uninstall_c

build_py:

build_libinsane:
	mkdir -p libinsane/build
	(cd libinsane/build ; cmake .. -DCMAKE_INSTALL_PREFIX=${DESTDIR})
	$(MAKE) -C libinsane/build VERBOSE=${VERBOSE}

build_libinsane_gobject: build_libinsane
	mkdir -p libinsane-gobject/build
	(cd libinsane-gobject/build ; cmake .. -DCMAKE_INSTALL_PREFIX=${DESTDIR})
	$(MAKE) -C libinsane-gobject/build VERBOSE=${VERBOSE}

build_c: build_libinsane build_libinsane_gobject

version:

doxygen:
	mkdir -p doc/build
	doxygen doc/doxygen.conf

gtkdoc: build_libinsane_gobject
	$(MAKE) -C libinsane-gobject/build doc
	mv libinsane-gobject/build/libinsane_gobject/html doc/build/gtkdoc

doc: doxygen gtkdoc
	cp doc/index.html doc/build/index.html

check: build_c
	@echo
	@echo "### CHECK libinsane ###"
	(cd libinsane/build ; ! /usr/lib/llvm-4.0/share/clang/run-clang-tidy.py | grep warning 2>&1)
	@echo
	@echo "### CHECK libinsane-gobject ###"
	(cd libinsane-gobject/build ; ! /usr/lib/llvm-4.0/share/clang/run-clang-tidy.py | grep warning 2>&1)

test: build_libinsane
	@echo
	@echo "### TEST libinsane ###"
	(cd libinsane/build ; CTEST_OUTPUT_ON_FAILURE=1 $(MAKE) test)

linux_exe:

windows_exe:

release:
ifeq (${RELEASE}, )
	@echo "You must specify a release version ($(MAKE) release RELEASE=1.2.3)"
else
	@echo "Will release: ${RELEASE}"
	@echo "Checking release is in ChangeLog ..."
	grep ${RELEASE} ChangeLog
	@echo "Checking release is in libinsane-gobject/CMakeLists.txt ..."
	grep ${RELEASE} libinsane-gobject/CMakeLists.txt
	@echo "Releasing ..."
	git tag -a ${RELEASE} -m ${RELEASE}
	git push origin ${RELEASE}
	@echo "All done"
endif

clean:
	rm -rf doc/build
	rm -rf libinsane/build
	rm -rf libinsane-gobject/build
	rm -rf libinsane-gobject/generated
	mkdir -p libinsane-gobject/generated
	touch libinsane-gobject/generated/.notempty

install_py:


install_c: build_c
	$(MAKE) -C libinsane/build VERBOSE=${VERBOSE} install
	$(MAKE) -C libinsane-gobject/build VERBOSE=${VERBOSE} install


uninstall_py:


uninstall_c:


help:
	@echo "make build || make build_c || make build_py"
	@echo "make check"
	@echo "make doc"
	@echo "make help: display this message"
	@echo "make install || make install_py"
	@echo "make release"
	@echo "make test"
	@echo "make uninstall || make uninstall_py"

.PHONY: \
	build \
	build_c \
	build_py \
	check \
	makefile_libinsane \
	doc \
	doxygen \
	gtkdoc \
	linux_exe \
	windows_exe \
	help \
	install \
	install_c \
	install_py \
	release \
	test \
	uninstall \
	uninstall_c \
	version
