#include "bank.h"
#include <pthread.h>

bool validAccount(Bank* bank, size_t A) {
    return A < bank->N;
}

size_t getCurrent(Bank* bank, size_t A) {
    pthread_mutex_lock(&bank->mutex);
    size_t value = bank->accounts[A].balance;
    pthread_mutex_unlock(&bank->mutex);
    return value;
}

size_t getMin(Bank* bank, size_t A) {
    pthread_mutex_lock(&bank->mutex);
    size_t value = bank->accounts[A].minBalance;
    pthread_mutex_unlock(&bank->mutex);
    return value;
}

size_t getMax(Bank* bank, size_t A) {
    pthread_mutex_lock(&bank->mutex);
    size_t value = bank->accounts[A].maxBalance;
    pthread_mutex_unlock(&bank->mutex);
    return value;
}

bool transfer(Bank* bank, size_t A, size_t B, size_t X) {
    if (!validAccount(bank, A) || !validAccount(bank, B))
        return false;

    pthread_mutex_lock(&bank->mutex);
    Account& from = bank->accounts[A];
    Account& to   = bank->accounts[B];

    if (from.frozen || to.frozen ||
        from.balance < X ||
        from.balance - X < from.minBalance ||
        to.balance + X > to.maxBalance) {
        pthread_mutex_unlock(&bank->mutex);
        return false;
    }

    from.balance -= X;
    to.balance += X;
    pthread_mutex_unlock(&bank->mutex);
    return true;
}

bool addAll(Bank* bank, size_t X) {
    pthread_mutex_lock(&bank->mutex);
    for (size_t i = 0; i < bank->N; ++i) {
        if (!bank->accounts[i].frozen &&
            bank->accounts[i].balance + X > bank->accounts[i].maxBalance) {
            pthread_mutex_unlock(&bank->mutex);
            return false;
        }
    }
    for (size_t i = 0; i < bank->N; ++i) {
        if (!bank->accounts[i].frozen)
            bank->accounts[i].balance += X;
    }
    pthread_mutex_unlock(&bank->mutex);
    return true;
}

bool subAll(Bank* bank, size_t X) {
    pthread_mutex_lock(&bank->mutex);
    for (size_t i = 0; i < bank->N; ++i) {
        if (!bank->accounts[i].frozen &&
            (bank->accounts[i].balance < X ||
             bank->accounts[i].balance - X < bank->accounts[i].minBalance)) {
            pthread_mutex_unlock(&bank->mutex);
            return false;
        }
    }
    for (size_t i = 0; i < bank->N; ++i) {
        if (!bank->accounts[i].frozen)
            bank->accounts[i].balance -= X;
    }
    pthread_mutex_unlock(&bank->mutex);
    return true;
}

bool setMin(Bank* bank, size_t A, size_t X) {
    if (!validAccount(bank, A)) return false;
    pthread_mutex_lock(&bank->mutex);
    bank->accounts[A].minBalance = X;
    pthread_mutex_unlock(&bank->mutex);
    return true;
}

bool setMax(Bank* bank, size_t A, size_t X) {
    if (!validAccount(bank, A)) return false;
    pthread_mutex_lock(&bank->mutex);
    bank->accounts[A].maxBalance = X;
    pthread_mutex_unlock(&bank->mutex);
    return true;
}

bool freeze(Bank* bank, size_t A) {
    if (!validAccount(bank, A)) return false;
    pthread_mutex_lock(&bank->mutex);
    bank->accounts[A].frozen = true;
    pthread_mutex_unlock(&bank->mutex);
    return true;
}

bool unfreeze(Bank* bank, size_t A) {
    if (!validAccount(bank, A)) return false;
    pthread_mutex_lock(&bank->mutex);
    bank->accounts[A].frozen = false;
    pthread_mutex_unlock(&bank->mutex);
    return true;
}
