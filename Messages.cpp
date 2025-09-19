#include <cstring>
#include <iostream>

#include <arpa/inet.h>
#include "Messages.h"

CustomerRequest::CustomerRequest() {
	customer_id = -1;
	order_number = -1;
	request_type = -1;
}

void CustomerRequest::SetOrder(int id, int number, int type) {
	customer_id = id;
	order_number = number;
	request_type = type;
}

int CustomerRequest::GetCustomerId() { return customer_id; }
int CustomerRequest::GetOrderNumber() { return order_number; }
int CustomerRequest::GetRequestType() { return request_type; }

int CustomerRequest::Size() {
	return sizeof(customer_id) + sizeof(order_number) + sizeof(request_type);
}

void CustomerRequest::Marshal(char *buffer) {
	int net_customer_id = htonl(customer_id);
	int net_order_number = htonl(order_number);
	int net_request_type = htonl(request_type);
	int offset = 0;
	memcpy(buffer + offset, &net_customer_id, sizeof(net_customer_id));
	offset += sizeof(net_customer_id);
	memcpy(buffer + offset, &net_order_number, sizeof(net_order_number));
	offset += sizeof(net_order_number);
	memcpy(buffer + offset, &net_request_type, sizeof(net_request_type));
}

void CustomerRequest::Unmarshal(char *buffer) {
	int net_customer_id;
	int net_order_number;
	int net_request_type;
	int offset = 0;
	memcpy(&net_customer_id, buffer + offset, sizeof(net_customer_id));
	offset += sizeof(net_customer_id);
	memcpy(&net_order_number, buffer + offset, sizeof(net_order_number));
	offset += sizeof(net_order_number);
	memcpy(&net_request_type, buffer + offset, sizeof(net_request_type));

	customer_id = ntohl(net_customer_id);
	order_number = ntohl(net_order_number);
	request_type = ntohl(net_request_type);
}

bool CustomerRequest::IsValid() {
	return (customer_id != -1);
}

void CustomerRequest::Print() {
	std::cout << "id " << customer_id << " ";
	std::cout << "num " << order_number << " ";
	std::cout << "type " << request_type << std::endl;
}

LaptopInfo::LaptopInfo() {
	customer_id = -1;
	order_number = -1;
	request_type = -1;
	engineer_id = -1;
	admin_id = -1;
}

void LaptopInfo::SetInfo(int id, int number, int type, int engid, int expid) {
	customer_id = id;
	order_number = number;
	request_type = type;
	engineer_id = engid;
	admin_id = expid;
}

void LaptopInfo::CopyOrder(CustomerRequest order) {
	customer_id = order.GetCustomerId();
	order_number = order.GetOrderNumber();
	request_type = order.GetRequestType();
}
void LaptopInfo::SetEngineerId(int id) { engineer_id = id; }
void LaptopInfo::SetExpertId(int id) { admin_id = id; }

int LaptopInfo::GetCustomerId() { return customer_id; }
int LaptopInfo::GetOrderNumber() { return order_number; }
int LaptopInfo::GetRequestType() { return request_type; }
int LaptopInfo::GetEngineerId() { return engineer_id; }
int LaptopInfo::GetExpertId() { return admin_id; }

int LaptopInfo::Size() {
	return sizeof(customer_id) + sizeof(order_number) + sizeof(request_type)
		+ sizeof(engineer_id) + sizeof(admin_id);
}

void LaptopInfo::Marshal(char *buffer) {
	int net_customer_id = htonl(customer_id);
	int net_order_number = htonl(order_number);
	int net_request_type = htonl(request_type);
	int net_engineer_id = htonl(engineer_id);
	int net_expert_id = htonl(admin_id);
	int offset = 0;

	memcpy(buffer + offset, &net_customer_id, sizeof(net_customer_id));
	offset += sizeof(net_customer_id);
	memcpy(buffer + offset, &net_order_number, sizeof(net_order_number));
	offset += sizeof(net_order_number);
	memcpy(buffer + offset, &net_request_type, sizeof(net_request_type));
	offset += sizeof(net_request_type);
	memcpy(buffer + offset, &net_engineer_id, sizeof(net_engineer_id));
	offset += sizeof(net_engineer_id);
	memcpy(buffer + offset, &net_expert_id, sizeof(net_expert_id));

}

