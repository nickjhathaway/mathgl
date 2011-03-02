# FindMathGL.cmake - Check for the presence of MathGL
#
# The following variables are set when MathGL is found:
#  HAVE_MATHGL       = Set to true, if all components of MathGL have been found.
#  MATHGL_INCLUDES   = Include path for the header files of MathGL
#  MATHGL_LIBRARIES  = Link these to use MathGL
#  MATHGL_LFLAGS     = Linker flags (optional)

## -----------------------------------------------------------------------------
## Search locations

set (include_locations
    /usr/include
    /usr/local/include)

set (lib_locations
    /usr/local/lib64
    /usr/local/lib
    /usr/lib64
    /usr/lib)

## -----------------------------------------------------------------------------
## Check for the header files

find_path (MATHGL_INCLUDES mgl/mgl.h mgl/mgl_data.h
  PATHS ${include_locations}
  NO_DEFAULT_PATH)

## -----------------------------------------------------------------------------
## Check for the various libraries

set (MATHGL_LIBRARIES "")

## libmgl

find_library (HAVE_LIBMGL mgl
  PATHS ${lib_locations}
  NO_DEFAULT_PATH)
if (HAVE_LIBMGL)
  list (APPEND MATHGL_LIBRARIES ${HAVE_LIBMGL})
endif (HAVE_LIBMGL)

## -----------------------------------------------------------------------------
## Actions taken when all components have been found

if (MATHGL_INCLUDES AND MATHGL_LIBRARIES)
  set (HAVE_MATHGL TRUE)
else (MATHGL_INCLUDES AND MATHGL_LIBRARIES)
  set (HAVE_MATHGL FALSE)
  if (NOT MATHGL_FIND_QUIETLY)
    if (NOT MATHGL_INCLUDES)
      message (STATUS "Unable to find MATHGL header files!")
    endif (NOT MATHGL_INCLUDES)
    if (NOT MATHGL_LIBRARIES)
      message (STATUS "Unable to find MATHGL library files!")
    endif (NOT MATHGL_LIBRARIES)
  endif (NOT MATHGL_FIND_QUIETLY)
endif (MATHGL_INCLUDES AND MATHGL_LIBRARIES)

if (HAVE_MATHGL)
  if (NOT MATHGL_FIND_QUIETLY)
    message (STATUS "Found components for MATHGL")
    message (STATUS "MATHGL_INCLUDES  = ${MATHGL_INCLUDES}")
    message (STATUS "MATHGL_LIBRARIES = ${MATHGL_LIBRARIES}")
  endif (NOT MATHGL_FIND_QUIETLY)
else (HAVE_MATHGL)
  if (MATHGL_FIND_REQUIRED)
    message (FATAL_ERROR "Could not find MATHGL!")
  endif (MATHGL_FIND_REQUIRED)
endif (HAVE_MATHGL)

## -----------------------------------------------------------------------------
## Mark advanced variables

mark_as_advanced (
  MATHGL_INCLUDES
  MATHGL_LIBRARIES
)

# EOF FindMathGL.cmake
