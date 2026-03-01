#pragma once

struct Account {
    size_t balance;
    size_t minBalance;
    size_t maxBalance;
    bool frozen;
};

struct Bank {
    size_t N;
    Account accounts[];
};

bool validAccount(Bank* bank, size_t A);
size_t getCurrent(Bank* bank, size_t A);
size_t getMin(Bank* bank, size_t A);
size_t getMax(Bank* bank, size_t A);
bool transfer(Bank* bank, size_t A, size_t B, size_t X);
bool addAll(Bank* bank, size_t X);
bool subAll(Bank* bank, size_t X);
bool setMin(Bank* bank, size_t A, size_t X);
bool setMax(Bank* bank, size_t A, size_t X);
bool freeze(Bank* bank, size_t A);
bool unfreeze(Bank* bank, size_t A);
