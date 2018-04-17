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

build_linux_c: libinsane/configure
	(cd libinsane && ./configure --enable-fatal-warnings --enable-debug)
	make -C libinsane

build_windows_c: libinsane/configure
	(cd libinsane && ./configure --enable-fatal-warnings --enable-debug --host=x86_64-w64-mingw32)
	make -C libinsane

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
	rm -rf doc/build
	if [ -e libinsane/Makefile ]; then make -C libinsane clean ; fi

install_py:


install_c: build_linux_c
	make -C libinsane install

install: install_c

uninstall_py:

uninstall_c:
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
