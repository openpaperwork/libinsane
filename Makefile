PYTHON ?= python3

build: build_c build_py

install: install_py install_c

uninstall: uninstall_py

cmake:
	mkdir -p build
	(cd build ; cmake ..)

build_py:

build_c: cmake
	(cd build ; make)

version:

doxygen:
	mkdir -p doc/build
	doxygen doc/doxygen.conf

sphinx:
	(cd doc && DISPLAY="" make html)

gtkdoc: cmake
	# Does not work yet
	# (cd build ; make doc-libinsane)

doc: doxygen sphinx gtkdoc
	cp doc/index.html doc/build/index.html

check:

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
	rm -rf build

install_py:

install_c:

uninstall_py:

uninstall_c:

help:
	@echo "make build || make build_py"
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
	cmake \
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
	sphinx \
	test \
	uninstall \
	uninstall_c \
	version
