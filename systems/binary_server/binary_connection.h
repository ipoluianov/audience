#ifndef _BINARY_CONNECTION_H_
#define _BINARY_CONNECTION_H_

#include "lwip/debug.h"
#include "lwip/stats.h"
#include "lwip/tcp.h"
#include "../utils/stream/stream.h"
#include "../utils/stringt.h"

struct Transaction
{
	unsigned transactionId;
	unsigned function;
	unsigned errorCode;
};


class BinaryConnection
{
public:
	BinaryConnection()
	{
	}
	virtual ~BinaryConnection() {}

	enum tcp_echoserver_states
	{
	  ES_NONE = 0,
	  ES_ACCEPTED,
	  ES_RECEIVED,
	  ES_CLOSING
	};

	enum ErrorCode
	{
		ErrorOK,
		ErrorWrongFunction,
		ErrorWrongParameters,
		ErrorResourceIsBusy
	};

	tcp_echoserver_states state = ES_NONE; // current connection state

	static const int INPUT_BUFFER_SIZE = 1024;


	  tcp_pcb * pcb_ = nullptr;    // pointer on the current tcp_pcb
	  char inputBuffer_[INPUT_BUFFER_SIZE];
	  int inputBufferIndex_ = 0;

	  //Stream<256> streamIn;
	  Stream<char, 256> streamOut;

	  void start(tcp_pcb *pcb);
	  void stop();

	  void process();

	  bool input(char * data, int size);
	  int parseInputBuffer();
	  void output(const char * data, int size);
	  void sendOutput();

	  void processFrame(const Transaction & tr, const char * data, int size);

	  bool validateDataSize(const Transaction & tr, int size, int needSize);

	  void sendResponse(const Transaction & tr, unsigned errorCode, const char * data, int size);
	  void sendResponseOK(const Transaction & tr);
	  void sendResponseError(const Transaction & tr, unsigned errorCode);
	  void sendResponseErrorWrongParameters(const Transaction & tr);
	  void sendResponseErrorWrongFunction(const Transaction & tr);
	  void sendResponseData(const Transaction & tr, const char * data, int size);

	  void sendResponseInt(const Transaction & tr, int data);
	  void sendResponseDouble(const Transaction & tr, double data);


private:
	  bool connected_ = false;
};

#endif // _BINARY_CONNECTION_H_
