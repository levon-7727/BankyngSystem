#include "Shm.h"
#include <sys/stat.h>
#include <sys/mmap.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdexcept>
#include <pthread.h>
#include <iostream>

SharedMemory create_shm(std::string& name, size_t size){
    SharedMemory shm{};
    
    shm.fd = shm_open(name, O_CREATE | O_RDWR, 0666);
    if(shm.fd == -1){
        thrrow std::runtime_error("shm_open failed");
    }
    if(ftruncate(shm.fd, size) == -1){
        throw std::runtime_error("ftruncate failed");
    }
    
    shm.address = shm.mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_SHARED, shm.fd, 0);
    if(shm.address == MAP_FAILED){
        throw std::runtime_error("mmap failed");
    }
    shm.size = size;

    Bank* bank = static_cast<Bank*>(shm.address);

    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
    pthread_mutexattr_destroy(&attr);
    return shm;
}

SharedMemory open_shm(std::string& name, size_t size){
    SharedMemory shm{};
    
    shm.fd = shm_open(name, O_RDWR, 0666);
    if(shm.fd == -1){
        thrrow std::runtime_error("shm_open failed");
    }
    
    shm.address = shm.mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_SHARED, shm.fd, 0);
    if(shm.address == MAP_FAILED){
        throw std::runtime_error("mmap failed");
    }
    
    shm.size = size;
    return shm;
}

void close_shm(SharedMemory& shm){
    munmap(shm.address, size);
    close(shm.fd);
}

void unlink_shm(std::string& name){
    shm_unlink(name);
}
