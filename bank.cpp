#include "bank.h"
#include <cassert>

bool validAccount(Bank* bank, size_t A){
    return A < bank->N;
}

size_t getCurrent(Bank* bank, size_t A){
    assert(validAccount(bank, A));
    return bank->accounts[A].balance;
}

size_t getMin(Bank* bank, size_t A){
    assert(validAccount(bank, A));
    return bank->accounts[A].minBalance;
}

size_t getMax(Bank* bank, size_t A){
    assert(validAccount(bank, A));
    return bank->accounts[A].maxBalance;
}

bool freeze(Bank* bank, size_t A){
    if(!validAccount(bank, A)) return false;
    bank->accounts[A].frozen = true;
    return true;
}

bool unfreeze(Bank* bank, size_t A){
    if(!validAccount(bank, A)) return false;
    bank->accounts[A].frozen = false;
    return true;    
} 

bool transfer(Bank* bank, size_t A, size_t B, size_t x){
    if(!validAccount(bank, A)  !validAccount(bank, B)  x <= 0) return false;
    if(bank->accounts[A].frozen  bank->accounts[B].frozen) return false;
    if(bank->accounts[A].balance - x < bank->accounts[A].minBalance) return false;
    if(bank->accounts[B].balance + x > bank->accounts[B].maxBalance) return false;
    bank->accounts[A].balance -= x;
    bank->accounts[B].balance += x;
    return true;
}

bool addAll(Bank* bank, size_t x){
    for(int i = 0; i < bank->N; i++){
        if(bank->accounts[i].frozen  bank->accounts[i].balance + x > bank->accounts[i].maxBalance){
            return false;
        }
    }
    for(int i = 0; i < bank->N; i++){
        bank->accounts[i].balance += x;
    }
    return true;
}

bool subAll(Bank* bank, size_t x){
    for(int i = 0; i < bank->N; i++){
        if(bank->accounts[i].frozen || bank->accounts[i].balance - x < bank->accounts[i].minBalance){
            return false;
        }
    }
    for(int i = 0; i < bank->N; i++){
        bank->accounts[i].balance -= x;
    }
    return true;
}

bool setMin(Bank* bank, size_t A, size_t x){
    if(!validAccount(bank, A)) return false;
    if(bank->accounts[A].balance < x) return false;
    bank->accounts[A].minBalance = x;
    return true;
}

bool setMax(Bank* bank, size_t A, size_t x){
    if(!validAccount(bank, A)) return false;
    if(bank->accounts[A].balance > x) return false;
    bank->accounts[A].maxBalance = x;
    return true;
}
