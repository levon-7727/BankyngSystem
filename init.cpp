#include "shm.h"
#include "bank.h"
#include <iostream>

int main() {
    size_t N = 5;  
    size_t bankSize = sizeof(Bank) + N * sizeof(Account);

    SharedMemory shm = create_shm(std::string("/mybank"), bankSize);
    Bank* bank = static_cast<Bank*>(shm.address);
    bank->N = N;

    for (size_t i = 0; i < bank->N; ++i) {
        bank->accounts[i].balance = 0;
        bank->accounts[i].minBalance = 0;
        bank->accounts[i].maxBalance = 1000000;
        bank->accounts[i].frozen = false;
    }

    std::cout << "Bank created\n";
    close_shm(shm);
    return 0;
}
