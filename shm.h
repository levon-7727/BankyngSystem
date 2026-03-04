#pragma once
#include <cstddef>
#include <string>
#include <pthread.h>
#include "bank.h"

struct SharedMemory {
    int fd;
    void* address;
    size_t size;
};

SharedMemory create_shm(std::string& name, size_t size);
SharedMemory open_shm(const std::string& name, size_t size);
void close_shm(SharedMemory& shm);
void unlink_shm(const std::string& name);
