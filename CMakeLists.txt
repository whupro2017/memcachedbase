cmake_minimum_required(VERSION 3.10)
project(memcachedbase)
set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")

include_directories(src)
include_directories(src/utils)

set(CMAKE_CXX_STANDARD 14)

#set(LIBMEMC true)

if (CMAKE_SYSTEM_NAME MATCHES "Darwin")
    enable_testing()
    find_package(gtest REQUIRED)
    include_directories(${GTEST_INCLUDE_DIRS})
    link_directories(/usr/local/lib)
else ()
    include_directories(SYSTEM ${PROJECT_SOURCE_DIR}/third-party/gtest-1.7.0/fused-src)
    include_directories(SYSTEM ${PROJECT_SOURCE_DIR}/third-party/gtest-1.7.0/fused-src/gtest)
    if (LIBMEMC)
        include_directories(/usr/include/libmemcached)
    endif ()
    add_subdirectory(third-party/gtest-1.7.0/fused-src/gtest)
    enable_testing()
endif ()

set(PTHREAD -lpthread)

if (LIBMEMC)
    set(LIBMC -lmemcached)
    if (CMAKE_SYSTEM_NAME MATCHES "Darwin")
        include_directories(/usr/local/Cellar/libmemcached/1.0.18_2/include/)
        include_directories(/usr/local/Cellar/libmemcached/1.0.18_2/include/libmemcached)
        link_directories(/usr/local/Cellar/libmemcached/1.0.18_2/lib)
        include_directories(/usr/local/include)
        include_directories(/usr/local/include/libmemcached/)
    else ()
        include_directories(/usr/include)
        include_directories(/usr/include/libmemcached/)
    endif ()

    set(MC_GTEST
            ./test/unittests/AddGetDeletTest.cpp)

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
        target_link_libraries(${CMAKE_PROJECT_NAME}_${exename}${ARTIFACT_SUFFIX} gtest ${ATOMIC} ${LIBMC} ${PTHREAD})
    endforeach (sourcefile ${MC_GTESTS_EXES})

    set(MC_TEST
            ./test/efficiency/SimpleTest.cpp
            ./test/efficiency/MultiTest.cpp
            ./test/efficiency/CASSingleTest.cpp)

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
        target_link_libraries(${CMAKE_PROJECT_NAME}_${exename}${ARTIFACT_SUFFIX} ${ATOMIC} ${LIBMC} ${PTHREAD})
    endforeach (sourcefile ${MC_TESTS_EXES})
else ()
    set(STANDALONE 0)
    include_directories(src/memcached)
    include_directories(/usr/include/sasl)
    #include_directories(/usr/include/event2)

    set(LIBSASL -lsasl2)
    set(LIBEVENT -levent)

    set(CMAKE_CXX_FLAGS "-DHAVE_CONFIG_H ${CMAKE_CXX_FLAGS}")
    set(CMAKE_C_FLAGS "-DHAVE_CONFIG_H ${CMAKE_C_FLAGS}")

    set(MEMC_SRC
            src/memcached/assoc.c
            src/memcached/authfile.c
            src/memcached/bipbuffer.c
            src/memcached/cache.c
            src/memcached/crawler.c
            src/memcached/daemon.c
            src/memcached/hash.c
            src/memcached/items.c
            src/memcached/itoa_ljust.c
            src/memcached/jenkins_hash.c
            src/memcached/logger.c
            src/memcached/murmur3_hash.c
            src/memcached/restart.c
            #src/memcached/sasl_defs.c
            src/memcached/slabs.c
            src/memcached/stats.c
            src/memcached/slab_automove.c
            src/memcached/thread.c
            #src/memcached/slab_automove_extstore.c
            src/memcached/util.c
            )

    set(MEMC_MAIN
            src/memcached/memcached.c)

    if (STANDALONE)
        set(CMAKE_CXX_FLAGS "-DSTANDALONE ${CMAKE_CXX_FLAGS}")
        set(CMAKE_C_FLAGS "-DSTANDALONE ${CMAKE_C_FLAGS}")
        set(MEMC_TEST_EXES ${MEMC_MAIN})
    else ()
        set(MEMC_SRC ${MEMC_SRC}
                ${MEMC_MAIN})
        message(${MEMC_SRC})
        set(MEMC_TEST
                test/unittests/MemcachedTest.cpp)
        set(MEMC_TEST_EXES ${MEMC_TEST})
    endif ()

    foreach (sourcefile ${MEMC_TEST_EXES})
        get_filename_component(exename ${sourcefile} NAME_WE)
        add_executable(${CMAKE_PROJECT_NAME}_${exename}${ARTIFACT_SUFFIX} ${sourcefile} ${MEMC_SRC})
        set_target_properties(${CMAKE_PROJECT_NAME}_${exename}${ARTIFACT_SUFFIX}
                PROPERTIES EXCLUDE_FROM_DEFAULT_BUILD_RELEASE 1
                EXCLUDE_FROM_DEFAULT_BUILD_MINRELEASE 1
                EXCLUDE_FROM_DEFAULT_BUILD_RELWITHDEBINFO 1
                OUTPUT_NAME ${exename}${ARTIFACT_SUFFIX}
                )
        target_link_libraries(${CMAKE_PROJECT_NAME}_${exename}${ARTIFACT_SUFFIX} ${ATOMIC} ${PTHREAD} ${LIBEVENT} ${LIBSASL})
    endforeach (sourcefile ${MEMC_TEST_EXES})
endif ()