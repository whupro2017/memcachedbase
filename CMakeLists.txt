cmake_minimum_required(VERSION 3.10)
project(memcachedbase)

include_directories(src)
include_directories(src/utils)
include_directories(src/efficiency)

set(CMAKE_CXX_STANDARD 14)

if (CMAKE_SYSTEM_NAME MATCHES "Darwin")
    enable_testing()
    find_package(gtest REQUIRED)
    include_directories(${GTEST_INCLUDE_DIRS})
    link_directories(/usr/local/lib)
else ()
    include_directories(SYSTEM ${PROJECT_SOURCE_DIR}/third-party/gtest-1.7.0/fused-src)
    include_directories(SYSTEM ${PROJECT_SOURCE_DIR}/third-party/gtest-1.7.0/fused-src/gtest)
    add_subdirectory(third-party/gtest-1.7.0/fused-src/gtest)
    enable_testing()
endif ()

set(PTHREAD -lpthread)

set(LIBMC -lmemcached)

if (CMAKE_SYSTEM_NAME MATCHES "Darwin")
    include_directories(/usr/local/Cellar/libmemcached/1.0.18_2/include/)
    include_directories(/usr/local/Cellar/libmemcached/1.0.18_2/include/libmemcached)
    link_directories(/usr/local/Cellar/libmemcached/1.0.18_2/lib)
else ()
    include_directories(/usr/include)
    include_directories(/usr/include/libmemcached/)
endif ()

set(MC_GTEST
        ./src/unittests/AddGetDeletTest.cpp)

set(MC_GTESTS_EXES ${MC_GTEST})
foreach (sourcefile ${MC_GTESTS_EXES})
    get_filename_component(exename ${sourcefile} NAME_WE)
    add_executable(${CMAKE_PROJECT_NAME}_${exename}${ARTIFACT_SUFFIX} ${sourcefile})
    set_target_properties(${CMAKE_PROJECT_NAME}_${exename}${ARTIFACT_SUFFIX}
            PROPERTIES EXCLUDE_FROM_DEFAULT_BUILD_RELEASE 1
            EXCLUDE_FROM_DEFAULT_BUILD_MINRELEASE 1
            EXCLUDE_FROM_DEFAULT_BUILD_RELWITHDEBINFO 1
            OUTPUT_NAME ${exename}${ARTIFACT_SUFFIX}
            )
    target_link_libraries(${CMAKE_PROJECT_NAME}_${exename}${ARTIFACT_SUFFIX} gtest ${PTHREAD} ${ATOMIC} ${LIBMC})
endforeach (sourcefile ${MC_GTESTS_EXES})

set(MC_TEST
        ./src/efficiency/SimpleTest.cpp
        ./src/efficiency/MultiTest.cpp)

set(MC_TESTS_EXES ${MC_TEST})
foreach (sourcefile ${MC_TESTS_EXES})
    get_filename_component(exename ${sourcefile} NAME_WE)
    add_executable(${CMAKE_PROJECT_NAME}_${exename}${ARTIFACT_SUFFIX} ${sourcefile})
    set_target_properties(${CMAKE_PROJECT_NAME}_${exename}${ARTIFACT_SUFFIX}
            PROPERTIES EXCLUDE_FROM_DEFAULT_BUILD_RELEASE 1
            EXCLUDE_FROM_DEFAULT_BUILD_MINRELEASE 1
            EXCLUDE_FROM_DEFAULT_BUILD_RELWITHDEBINFO 1
            OUTPUT_NAME ${exename}${ARTIFACT_SUFFIX}
            )
    target_link_libraries(${CMAKE_PROJECT_NAME}_${exename}${ARTIFACT_SUFFIX} ${PTHREAD} ${ATOMIC} ${LIBMC})
endforeach (sourcefile ${MC_TESTS_EXES})