//
// Created by Michael on 11/7/19.
//

#include <iostream>
#include <cstring>
#include <memcached.h>
#include "tracer.h"

constexpr size_t total_count = 1000;
constexpr size_t buff_length = 256;

int main(int argc, char **argv) {
    memcached_server_st *servers = memcached_servers_parse("localhost");
    memcached_st *memc = memcached_create(NULL);
    memcached_server_push(memc, servers);
    //memcached_server_list_free(servers);
    memcached_behavior_set(memc, MEMCACHED_BEHAVIOR_SUPPORT_CAS, (uint64_t) 1);
    char key[buff_length];
    char value[buff_length];
    char retvl[buff_length];
    size_t success = 0;
    Tracer tracer;
    tracer.startTime();
    for (int i = 0; i < total_count; i++) {
        std::memset(key, 0, buff_length);
        std::memset(value, 0, buff_length);
        std::sprintf(key, "%d", i);
        std::memcpy(value, "hello", 5);
        std::sprintf(value + 5, "%d", i);
        memcached_return_t ret = memcached_set(memc, key, std::strlen(key), value, std::strlen(value), 0, 0);
        if (ret == memcached_return_t::MEMCACHED_SUCCESS) success++;
    }
    cout << tracer.getRunTime() << "\t" << success << " " << total_count << endl;
    success = 0;
    /*memcached_result_st results_obj;
    memcached_result_st *results = memcached_result_create(memc, &results_obj);
    memcached_return rc;*/
    tracer.startTime();
    for (int i = 0; i < total_count; i++) {
        std::memset(key, 0, buff_length);
        std::memset(value, 0, buff_length);
        std::sprintf(key, "%d", i);
        std::memcpy(value, "hello", 5);
        std::sprintf(value + 5, "%d", total_count - i);
        /*rc = memcached_flush(memc, 0);
        results = memcached_fetch_result(memc, &results_obj, &rc);*/
        memcached_return_t ret = memcached_cas(memc, key, std::strlen(key), value, std::strlen(value), 0, 0,
                /*results->item_cas*/0);
        if (ret == memcached_return_t::MEMCACHED_SUCCESS) success++;
    }
    cout << tracer.getRunTime() << "\t" << success << " " << total_count;
    memcached_free(memc);
    return 0;
}