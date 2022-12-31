cmake_minimum_required (VERSION 3.12)
if (MSVC)
set (CMAKE_SYSTEM_VERSION 10.0.22000.0)
endif (MSVC)
project (LowLevel VERSION 3.18.1.7 DESCRIPTION "Low Level Structures Library")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

SET(BOOST_INCLUDE_DIR "error" CACHE STRING "The path to the boost libraries includes")
SET(GTEST_INCLUDE_DIR "error" CACHE STRING "The path to the Google Test includes")
SET(GTEST_LIBRARY_DIR "error" CACHE STRING "The path to the Google Test libraries")

find_library(FOUND_GTEST_LIBRARY_PATH NAMES gtest PATHS ${GTEST_LIBRARY_DIR})
find_library(FOUND_GTEST_MAIN_LIBRARY_PATH NAMES gtest_main PATHS ${GTEST_LIBRARY_DIR})

enable_testing()

if (MSVC)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD -D_AFXDLL")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd /W4 -D_AFXDLL")
else ()
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_NO_MFC")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
endif (MSVC)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_DEBUG -DDEBUG")

add_library(LowLevel STATIC
    cpp/AfxIniSettings.cpp
    cpp/colors.c
    cpp/convert.cpp
    cpp/cpoints.cpp
    cpp/Exprtopt.cpp
    cpp/hresult.cpp
    cpp/htree.cpp
    cpp/linklist.cpp
    cpp/linklist.noclang.cpp
    cpp/misc.c
    cpp/objectcache.cpp
    cpp/str_printf.cpp
    cpp/tstring.cpp
    cpp/validation_object.cpp
    cpp/valuecache.cpp
    cpp/vvector.cpp
)

add_executable(LowLevelTest
    test/gtest.cpp
    test/LowlevelTest.cpp
)

target_include_directories(LowLevel PUBLIC
    ${BOOST_INCLUDE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party
)

target_include_directories(LowLevelTest PUBLIC
    ${BOOST_INCLUDE_DIR}
    ${GTEST_INCLUDE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party
)

set_target_properties(LowLevel PROPERTIES VERSION ${CMAKE_PROJECT_VERSION})
set_target_properties(LowLevel PROPERTIES SOVERSION 3)
set_target_properties(LowLevel PROPERTIES DEFINE_SYMBOL "")

target_link_libraries(LowLevelTest ${FOUND_GTEST_LIBRARY_PATH} ${FOUND_GTEST_MAIN_LIBRARY_PATH} LowLevel)
if (MSVC)
else ()
target_link_libraries(LowLevelTest pthread)
endif (MSVC)

add_test(LowLevelTests LowLevelTest)

set_target_properties(LowLevel PROPERTIES
    PUBLIC_HEADER include/hssconfig/config.h
    PUBLIC_HEADER include/AfxIniSettings.h
    PUBLIC_HEADER include/boost_bimap.h
    PUBLIC_HEADER include/boost_compression.h
    PUBLIC_HEADER include/boost_ll_config.h
    PUBLIC_HEADER include/colors.h
    PUBLIC_HEADER include/cpoints.h
    PUBLIC_HEADER include/COMInit.h
    PUBLIC_HEADER include/ConversionFactors.h
    PUBLIC_HEADER include/convert.h
    PUBLIC_HEADER include/debug-trap.h
    PUBLIC_HEADER include/Exprtopt.h
    PUBLIC_HEADER include/filesystem.hpp
    PUBLIC_HEADER include/guid.h
    PUBLIC_HEADER include/hresult.h
    PUBLIC_HEADER include/hss_inlines.h
    PUBLIC_HEADER include/hss_propagate_const.h
    PUBLIC_HEADER include/htree.h
    PUBLIC_HEADER include/insert_ordered_map.h
    PUBLIC_HEADER include/intel_check.h
    PUBLIC_HEADER include/intrusive_ptr.h
    PUBLIC_HEADER include/linklist.h
    PUBLIC_HEADER include/misc.h
    PUBLIC_HEADER include/objectcache.h
    PUBLIC_HEADER include/objectcache.inl
    PUBLIC_HEADER include/out_helper.h
    PUBLIC_HEADER include/propagate_const.h
    PUBLIC_HEADER include/stdchar.h
    PUBLIC_HEADER include/str_printf.h
    PUBLIC_HEADER include/tstring.h
    PUBLIC_HEADER include/types.h
    PUBLIC_HEADER include/validation_ids.h
    PUBLIC_HEADER include/validation_object.h
    PUBLIC_HEADER include/valuecache.h
    PUBLIC_HEADER include/vvector.h
    PUBLIC_HEADER include/WinReplacement.h
)

configure_file(LowLevel.pc.in LowLevel.pc @ONLY)

message(STATUS "Windows SDK Version ${CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION}")

install(FILES ${CMAKE_BINARY_DIR}/LowLevel.pc DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)
install(TARGETS LowLevel DESTINATION /usr/lib)

if (PROMETHEUS_OUTPUT_DIRECTORY)
if (MSVC)
add_custom_command(TARGET LowLevel POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_CURRENT_BINARY_DIR}/LowLevel.lib
            ${PROMETHEUS_OUTPUT_DIRECTORY}/${CMAKE_BUILD_TYPE}
)
else ()
add_custom_command(TARGET LowLevel POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_CURRENT_BINARY_DIR}/libLowLevel.a
            ${PROMETHEUS_OUTPUT_DIRECTORY}/${CMAKE_BUILD_TYPE}
)
endif ()
endif ()
