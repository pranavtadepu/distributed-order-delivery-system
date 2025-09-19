#ifndef __MESSAGES_H__
#define __MESSAGES_H__

#include <string>

class CustomerRequest {
private:
	int customer_id;// For laptop order: customer id who placed order. 
					// For read request: customer id to read from map.
	int order_number;// # of orders issued by this customer so far.
					// Record-read request does not count as an order. 
					// Set to -1 for read requests.
	int request_type;// Either 1 - regular laptop order request, or 
					// 2 - customer record read request

public:
	CustomerRequest();
	void operator = (const CustomerRequest &order) {
		customer_id = order.customer_id;
		order_number = order.order_number;
		request_type = order.request_type;
	}
	void SetOrder(int cid, int order_num, int type);
	int GetCustomerId();
	int GetOrderNumber();
	int GetRequestType();

	int Size();

	void Marshal(char *buffer);
	void Unmarshal(char *buffer);

	bool IsValid();

	void Print();
};

class LaptopInfo {
private:
	int customer_id;
	int order_number;
	int request_type;
	int engineer_id;
	int admin_id;

public:
	LaptopInfo();
	void operator = (const LaptopInfo &info) {
		customer_id = info.customer_id;
		order_number = info.order_number;
		request_type = info.request_type;
		engineer_id = info.engineer_id;
		admin_id = info.admin_id;
	}
	void SetInfo(int cid, int order_num, int type, int engid, int expid);
	void CopyOrder(CustomerRequest order);
	void SetEngineerId(int id);
	void SetExpertId(int id);

	int GetCustomerId();
	int GetOrderNumber();
	int GetRequestType();
	int GetEngineerId();
	int GetExpertId();

	int Size();

	void Marshal(char *buffer);
	void Unmarshal(char *buffer);

	bool IsValid();

	void Print();
};

class CustomerRecord {
private:
	int customer_id;
	int last_order;	

public:
	CustomerRecord();
	void operator = (const CustomerRecord &record) {
		customer_id = record.customer_id;
		last_order = record.last_order;
	}
	void SetCustomerRecord(int customer_id, int last_order);
	void SetCustomerId(int customer_id);
	void SetLastOrder(int last_order);

	int GetCustomerId();
	int GetLastOrder();

	int Size();

	void Marshal(char *buffer);
	void Unmarshal(char *buffer);

	bool IsValid();
	void Print();

	
};

class MapOp {
private: 
	int opcode; // operation code: 1 - update value int arg1;  
	int arg1; // customer_id to apply the operation int arg2;
	int arg2; // parameter for the operation

public:
	MapOp();
	void operator = (const MapOp &op ) {
		opcode = op.opcode;
		arg1 = op.arg1;
		arg2 = op.arg2;
	}

	int GetOpcode();
	int GetArg1();
	int GetArg2();

	void SetMapOp(int op, int arg1, int arg2);
	void SetOpcode(int op);
	void SetArg1(int arg1);
	void SetArg2(int arg2);
	void CopyMapOp(MapOp op);
	int Size();
	void Marshal(char *buffer);
	void Unmarshal(char *buffer);
	bool isValid();
	void Print();

};

class RequestLog {
private:
	int factoryId;
	int commitIndex;
	int lastIndex;
	MapOp mapOp;

public:
	RequestLog();
	void operator = (const RequestLog &req) {
		factoryId = req.factoryId;
		commitIndex = req.commitIndex;
		lastIndex = req.lastIndex;
		mapOp = req.mapOp;
	}

	int GetFactoryId();
	int GetCommitIndex();
	int GetLastIndex();
	MapOp GetMapOp();

	void SetFactoryId(int facId);
	void SetCommitIndex(int idx);
	void SetLastIndex(int lastIdx);
	void SetMapOp(MapOp mapOp);
	void CopyRequest(RequestLog req);
	int Size();
	void Marshal(char *buffer);
	void Unmarshal(char *buffer);
	bool IsValid();
	void Print();
};

class ResponseLog {
private:
	int factoryId;

public:
	ResponseLog();
	void operator = (const ResponseLog &res) {
		factoryId = res.factoryId;
	}

	int GetFactoryId();
	void SetFactoryId(int facId);
	int Size();
	void Marshal(char * buffer);
	void Unmarshal(char *buffer);
	bool IsValid();
	void Print();
};

#endif // #ifndef __MESSAGES_H__
