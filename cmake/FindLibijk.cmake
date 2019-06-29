# find Libijk
# Usage: 
# LIBIJK_INCLUDE_DIRS, where to find libijk headers
# LIBIJK_LIBRARIES, libijk libraries
# Libijk_FOUND, If false, do not try to use libijk

set(LIBIJK_ROOT CACHE PATH "Root directory of libijk installation")
set(LIBIJK_EXTRA_PREFIXES ${LIBIJK_ROOT} /usr /usr/local /opt/local "$ENV{HOME}")
foreach(prefix ${LIBIJK_EXTRA_PREFIXES})
  list(APPEND Libijk_INCLUDE_PATHS "${prefix}/include")
  list(APPEND Libijk_LIBRARIES_PATHS "${prefix}/lib")
  list(APPEND Libijk_LIBRARIES_PATHS "${prefix}/lib64")
endforeach()

find_path(LIBIJK_INCLUDE_DIRS ijk/ijk.h PATHS ${Libijk_INCLUDE_PATHS})

find_library(LIBIJK_LIBRARIES NAMES ijk libijk PATHS ${Libijk_LIBRARIES_PATHS})

if (LIBIJK_LIBRARIES AND LIBIJK_INCLUDE_DIRS)
  set(Libijk_FOUND TRUE)
  set(LIBIJK_LIBRARIES ${LIBIJK_LIBRARIES})
else ()
  set(Libijk_FOUND FALSE)
endif ()

if (Libijk_FOUND)
  if (NOT Libijk_FIND_QUIETLY)
    message(STATUS "Found libijk: ${LIBIJK_LIBRARIES}")
  endif ()
else ()
  if (Libijk_FIND_REQUIRED)
    message(FATAL_ERROR "Could NOT find libijk.")
  endif ()
  message(STATUS "libijk NOT found.")
endif ()

mark_as_advanced(
    LIBIJK_LIBRARIES
    LIBIJK_INCLUDE_DIRS
  )
