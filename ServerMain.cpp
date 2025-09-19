#include <chrono>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

#include "ServerSocket.h"
#include "ServerThread.h"

int main(int argc, char *argv[]) {
	int port;
	int engineer_cnt = 0;
	int adminId = -1;
	int numPeerServers = 0;
	ServerSocket socket;
	LaptopFactory factory;
	std::unique_ptr<ServerSocket> new_socket;
	std::vector<std::thread> thread_vector;
	
	if (argc < 4) {
		std::cout << "not enough arguments" << std::endl;
		std::cout << argv[0] << "[port #] [unique ID] [# peers] (repeat [ID] [IP] [port #])" << std::endl;
		return 0;
	}
	port = atoi(argv[1]);
	adminId = atoi(argv[2]);
	numPeerServers = atoi(argv[3]);
	if(argc < 4 + numPeerServers*3) {
		std::cout << "not enough arguments" << std::endl;
		std::cout << argv[0] << "[port #] [unique ID] [# peers] (repeat [ID] [IP] [port #])" << std::endl;
		return 0;
	}
	if (!socket.Init(port)) {
		std::cout << "Socket initialization failed" << std::endl;
		return 0;
	}

	for (int i = 0; i < numPeerServers; i++) {
		int id = atoi(argv[4 + i*3]);
		std::string ip = argv[5 + i*3];
		int portNum = atoi(argv[6 + i*3]);
		factory.adminMap[id] = std::make_pair(ip, portNum);
	}

	thread_vector.push_back(std::thread(&LaptopFactory::AdminThread, &factory, adminId));

	

	while ((new_socket = socket.Accept())) {
		std::thread engineer_thread(&LaptopFactory::EngineerThread, &factory, std::move(new_socket), engineer_cnt++);
		thread_vector.push_back(std::move(engineer_thread));
	}
	return 0;
}
