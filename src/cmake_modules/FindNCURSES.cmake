MESSAGE(STATUS "looking for  ncurses")
MESSAGE(STATUS "----------------------------------------------------------------------------------------------------")
find_package(PkgConfig REQUIRED)
pkg_search_module(PC_NCURSES ncurses)

find_path(NCURSES_INCLUDE_DIR ncurses.h
          HINTS ${PC_NCURSES_INCLUDEDIR} ${PC_NCURSES_INCLUDE_DIRS})

find_library(NCURSES_LIBRARY NAMES curses
             HINTS ${PC_NCURSES_LIBDIR} ${PC_NCURSES_LIBRARY_DIRS} )

IF (NCURSES_LIBRARY)
   MESSAGE(STATUS "Found ncurses at ${NCURSES_LIBRARY}")
   SET(NCURSES_FOUND TRUE)
ENDIF (NCURSES_LIBRARY)

IF (NCURSES_FOUND)
   IF (NOT NCURSES_FIND_QUIETLY)
      MESSAGE(STATUS "Found ncurses: ${NCURSES_LIBRARY}")
   ENDIF (NOT NCURSES_FIND_QUIETLY)
ELSE (NCURSES_FOUND)
   IF (NCURSES_FIND_REQUIRED)
      MESSAGE(FATAL_ERROR "Could not find ncurses")
   ENDIF (NCURSES_FIND_REQUIRED)
ENDIF (NCURSES_FOUND)

MESSAGE(STATUS " finish looking for  ncurses")
MESSAGE(STATUS "----------------------------------------------------------------------------------------------------")
