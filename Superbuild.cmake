# Arguments that we want our external dependencies to build
# with.  We need to set the install prefix and paths, but others
# are optional as well if we want to control the build of all
# external projects.
set(cmake_common_args
    -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
    -DCMAKE_C_COMPILER:PATH=${CMAKE_C_COMPILER}
    -DCMAKE_C_FLAGS:STRING=${CMAKE_C_FLAGS}
    -DCMAKE_CXX_COMPILER:PATH=${CMAKE_CXX_COMPILER}
    -DCMAKE_CXX_FLAGS:STRING=${CMAKE_CXX_FLAGS}
    -DLIB_SUFFIX:STRING=${LIB_SUFFIX}
    -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_BINARY_DIR}/INSTALL
    -DCMAKE_INSTALL_RPATH:PATH=${CMAKE_BINARY_DIR}/INSTALL/lib${LIB_SUFFIX}
    -DCMAKE_INSTALL_LIBDIR:PATH=lib
    -DCMAKE_INCLUDE_PATH:PATH=${CMAKE_BINARY_DIR}/INSTALL/include
    -DBUILD_TESTS:BOOL=${BUILD_TESTS}
    -DUSE_DOXYGEN:BOOL=${USE_DOXYGEN}
    -DBUILD_STATIC_LIB:BOOL=${BUILD_STATIC_LIB}
    -DBUILD_DEB_PACKAGE:BOOL=${BUILD_DEB_PACKAGE}
    -DUSE_SUPERBUILD:BOOL=OFF
)

add_custom_target(submodule_init
    COMMAND ${GIT_EXECUTABLE} submodule init ${CMAKE_SOURCE_DIR}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

macro(add_external_project MYNAME LOCATION DEPENDS ARGS)
    ExternalProject_Add( ${MYNAME}
        SOURCE_DIR ${CMAKE_SOURCE_DIR}/${LOCATION}
        DOWNLOAD_COMMAND ${GIT_EXECUTABLE} submodule update --checkout ${CMAKE_SOURCE_DIR}/${LOCATION}
        DOWNLOAD_DIR ${CMAKE_SOURCE_DIR}
        CMAKE_ARGS ${cmake_common_args} ${ARGS}
        INSTALL_DIR ${CMAKE_BINARY_DIR}/INSTALL
        DEPENDS ${DEPENDS} submodule_init
    )
endmacro(add_external_project)

#This depends on JsonBox only if we are building tests
if (BUILD_TESTS)
  find_package(JsonBox CONFIG)
  if (NOT JSONBOX_FOUND)
    add_external_project(JsonBox dependencies/JsonBox "" "")
  endif ()
  set (acl_depends JsonBox)
endif (BUILD_TESTS)

ExternalProject_Add(acl
  SOURCE_DIR ${CMAKE_SOURCE_DIR}
  BUILD_ALWAYS 1
  CMAKE_ARGS ${cmake_common_args} 
  INSTALL_DIR ${CMAKE_BINARY_DIR}/INSTALL
  LOG_INSTALL 1
  DEPENDS ${acl_depends}
)

