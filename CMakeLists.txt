# -*- CMake -*-
# ------------------------------------ #
# CMake library                        #
# Project libbpcrypt                   #
# Author: Alexei Panov <me@elemc.name> #
# ------------------------------------ #

cmake_minimum_required (VERSION 2.8.0)

enable_testing()

if ( NOT CMAKE_BUILD_TYPE )
    set( CMAKE_BUILD_TYPE Debug )
endif ( NOT CMAKE_BUILD_TYPE )
if (NOT DEBUG)
    if ( CMAKE_BUILD_TYPE STREQUAL Debug )
        set (DEBUG 1)
    else ( CMAKE_BUILD_TYPE STREQUAL Debug )
        set (DEBUG 0)
    endif ( CMAKE_BUILD_TYPE STREQUAL Debug )
endif (NOT DEBUG)

if ( NOT APPLE )
    set( LIB_INSTALL_DIR lib${LIB_SUFFIX} )
    set( INCLUDE_INSTALL_DIR include )
    set( CMAKE_SKIP_BUILD_RPATH ON )
else ( NOT APPLE )
    if ( NOT CMAKE_MACOSX_RPATH )
        set( CMAKE_MACOSX_RPATH 0 )
    endif ( NOT CMAKE_MACOSX_RPATH )
endif ( NOT APPLE )

set (PROJECT_NAME bpcrypt)

set(LIBBPCRYPT_VERSION_MAJOR 0)
set(LIBBPCRYPT_VERSION_MINOR 1)
set(LIBBPCRYPT_VERSION_PATCH 0)
set(LIBBPCRYPT_VERSION_STRING ${LIBBPCRYPT_VERSION_MAJOR}.${LIBBPCRYPT_VERSION_MINOR}.${LIBBPCRYPT_VERSION_PATCH})
 
set (HEADERS
    crypt.h
    misc.h
    tree.h
)
set (SOURCES
    crypt.c
    misc.c
    tree.c
)

if ( WITH_OLD_CRYPT )
    set ( HEADERS ${HEADERS} old_crypt.h )
    set ( SOURCES ${SOURCES} old_crypt.c )
endif ( WITH_OLD_CRYPT )
if ( WITH_XML )
    set ( HEADERS ${HEADERS} xml.h )
    set ( SOURCES ${SOURCES} xml.c )
endif ( WITH_XML )


project (${PROJECT_NAME})

set (DEP_LIB_INCLUDES           "")
set (BUILD_PROJECT_LIBRARIES    "")

find_package (OpenSSL REQUIRED)
set (BUILD_PROJECT_LIBRARIES ${BUILD_PROJECT_LIBRARIES} ${OPENSSL_LIBRARIES})
set (DEP_LIB_INCLUDES ${DEP_LIB_INCLUDES} ${OPENSSL_INCLUDE_DIR})

if ( WITH_XML )
    find_package (LibXml2 REQUIRED)
    set (BUILD_PROJECT_LIBRARIES ${BUILD_PROJECT_LIBRARIES} ${LIBXML2_LIBRARIES})
    set (DEP_LIB_INCLUDES ${DEP_LIB_INCLUDES} ${LIBXML2_INCLUDE_DIR})
endif( WITH_XML )


configure_file (
    "${PROJECT_SOURCE_DIR}/config.h.in"
    "${PROJECT_BINARY_DIR}/config.h"
)

if ( WITH_OLD_CRYPT )
    message( STATUS "Test CXML file coping" )
    configure_file( "${PROJECT_SOURCE_DIR}/test.cxml"
                    "${PROJECT_BINARY_DIR}/test.cxml"
                    COPYONLY
    )
endif ( WITH_OLD_CRYPT )

include_directories (${CMAKE_CURRENT_BINARY_DIR} ${DEP_LIB_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR})

source_group ("Header Files" FILES ${HEADERS})
source_group ("Source Files" FILES ${SOURCES})

# Library
add_library( ${PROJECT_NAME} ${SOURCES} ${HEADERS} )
target_link_libraries (${PROJECT_NAME} ${BUILD_PROJECT_LIBRARIES})
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${LIBBPCRYPT_VERSION_STRING} SOVERSION ${LIBBPCRYPT_VERSION_MAJOR})
if ( APPLE )
    get_target_property(GENERIC_COMPILE_FLAGS ${PROJECT_NAME} COMPILE_FLAGS)
    if( GENERIC_COMPILE_FLAGS STREQUAL "GENERIC_COMPILE_FLAGS-NOTFOUND" )
        set( GENERIC_COMPILE_FLAGS "" )
    endif( GENERIC_COMPILE_FLAGS STREQUAL "GENERIC_COMPILE_FLAGS-NOTFOUND" )
    set( NEW_COMPILE_FLAGS "${GENERIC_COMPILE_FLAGS} -Wno-deprecated-declarations" )
    set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS ${NEW_COMPILE_FLAGS})
endif ( APPLE )

# Tests section

add_executable(test-crypt test-crypt.c)
add_executable(test-misc test-misc.c)
add_executable(test-tree test-tree.c)
target_link_libraries( test-crypt ${PROJECT_NAME} )
target_link_libraries( test-misc ${PROJECT_NAME} )
target_link_libraries( test-tree ${PROJECT_NAME} )

add_test( NAME "Crypt" COMMAND test-crypt Makefile Makefile.1 )
add_test( NAME "Misc" COMMAND test-misc )
add_test( NAME "Tree" COMMAND test-tree )

set_tests_properties( "Crypt" PROPERTIES ENVIRONMENT "LD_LIBRARY_PATH=${CMAKE_CURRENT_BINARY_DIR}" )
set_tests_properties( "Misc" PROPERTIES ENVIRONMENT "LD_LIBRARY_PATH=${CMAKE_CURRENT_BINARY_DIR}" )
set_tests_properties( "Tree" PROPERTIES ENVIRONMENT "LD_LIBRARY_PATH=${CMAKE_CURRENT_BINARY_DIR}" )

if ( WITH_OLD_CRYPT )
    add_executable(test-old-crypt test-old-crypt.c)
    target_link_libraries( test-old-crypt ${PROJECT_NAME} )
    add_test( NAME "Old-Crypt" COMMAND test-old-crypt )
    set_tests_properties( "Old-Crypt" PROPERTIES ENVIRONMENT "LD_LIBRARY_PATH=${CMAKE_CURRENT_BINARY_DIR}" )
endif( WITH_OLD_CRYPT )
if ( WITH_XML )
    add_executable(test-xml test-xml.c)
    target_link_libraries( test-xml ${PROJECT_NAME} )
    add_test( NAME "XML" COMMAND test-xml ~/.basketpwd/default.cxml )
    set_tests_properties( "XML" PROPERTIES ENVIRONMENT "LD_LIBRARY_PATH=${CMAKE_CURRENT_BINARY_DIR}" )
endif( WITH_XML )


# Install section
if ( NOT APPLE )
    install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}
            LIBRARY DESTINATION ${LIB_INSTALL_DIR}
            ARCHIVE DESTINATION ${LIB_INSTALL_DIR}/static)
    install(FILES ${HEADERS} DESTINATION ${INCLUDE_INSTALL_DIR}/${PROJECT_NAME})
endif ( NOT APPLE )

