//
// Created by Michael on 10/27/19.
//

#include <iostream>
#include <cstring>
#include <memcached.h>
#include "tracer.h"

int main(int argc, char **argv) {
    memcached_server_st *servers = memcached_servers_parse("localhost");
    memcached_st * memc = memcached_create(NULL);
    memcached_server_push(memc, servers);
    memcached_server_list_free(servers);
    memcached_behavior_set(memc, MEMCACHED_BEHAVIOR_BINARY_PROTOCOL, (uint64_t) 0);
    char key[1024];
    char value[1024];
    std::memset(key, 0, 1024);
    std::memset(value, 0, 1024);
    std::memcpy(value, "hello", 5);
    Tracer tracer;
    tracer.startTime();
    for (int i = 0; i < 10000000; i++) {
        std::sprintf(key, "%d", i);
        std::sprintf(value + 5, "%d", i);
        memcached_add(memc, key, sizeof(key), value, sizeof(value), 0, 0);
    }
    cout << tracer.getRunTime();
    memcached_free(memc);
    return 0;
}