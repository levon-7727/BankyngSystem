CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -pthread

all: init destroy server client

init: init.cpp shm.cpp
	$(CXX) $(CXXFLAGS) init.cpp shm.cpp -o init

destroy: destroy.cpp shm.cpp
	$(CXX) $(CXXFLAGS) destroy.cpp shm.cpp -o destroy

server: server.cpp bank.cpp shm.cpp
	$(CXX) $(CXXFLAGS) server.cpp bank.cpp shm.cpp -o server

client: client.cpp
	$(CXX) $(CXXFLAGS) client.cpp -o client

clean:
	rm -f init destroy server client
