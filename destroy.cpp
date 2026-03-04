#include "shm.h"
#include <iostream>
#include <string>

int main() {
    std::string name = "/mybank";
    unlink_shm(name);
    std::cout << "Bank destroyed\n";
    return 0;
}
