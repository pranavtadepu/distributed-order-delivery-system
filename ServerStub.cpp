#include "ServerStub.h"
#include <arpa/inet.h>
#include <iostream>

ServerStub::ServerStub() {}

void ServerStub::Init(std::unique_ptr<ServerSocket> socket) {
	this->socket = std::move(socket);
}

CustomerRequest ServerStub::ReceiveRequest() {
	char buffer[32];
	CustomerRequest request;
	if (socket->Recv(buffer, request.Size(), 0)) {
		request.Unmarshal(buffer);
	}
	return request;	
}

int ServerStub::ShipLaptop(LaptopInfo info) {
	char buffer[32];
	info.Marshal(buffer);
  // std::cout<<"Shipping laptop: "<<std::endl;
  // info.Print();
	return socket->Send(buffer, info.Size(), 0);
}

int ServerStub::ReturnRecord(CustomerRecord record) {
	char buffer[32];
	record.Marshal(buffer);
	return socket->Send(buffer, record.Size(), 0);
}

RequestLog ServerStub::ReceiveRequestLog() {
  char buffer[32];
  RequestLog response;
  if (socket->Recv(buffer, response.Size(), 0)) {
    response.Unmarshal(buffer);
  }
  return response;
}

int ServerStub::ReturnResponseLog(ResponseLog response) {
  char buffer[32];
  response.Marshal(buffer);
  return socket->Send(buffer, response.Size(), 0);
}

int ServerStub::ReceiveServerRole() {
  char buffer[32];
  int serverRole = -1;
  if (socket->Recv(buffer, sizeof(int), 0)) {
    serverRole = ntohl(*(int *)buffer);
  }
  return serverRole;
}