void LaptopInfo::Unmarshal(char *buffer) {
	int net_customer_id;
	int net_order_number;
	int net_request_type;
	int net_engineer_id;
	int net_expert_id;
	int offset = 0;

	memcpy(&net_customer_id, buffer + offset, sizeof(net_customer_id));
	offset += sizeof(net_customer_id);
	memcpy(&net_order_number, buffer + offset, sizeof(net_order_number));
	offset += sizeof(net_order_number);
	memcpy(&net_request_type, buffer + offset, sizeof(net_request_type));
	offset += sizeof(net_request_type);
	memcpy(&net_engineer_id, buffer + offset, sizeof(net_engineer_id));
	offset += sizeof(net_engineer_id);
	memcpy(&net_expert_id, buffer + offset, sizeof(net_expert_id));

	customer_id = ntohl(net_customer_id);
	order_number = ntohl(net_order_number);
	request_type = ntohl(net_request_type);
	engineer_id = ntohl(net_engineer_id);
	admin_id = ntohl(net_expert_id);
}

bool LaptopInfo::IsValid() {
	return (customer_id != -1);
}

void LaptopInfo::Print() {
	std::cout << "id " << customer_id << " ";
	std::cout << "num " << order_number << " ";
	std::cout << "type " << request_type << " ";
	std::cout << "engid " << engineer_id << " ";
	std::cout << "expid " << admin_id << std::endl;
}

CustomerRecord::CustomerRecord() {
	customer_id = -1;
	last_order = -1;
}

void CustomerRecord::SetCustomerRecord(int cid, int lo) {
	customer_id = cid;
	last_order = lo;
}

void CustomerRecord::SetCustomerId(int cid) { customer_id = cid; }
void CustomerRecord::SetLastOrder(int lo) { last_order = lo; }

int CustomerRecord::GetCustomerId() {return customer_id;}
int CustomerRecord::GetLastOrder() {return last_order;}

int CustomerRecord::Size() {
	return sizeof(customer_id) + sizeof(last_order);
}

void CustomerRecord::Marshal(char* buffer) {
	int net_customer_id = htonl(customer_id);
	int net_last_order = htonl(last_order);
	int offset = 0;

	memcpy(buffer + offset, &net_customer_id, sizeof(net_customer_id));
	offset += sizeof(net_customer_id);
	memcpy(buffer + offset, &net_last_order, sizeof(net_last_order));
}

void CustomerRecord::Unmarshal(char *buffer) {
	int net_customer_id;
	int net_last_order;
	int offset = 0;

	memcpy(&net_customer_id, buffer + offset, sizeof(net_customer_id));
	offset += sizeof(net_customer_id);
	memcpy(&net_last_order, buffer + offset, sizeof(net_last_order));

	customer_id = ntohl(net_customer_id);
	last_order = ntohl(net_last_order);
}

bool CustomerRecord::IsValid() {
	return (customer_id != -1);
}

void CustomerRecord::Print() {
	std::cout << "Cust id " << customer_id << " ";
	std::cout << "Last order " << last_order << std::endl;
}

MapOp::MapOp() {
	opcode = -1;
	arg1 = -1;
	arg2 = -1;
}

int MapOp::GetOpcode() {
	return opcode;
}

int MapOp::GetArg1() {
	return arg1;
}

int MapOp::GetArg2() {
	return arg2;
}

void MapOp::SetMapOp(int opCode, int argument1, int argument2) {
	opcode = opCode;
	arg1 = argument1;
	arg2 = argument2;

}

void MapOp::SetOpcode(int op) {
	opcode = op;
}

void MapOp::SetArg1(int a1) {
	arg1 = a1;
}

void MapOp::SetArg2(int a2) {
	arg2 = a2;
}

void MapOp::CopyMapOp(MapOp op) {
	opcode = op.opcode;
	arg1 = op.arg1;
	arg2 = op.arg2;
}

int MapOp::Size() {
  	return sizeof(opcode) + sizeof(arg1) + sizeof(arg2);
}

void MapOp::Marshal(char *buffer) {
	int net_opcode = htonl(opcode);
	int net_arg1 = htonl(arg1);
	int net_arg2 = htonl(arg2);
	int offset = 0;

	memcpy(buffer + offset, &net_opcode, sizeof(net_opcode));
	offset += sizeof(net_opcode);
	memcpy(buffer + offset, &net_arg1, sizeof(net_arg1));
	offset += sizeof(net_arg1);
	memcpy(buffer + offset, &net_arg2, sizeof(net_arg2));
}

