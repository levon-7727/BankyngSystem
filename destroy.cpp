#include "shm.h"
#include <iostream>

int main() {
    unlink_shm("/mybank");
    std::cout << "Bank destroyed\n";
}
