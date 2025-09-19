#include "ClientStub.h"
#include <arpa/inet.h> 
#include <iostream>

ClientStub::ClientStub() {}

int ClientStub::Init(std::string ip, int port) {
	return socket.Init(ip, port);	
}

LaptopInfo ClientStub::OrderLaptop(CustomerRequest request) {
	//std::cout<<"Ordering laptop for custid "<<request.GetCustomerId()<<std::endl;
	//std::cout<<"request received custid "<< request.GetCustomerId()<<std::endl;
	LaptopInfo info;
	char buffer[32];
	int size;
	request.Marshal(buffer);
	size = request.Size();
	if (socket.Send(buffer, size, 0)) {
		size = info.Size();
		if (socket.Recv(buffer, size, 0)) {
			info.Unmarshal(buffer);
		} 
	}
	// std::cout<<"laptop info for ";
	// info.Print();
	return info;
}



CustomerRecord ClientStub::ReadRecord(CustomerRequest order) {

	//std::cout<<"read request received custid "<< order.GetCustomerId()<<std::endl;

	CustomerRecord record;
	char buffer[32];
	int size;
	order.Marshal(buffer);
	size = order.Size();
	if (socket.Send(buffer, size, 0)) {
		size = record.Size();
		if (socket.Recv(buffer, size, 0)) {
			record.Unmarshal(buffer);
		}
	}
	// std::cout<<"record sending for ";
	// record.Print();
	return record;

}

ResponseLog ClientStub::BackupRecord(RequestLog log) {
  ResponseLog resp;
  char buffer[32];
  int size;
  log.Marshal(buffer);
  size = log.Size();
  if (socket.Send(buffer, size, 0)) {
    size = resp.Size();
    if (socket.Recv(buffer, size, 0)) {
      resp.Unmarshal(buffer);
    }
  }
  return resp;
}

void ClientStub::SetRole(int role) {
	int net_role = htonl(role);
	socket.Send((char *)&net_role, sizeof(net_role), 0);
}
