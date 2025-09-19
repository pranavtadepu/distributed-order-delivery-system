#include <iostream>
#include <memory>

#include "ServerThread.h"
#include "ServerStub.h"

LaptopInfo LaptopFactory::
CreateRegularLaptop(CustomerRequest order, int engineer_id) {
	LaptopInfo laptop;
	laptop.CopyOrder(order);
	laptop.SetEngineerId(engineer_id);
	laptop.SetExpertId(-1);
	std::promise<LaptopInfo> promise;
	std::future<LaptopInfo> future = promise.get_future();
	std::unique_ptr<AdminRequest> request = std::unique_ptr<AdminRequest>(new AdminRequest);
	request->laptop = laptop;
	request->prom = std::move(promise);
	erq_lock.lock();
	erq.push(std::move(request));
	erq_cv.notify_one();
	erq_lock.unlock();
	laptop = future.get();
	return laptop;
}

CustomerRecord LaptopFactory::CreateCustomerRecord(CustomerRequest request) {
	CustomerRecord customerRecord;
	int lastOrder = -1;
	int customerId = request.GetCustomerId();
	{
		std::lock_guard<std::mutex> lock(cr_lock);
		if (customerRecordMap.find(customerId) != customerRecordMap.end()) {
			lastOrder = customerRecordMap[customerId];
			//customerRecord.SetLastOrder(customerRecordMap[customerId]);
		} 
		else {
			customerId = -1;
			lastOrder = -1;
			customerRecord.SetCustomerId(-1);
			customerRecord.SetLastOrder(-1);
		}
	}
	customerRecord.SetCustomerRecord(customerId, lastOrder);
	return customerRecord;
}

// LaptopInfo LaptopFactory::
// CreateCustomLaptop(CustomerRequest order, int engineer_id) {
// 	LaptopInfo laptop;
// 	laptop.CopyOrder(order);
// 	laptop.SetEngineerId(engineer_id);

// 	std::promise<LaptopInfo> prom;
// 	std::future<LaptopInfo> fut = prom.get_future();

// 	std::unique_ptr<AdminRequest> req = 
// 		std::unique_ptr<AdminRequest>(new AdminRequest);
// 	req->laptop = laptop;
// 	req->prom = std::move(prom);

// 	erq_lock.lock();
// 	erq.push(std::move(req));
// 	erq_cv.notify_one();
// 	erq_lock.unlock();

// 	laptop = fut.get();
// 	return laptop;
// }

void LaptopFactory::
EngineerThread(std::unique_ptr<ServerSocket> socket, int id) {
	int engineer_id = id;
	int request_type;
	CustomerRequest customerRequest;
	LaptopInfo laptop;
    RequestLog requestLog;
    ResponseLog responseLog;
	ServerStub stub;

	stub.Init(std::move(socket));

	int serverRole = stub.ReceiveServerRole();
	switch (serverRole) 
	{
		case 0:
			while(true) {
				customerRequest = stub.ReceiveRequest();
				//std::cout<<"Server: Customer Request received: "<<std::endl;
				//customerRequest.Print();
				if (customerRequest.IsValid() == false) break;
				request_type = customerRequest.GetRequestType();
				if(request_type == 1) {
					laptop = CreateRegularLaptop(customerRequest, engineer_id);
					//std::cout<<"created regular laptop: "<<std::endl;
					//laptop.Print();
					stub.ShipLaptop(laptop);
				}
				else if(request_type == 2) 
					stub.ReturnRecord(CreateCustomerRecord(customerRequest));
				else
					std::cout<< "laptop type not supported"<< std::endl;
			}
			break;
		
		case 1:
			while (true)
			{
				requestLog = stub.ReceiveRequestLog();
				if(requestLog.IsValid()) {
					primaryId = requestLog.GetFactoryId();
					MapOp mapOp = requestLog.GetMapOp();
					smr_log.emplace_back(mapOp);
					commitId = requestLog.GetCommitIndex();
					lastIndex = requestLog.GetLastIndex();
					if(commitId >= 0) {
						std::lock_guard<std::mutex> lock(cr_lock);
						MapOp toApply = smr_log[commitId];
						customerRecordMap[toApply.GetArg1()] = toApply.GetArg2();
					}
					responseLog.SetFactoryId(factoryId);
					stub.ReturnResponseLog(responseLog);
				}
				else {
					std::cout<<"Invalid request log"<<std::endl;
					break;
				}
			}
			
			break;
		default:
			std::cout<<"Server role not supported" << std::endl;
			break;
	}
}

void LaptopFactory::PFA(LaptopInfo &laptop) {
	primaryId = factoryId;
	if(adminStubEnabled == false) {
		for(auto& admin: adminMap) {
			int id = admin.first;
			std::pair<std::string, int>pr =admin.second;
			std::string ip = pr.first;
			int portNum = pr.second;
			if(adminStub[id].Init(ip, portNum)) {
				adminStub[id].SetRole(1);
			}
		}
		adminStubEnabled = true;
	}
	MapOp mapOp;
	mapOp.SetMapOp(1, laptop.GetCustomerId(), laptop.GetOrderNumber());
	smr_log.emplace_back(mapOp);
	lastIndex = smr_log.size() - 1;
	RequestLog requestLog; 
	requestLog.SetFactoryId(factoryId);
	requestLog.SetCommitIndex(commitId);
	requestLog.SetLastIndex(lastIndex);
	requestLog.SetMapOp(mapOp);

	
	for(auto x = adminStub.begin(); x != adminStub.end();) {
		//std::cout<<x->first << " "<<std::endl;
		ResponseLog responseLog = x->second.BackupRecord(requestLog);
		if(responseLog.IsValid())
			x++;
		else
			x = adminStub.erase(x);
	}
	std::lock_guard<std::mutex> lock(cr_lock);
	customerRecordMap[laptop.GetCustomerId()] = laptop.GetOrderNumber();
	commitId = lastIndex;
}


void LaptopFactory::AdminThread(int id) {
	std::unique_lock<std::mutex> ul(erq_lock, std::defer_lock);
	commitId = -1;
	lastIndex = -1;
	factoryId = id;
	primaryId = -1;
	adminStubEnabled = false;

	while (true) {
		ul.lock();

		if (erq.empty()) {
			erq_cv.wait(ul, [this]{ return !erq.empty(); });
		}

		auto req = std::move(erq.front());
		erq.pop();

		ul.unlock();

		PFA(req->laptop);
		//std::this_thread::sleep_for(std::chrono::microseconds(100));
		req->laptop.SetExpertId(id);
		req->prom.set_value(req->laptop);	
	}
}

