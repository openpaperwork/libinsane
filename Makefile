PYTHON ?= python3
VERBOSE ?=
DESTDIR ?=

SRCS = $(wildcard src/libinsane/*.c)
HEADERS = $(wildcard include/libinsane/*.h)

build: build_c build_py

install: install_py install_c

uninstall: uninstall_py

build_py:

libinsane/configure:
	(cd libinsane && ./autogen.sh)

libinsane-gobject/configure:
	(cd libinsane-gobject && ./autogen.sh)

build_linux_c: libinsane/configure libinsane-gobject/configure
	(cd libinsane && ./configure --enable-fatal-warnings --enable-debug)
	make -j4 -C libinsane
	(cd libinsane-gobject && ./configure --enable-fatal-warnings --enable-debug)
	make -j4 -C libinsane-gobject

build_windows_c: libinsane/configure libinsane-gobject/configure
	(cd libinsane && ./configure --enable-fatal-warnings --enable-debug --host=x86_64-w64-mingw32)
	make -j4 -C libinsane
# Cannot cross-compile libinsane-gobject. gobject-introspection-1.0.pc and g-ir-scanner
# are not available using MXE. It has to be compiled with Msys2
# 	(cd libinsane-gobject && PKG_CONFIG_PATH=/usr/lib/mxe/usr/x86_64-w64-mingw32.shared/lib/pkgconfig ./configure --enable-fatal-warnings --enable-debug --host=x86_64-w64-mingw32)
#	make -j4 -C libinsane-gobject

build_c: build_linux_c

version:

doxygen:
	mkdir -p doc/build
	doxygen doc/doxygen.conf

gtkdoc:
	# Does not work yet

doc: doxygen gtkdoc
	cp doc/index.html doc/build/index.html

%.c_check:
	splint -f splint.rc $(patsubst %_check,%,$@)

%.h_check:
	splint -f splint.rc $(patsubst %_check,%,$@)

check: \
	$(patsubst %.h,%.h_check,${HEADERS}) \
	$(patsubst %.c,%.c_check,${SRCS})

test:

linux_exe:

windows_exe:

release:
ifeq (${RELEASE}, )
	@echo "You must specify a release version (make release RELEASE=1.2.3)"
else
	@echo "Will release: ${RELEASE}"
	@echo "Checking release is in ChangeLog ..."
	grep ${RELEASE} ChangeLog
	@echo "Releasing ..."
	git tag -a ${RELEASE} -m ${RELEASE}
	git push origin ${RELEASE}
	@echo "All done"
endif

clean:
	if [ -e libinsane/Makefile ]; then make -C libinsane clean ; fi
	if [ -e libinsane-gobject/Makefile ]; then make -C libinsane-gobject clean ; fi
	rm -rf doc/build
	rm -f libinsane/configure libinsane-gobject/configure
	rm -rf libinsane/build libinsane-gobject/build
	rm -rf libinsane/out libinsane-gobject/out

install_py:


install_c: build_linux_c
	make -C libinsane install
	make -C libinsane-gobject install

install: install_c

uninstall_py:

uninstall_c:
	make -C libinsane-gobject uninstall
	make -C libinsane uninstall

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
