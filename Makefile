PYTHON ?= python3
VERBOSE ?=
DESTDIR ?=

SRCS = $(wildcard src/libinsane/*.c)
HEADERS = $(wildcard include/libinsane/*.h)

build: build_c build_py

install: install_py install_c

uninstall: uninstall_py

cmake_linux:
	mkdir -p build_linux
	(cd build_linux ; OS=linux cmake ..)

cmake_windows:
	mkdir -p build_windows
	(cd build_windows ; OS=mingw cmake ..)

build_py:

build_linux_c: cmake_linux
	(cd build_linux ; make VERBOSE=${VERBOSE} DESTDIR=${DESTDIR})

build_windows_c: cmake_windows
	(cd build_windows ; make VERBOSE=${VERBOSE} DESTDIR=${DESTDIR})

build_c: build_linux_c build_windows_c

version:

doxygen:
	mkdir -p doc/build
	doxygen doc/doxygen.conf

gtkdoc: cmake_linux
	# Does not work yet
	# (cd build_linux ; make doc-libinsane)

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
	rm -rf build_linux
	rm -rf build_windows

install_py:

ifeq ($(OS),Windows_NT)

install_linux_c: build_linux_c
	(cd build_linux ; make VERBOSE=${VERBOSE} DESTDIR=${DESTDIR} install)

install_c: install_linux_c

else

install_windows_c: build_windows_c
	(cd build_windows ; make VERBOSE=${VERBOSE} DESTDIR=${DESTDIR} install)

install_c: install_windows_c

endif

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
	build_linux_c \
	build_windows_c \
	build_py \
	check \
	cmake_linux \
	doc \
	doxygen \
	gtkdoc \
	linux_exe \
	windows_exe \
	help \
	install \
	install_c \
	install_linux_c \
	install_py \
	release \
	test \
	uninstall \
	uninstall_c \
	version
