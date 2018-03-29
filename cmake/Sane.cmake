FIND_PATH(SANE_INCLUDE_DIR sane/sane.h)
FIND_LIBRARY(SANE_LIBRARY NAMES  sane libsane
   PATHS
   /usr/lib/sane
   /usr/local/lib/sane
)
target_include_directories(libinsane SYSTEM PUBLIC ${SANE_INCLUDE_DIR})
target_link_libraries(libinsane ${SANE_LIBRARY})
