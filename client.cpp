#include "shm.h"
#include "bank.h"
#include <iostream>
#include <string>

int main() {
    size_t N = 5;
    size_t bankSize = sizeof(Bank) + N * sizeof(Account);

    std::string name = "/mybank";
    SharedMemory shm = open_shm(std::string(name), bankSize);
    Bank* bank = static_cast<Bank*>(shm.address);

    std::cout << "Bank opened\n";
    std::cout << "Enter commands:\n";

    std::string cmd;
    while (std::cin >> cmd) {
        if (cmd == "current") {
            size_t A; std::cin >> A;
            if (validAccount(bank, A))
                std::cout << "Balance[" << A << "] = " << getCurrent(bank, A) << "\n";
            else
                std::cout << "Invalid account\n";
        }
        else if (cmd == "transfer") {
            size_t A, B, X; std::cin >> A >> B >> X;
            if (transfer(bank, A, B, X))
                std::cout << "Transfer successful\n";
            else
                std::cout << "Transfer failed\n";
        }
        else if (cmd == "addAll") {
            size_t X; std::cin >> X;
            if (addAll(bank, X))
                std::cout << "Added to all accounts\n";
            else
                std::cout << "Failed\n";
        }
        else if (cmd == "subAll") {
            size_t X; std::cin >> X;
            if (subAll(bank, X))
                std::cout << "Subtracted from all accounts\n";
            else
                std::cout << "Failed\n";
        }
        else if (cmd == "freeze") {
            size_t A; std::cin >> A;
            if (freeze(bank, A))
                std::cout << "Account frozen\n";
            else
                std::cout << "Failed\n";
        }
        else if (cmd == "unfreeze") {
            size_t A; std::cin >> A;
            if (unfreeze(bank, A))
                std::cout << "Account unfrozen\n";
            else
                std::cout << "Failed\n";
        }
        else if (cmd == "setMin") {
            size_t A, X; std::cin >> A >> X;
            if (setMin(bank, A, X))
                std::cout << "Min balance set\n";
            else
                std::cout << "Failed\n";
        }
        else if (cmd == "setMax") {
            size_t A, X; std::cin >> A >> X;
            if (setMax(bank, A, X))
                std::cout << "Max balance set\n";
            else
                std::cout << "Failed\n";
        }
        else if (cmd == "exit") {
            break;
        }
        else {
            std::cout << "Unknown command\n";
        }
    }

    close_shm(shm);
    return 0;
}
