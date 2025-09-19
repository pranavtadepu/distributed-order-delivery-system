#ifndef __SERVERTHREAD_H__
#define __SERVERTHREAD_H__

#include <condition_variable>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <map>

#include "Messages.h"
#include "ServerSocket.h"

#include "ClientStub.h"

struct AdminRequest {
	LaptopInfo laptop;
	std::promise<LaptopInfo> prom;
};

class LaptopFactory {
private:
	std::queue<std::unique_ptr<AdminRequest>> erq;
	std::mutex erq_lock;
	std::condition_variable erq_cv;
	std::mutex cr_lock;
	std::map<int, int> customerRecordMap;
	std::map<int, ClientStub> adminStub;
	std::vector<MapOp> smr_log;
	bool adminStubEnabled;
	int factoryId;
	int commitId;
	int lastIndex;
	int primaryId;

public:
	std::map<int, std::pair<std::string, int>> adminMap;
	void EngineerThread(std::unique_ptr<ServerSocket> socket, int id);
	void AdminThread(int id);
	LaptopInfo CreateRegularLaptop(CustomerRequest order, int engineer_id);
	CustomerRecord CreateCustomerRecord(CustomerRequest order);
	void PFA(LaptopInfo& laptopInfo); // Production Factory Admin

	//void ExpertThread(int id);
};

#endif // end of #ifndef __SERVERTHREAD_H__

