find_package(GtkDoc 1.25 REQUIRED)

gtk_doc_add_module(
   libinsane
   SOURCE include/libinsane/gobject.h
   XML libinsane.xml
)

# Build doc-libmeep as part of the default target. Without this, you would
# have to explicitly run something like `make doc-libmeep` to build the docs.
# add_custom_target(documentation ALL DEPENDS doc-libmeep)

# Install the docs. (This assumes you're using the GNUInstallDirs CMake module
# to set the CMAKE_INSTALL_DOCDIR variable correctly).
# install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/libmeep/html
#         DESTINATION ${CMAKE_INSTALL_DOCDIR})
