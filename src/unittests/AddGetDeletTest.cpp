//
// Created by Michael on 10/27/19.
//

#include <gtest/gtest.h>
#include <cstring>
#include <memcached.h>

memcached_server_st *servers = memcached_servers_parse("localhost");
memcached_st *memc = memcached_create(NULL);

void SetUp() {
    memcached_server_push(memc, servers);
    memcached_server_list_free(servers);
    memcached_behavior_set(memc, MEMCACHED_BEHAVIOR_BINARY_PROTOCOL, (uint64_t) 0);
}

void TearDown() {
    memcached_free(memc);
}

TEST(LibmemcachedTest, AddGet) {
    memcached_add(memc, "hello", 5, "hello world", 11, 0, 0);
    size_t value_length = 64;
    uint32_t flag = 0;
    memcached_return_t ret;
    char *value = memcached_get(memc, "hello", 5, &value_length, &flag, &ret);
    ASSERT_STRCASEEQ("hello world", value);
    TearDown();
}

int main(int argc, char **argv) {
    SetUp();
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}