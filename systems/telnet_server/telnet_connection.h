#ifndef _TELNET_CONNECTION_H_
#define _TELNET_CONNECTION_H_

#include "lwip/debug.h"
#include "lwip/stats.h"
#include "lwip/tcp.h"
#include "../utils/stream/stream.h"
#include "../utils/stringt.h"

class TelnetConnection
{
public:
	TelnetConnection()
	{
	}
	virtual ~TelnetConnection() {}

	enum tcp_echoserver_states
	{
	  ES_NONE = 0,
	  ES_ACCEPTED,
	  ES_RECEIVED,
	  ES_CLOSING
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
	  void parseInputBuffer();
	  void output(char * data, int size);
	  void sendOutput();
	  void printLine(String<> line);
	  void print(String<> line);

	  unsigned short parseFrame(char * frame, int size);

	  String<> getParameter(String<> line, int index);
private:
	  bool connected_ = false;
};

#endif // _TELNET_CONNECTION_H_
