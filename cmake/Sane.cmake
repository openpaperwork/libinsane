FIND_PATH(SANE_INCLUDE_DIR sane/sane.h)
FIND_LIBRARY(SANE_LIBRARY NAMES  sane libsane
   PATHS
   /usr/lib/sane
   /usr/local/lib/sane
)