void MapOp::Unmarshal(char *buffer) {
	int net_opcode;
	int net_arg1;
	int net_arg2;
	int offset = 0;

	memcpy(&net_opcode, buffer + offset, sizeof(net_opcode));
	offset += sizeof(net_opcode);
	memcpy(&net_arg1, buffer + offset, sizeof(net_arg1));
	offset += sizeof(net_arg1);
	memcpy(&net_arg2, buffer + offset, sizeof(net_arg2));

	opcode = ntohl(net_opcode);
	arg1 = ntohl(net_arg1);
	arg2 = ntohl(net_arg2);
}

bool MapOp::isValid() {
	return (opcode != -1);
}

void MapOp::Print() {
	std::cout << "opcode " << opcode << " ";
	std::cout << "arg1 " << arg1 << " ";
	std::cout << "arg2 " << arg2 << std::endl;
}

RequestLog::RequestLog() {
	factoryId = -1;
	commitIndex = -1;
	lastIndex = -1;
}

int RequestLog::GetFactoryId() {
	return factoryId;
}

int RequestLog::GetCommitIndex() {
	return commitIndex;
}

int RequestLog::GetLastIndex() {
	return lastIndex;
}

MapOp RequestLog::GetMapOp() {
	return mapOp;
}

void RequestLog::SetFactoryId(int factId) {
	factoryId = factId;
}

void RequestLog::SetCommitIndex(int idx) {
	commitIndex = idx;
}

void RequestLog::SetLastIndex(int lastIdx) {
	lastIndex = lastIdx;
}

void RequestLog::SetMapOp(MapOp op) {
	mapOp = op;
}

void RequestLog::CopyRequest(RequestLog req) {
	factoryId = req.factoryId;
	commitIndex = req.commitIndex;
	lastIndex = req.lastIndex;
	mapOp.CopyMapOp(req.mapOp);
}

int RequestLog::Size() {
  return sizeof(factoryId) + sizeof(commitIndex) + sizeof(lastIndex) + mapOp.Size();
}

void RequestLog::Marshal(char *buffer) {
	int net_factoryId = htonl(factoryId);
	int net_commitIndex = htonl(commitIndex);
	int net_lastIndex = htonl(lastIndex);
	int offset = 0;

	memcpy(buffer + offset, &net_factoryId, sizeof(net_factoryId));
	offset += sizeof(net_factoryId);
	memcpy(buffer + offset, &net_commitIndex, sizeof(net_commitIndex));
	offset += sizeof(net_commitIndex);
	memcpy(buffer + offset, &net_lastIndex, sizeof(net_lastIndex));
	offset += sizeof(net_lastIndex);
	mapOp.Marshal(buffer + offset);
}

void RequestLog::Unmarshal(char *buffer) {
	int net_factoryId;
	int net_commitIndex;
	int net_lastIndex;
	int offset = 0;

	memcpy(&net_factoryId, buffer + offset, sizeof(net_factoryId));
	offset += sizeof(net_factoryId);
	memcpy(&net_commitIndex, buffer + offset, sizeof(net_commitIndex));
	offset += sizeof(net_commitIndex);
	memcpy(&net_lastIndex, buffer + offset, sizeof(net_lastIndex));
	offset += sizeof(net_lastIndex);
	
	factoryId = ntohl(net_factoryId);
	commitIndex = ntohl(net_commitIndex);
	lastIndex = ntohl(net_lastIndex);
	mapOp.Unmarshal(buffer + offset);
}

bool RequestLog::IsValid() {
	return (factoryId != -1);
}

void RequestLog::Print() {
	std::cout << "factoryId " << factoryId << " ";
	std::cout << "commitIndex " << commitIndex << " ";
	std::cout << "lastIndex " << lastIndex << " ";
	mapOp.Print();
}

ResponseLog::ResponseLog() {
	factoryId = -1;
}

int ResponseLog::GetFactoryId() {
	return factoryId;
}

void ResponseLog::SetFactoryId(int facId) {
	factoryId = facId;
}

int ResponseLog::Size() {
	return sizeof(factoryId);
}

void ResponseLog::Marshal(char *buffer) {
	int net_factoryId = htonl(factoryId);
	int offset = 0;
	memcpy(buffer + offset, &net_factoryId, sizeof(net_factoryId));

}

void ResponseLog::Unmarshal(char *buffer) {
	int net_factoryId;
	int offset = 0;

	memcpy(&net_factoryId, buffer + offset, sizeof(net_factoryId));
	factoryId = ntohl(net_factoryId);
}

bool ResponseLog::IsValid() {
	return (factoryId == -1);
}

void ResponseLog::Print() {
  	std::cout << "factoryId " << factoryId << std::endl;
}