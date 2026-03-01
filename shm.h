#pragma once

struct SharedMemory{
    int fd;
    void* address;
    size_t size;
};

void create_shm(std::string& name; size_t size);
void open_shm(std::string, size_t size);
void close_shm(SharedMemory& shm);
void unlink_shm(std::string& name);
