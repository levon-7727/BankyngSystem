#include "shm.h"
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdexcept>
#include <pthread.h>
#include <iostream>

SharedMemory create_shm(std::string& name, size_t size) {
    SharedMemory shm{};

    shm.fd = shm_open(name.c_str(), O_CREAT | O_RDWR, 0666);
    if (shm.fd == -1) {
        throw std::runtime_error("shm_open failed");
    }

    if (ftruncate(shm.fd, size) == -1) {
        close(shm.fd);
        throw std::runtime_error("ftruncate failed");
    }

    shm.address = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_SHARED, shm.fd, 0);
    if (shm.address == MAP_FAILED) {
        close(shm.fd);
        throw std::runtime_error("mmap failed");
    }

    shm.size = size;

    Bank* bank = static_cast<Bank*>(shm.address);
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(&bank->mutex, &attr);
    pthread_mutexattr_destroy(&attr);

    return shm;
}

SharedMemory open_shm(const std::string& name, size_t size) {
    SharedMemory shm{};

    shm.fd = shm_open(name.c_str(), O_RDWR, 0666);
    if (shm.fd == -1) {
        throw std::runtime_error("shm_open failed");
    }

    shm.address = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_SHARED, shm.fd, 0);
    if (shm.address == MAP_FAILED) {
        close(shm.fd);
        throw std::runtime_error("mmap failed");
    }

    shm.size = size;
    return shm;
}

void close_shm(SharedMemory& shm) {
    munmap(shm.address, shm.size);
    close(shm.fd);
}

void unlink_shm(const std::string& name) {
    shm_unlink(name.c_str());
}
