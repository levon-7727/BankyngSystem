#include "shm.h"
#include "bank.h"
#include <iostream>

int main() {
    SharedMemory shm = create_shm("/mybank", sizeof(Bank));
    Bank* bank = static_cast<Bank*>(shm.address);
    bank->N = 5;

    for (size_t i = 0; i < bank->N; ++i) {
        bank->accounts[i].balance = 0;
        bank->accounts[i].min_balance = 0;
        bank->accounts[i].max_balance = 1000000;
        bank->accounts[i].frozen = false;
    }

    std::cout << "Bank created\n";
    close_shm(shm);
}
