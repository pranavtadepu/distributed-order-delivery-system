#include "ClientThread.h"
#include "Messages.h"

#include <iostream>

ClientThreadClass::ClientThreadClass() {}

void ClientThreadClass::
ThreadBody(std::string ip, int port, int id, int orders, int type) {
	customer_id = id;
	num_orders = orders;
	request_type = type;
	if (!stub.Init(ip, port)) {
		std::cout << "Thread " << customer_id << " failed to connect" << std::endl;
		return;
	}
	stub.SetRole(0);
	if(type == 1) { //1 - regular laptop order request
		for (int i = 0; i < num_orders; i++) {
			CustomerRequest order;
			LaptopInfo laptop;
			order.SetOrder(customer_id, i, request_type);

			timer.Start();
			laptop = stub.OrderLaptop(order);
			timer.EndAndMerge();

			if (!laptop.IsValid()) {
				std::cout << "Invalid laptop " << customer_id << std::endl;
				break;	
			}
		}
	}
	else if (type == 2) { // 2 - customer record read request
		// Step 2.a: A customer sends a record read request for its own record
		CustomerRequest customerRequest;
		customerRequest.SetOrder(customer_id, -1, 2);
		
		timer.Start();
		// Step 2.b: The engineer reads the corresponding record from the customer record
		CustomerRecord customerRecord = stub.ReadRecord(customerRequest); // Step b
		timer.EndAndMerge();

		if(customerRecord.IsValid()) {
			std::cout<<"Customer " << customerRecord.GetCustomerId() << " last order " << customerRecord.GetLastOrder() << std::endl;
		}
		else {
			std::cout << "Customer "<<customer_id << " does not exist" << std::endl;
		}
	}
	else if (type == 3) {
		for(int i=0; i<num_orders; i++) {
			CustomerRequest customerRequest;
			CustomerRecord customerRecord;
			customerRequest.SetOrder(i, -1, 2);
			timer.Start();
			customerRecord = stub.ReadRecord(customerRequest);
			timer.EndAndMerge();
			if(customerRecord.IsValid()) {
				std::cout<<"CustomerId " << customerRecord.GetCustomerId()<<" LastOrder "<<customerRecord.GetLastOrder()<<std::endl;
			}
		}
	}


}

ClientTimer ClientThreadClass::GetTimer() {
	return timer;	
}