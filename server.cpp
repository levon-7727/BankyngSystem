#include "bank.h"
#include "shm.h"

#include <iostream>
#include <pthread.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sstream>
#include <vector>
#include <cstring>

#define PORT 8080

Bank* bank;
int server_fd;
bool running = true;

pthread_mutex_t req_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t req_cond = PTHREAD_COND_INITIALIZER;

int request_count = 0;

std::vector<std::string> split(const std::string& s) {
    std::stringstream ss(s);
    std::string word;
    std::vector<std::string> res;

    while (ss >> word) {
        res.push_back(word);
    }

    return res;
}

void* stats_thread(void*) {
    while (running) {
        pthread_mutex_lock(&req_mutex);

        while (request_count < 5 && running) {
            pthread_cond_wait(&req_cond, &req_mutex);
        }

        if (!running) {
            pthread_mutex_unlock(&req_mutex);
            break;
        }

        std::cout << "[STATS] 5 requests processed\n";

        request_count = 0;

        pthread_mutex_unlock(&req_mutex);
    }

    return nullptr;
}

void* handle_client(void* arg) {
    int client = *(int*)arg;
    delete (int*)arg;

    char buffer[1024];

    while (true) {
        memset(buffer, 0, sizeof(buffer));

        int bytes = read(client, buffer, sizeof(buffer) - 1);

        if (bytes <= 0)
            break;

        std::string cmd(buffer);

        auto parts = split(cmd);

        if (parts.empty())
            continue;

        std::string response = "Unknown command";

        std::string op = parts[0];

        if (op == "current") {
            size_t A = std::stoul(parts[1]);

            if (validAccount(bank, A))
                response = std::to_string(getCurrent(bank, A));
            else
                response = "Invalid account";
        }

        else if (op == "transfer") {
            size_t A = std::stoul(parts[1]);
            size_t B = std::stoul(parts[2]);
            size_t X = std::stoul(parts[3]);

            if (transfer(bank, A, B, X))
                response = "Transfer successful";
            else
                response = "Transfer failed";
        }

        else if (op == "addAll") {
            size_t X = std::stoul(parts[1]);

            if (addAll(bank, X))
                response = "Added";
            else
                response = "Failed";
        }

        else if (op == "subAll") {
            size_t X = std::stoul(parts[1]);

            if (subAll(bank, X))
                response = "Subtracted";
            else
                response = "Failed";
        }

        else if (op == "freeze") {
            size_t A = std::stoul(parts[1]);

            if (freeze(bank, A))
                response = "Frozen";
            else
                response = "Failed";
        }

        else if (op == "unfreeze") {
            size_t A = std::stoul(parts[1]);

            if (unfreeze(bank, A))
                response = "Unfrozen";
            else
                response = "Failed";
        }

        else if (op == "setMin") {
            size_t A = std::stoul(parts[1]);
            size_t X = std::stoul(parts[2]);

            if (setMin(bank, A, X))
                response = "Min updated";
            else
                response = "Failed";
        }

        else if (op == "setMax") {
            size_t A = std::stoul(parts[1]);
            size_t X = std::stoul(parts[2]);

            if (setMax(bank, A, X))
                response = "Max updated";
            else
                response = "Failed";
        }

        else if (op == "shutdown") {
            response = "Server shutting down";

            running = false;

            close(server_fd);

            pthread_cond_signal(&req_cond);
        }

        write(client, response.c_str(), response.size());

        pthread_mutex_lock(&req_mutex);

        request_count++;

        if (request_count >= 5) {
            pthread_cond_signal(&req_cond);
        }

        pthread_mutex_unlock(&req_mutex);

        if (!running)
            break;
    }

    close(client);

    return nullptr;
}

int main() {
    size_t N = 5;

    size_t bankSize = sizeof(Bank) + N * sizeof(Account);

    SharedMemory shm = open_shm("/mybank", bankSize);

    bank = static_cast<Bank*>(shm.address);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in addr{};

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    bind(server_fd, (sockaddr*)&addr, sizeof(addr));

    listen(server_fd, 10);

    pthread_t stats;

    pthread_create(&stats, nullptr, stats_thread, nullptr);

    std::cout << "Server started on port 8080\n";

    while (running) {
        sockaddr_in client_addr{};
        socklen_t len = sizeof(client_addr);

        int client = accept(server_fd,
                            (sockaddr*)&client_addr,
                            &len);

        if (client < 0) {
            if (!running)
                break;

            continue;
        }

        pthread_t tid;

        int* pclient = new int(client);

        pthread_create(&tid,
                       nullptr,
                       handle_client,
                       pclient);

        pthread_detach(tid);
    }

    pthread_join(stats, nullptr);

    close_shm(shm);

    return 0;
}
