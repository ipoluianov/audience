#include "telnet_connection.h"
#include "system.h"

void TelnetConnection::start(tcp_pcb *pcb)
{
	connected_ = true;
	pcb_ = pcb;
	inputBufferIndex_ = 0;

	printLine("AudioUstrojstvo v0.1");
	print("AudioUstrojstvo:/# ");
	//streamIn.reset();
	//streamOut.reset();
}

void TelnetConnection::stop()
{
	connected_ = false;
}

bool TelnetConnection::input(char * data, int size)
{
	int restOfBuffer = INPUT_BUFFER_SIZE - inputBufferIndex_;
	if (size <= restOfBuffer)
	{
		//output("123", 3);
		memcpy(inputBuffer_ + inputBufferIndex_, data, size);
		inputBufferIndex_ += size;
		parseInputBuffer();
		tcp_output(pcb_);
		return true;
	}
	return false; // Buffer overflow
}

void TelnetConnection::output(char * data, int size)
{
	streamOut.enqueue(data, size);
}

void TelnetConnection::process()
{
	sendOutput();
}

void TelnetConnection::sendOutput()
{
	//unsigned short sendBufferSize;
	unsigned realDataSize;
	char * bufferPointer;
	//int sent = 0;

	while(connected_)
	{
		realDataSize = streamOut.peekNonCopy(&bufferPointer);
		if (realDataSize > 4096) // ~MTU
				realDataSize = 4096;

		if (realDataSize < 1)
			break;

		err_t wr_err = ERR_OK;
		wr_err = tcp_write(pcb_, bufferPointer, realDataSize, 1);
		if (wr_err == ERR_OK)
		{
			streamOut.dequeue(realDataSize);
			tcp_output(pcb_);
		}
		else
		{
			// Flush TCP buffers
			for (int i = 0; i < 10; i++)
				tcp_output(pcb_);
			break;
		}
	}
	return;

}

void TelnetConnection::parseInputBuffer()
{
	while (inputBufferIndex_ > 0)
	{
		unsigned processedBytes = parseFrame(inputBuffer_, inputBufferIndex_);
		if (processedBytes == 0)
			break;

		unsigned rest = inputBufferIndex_ - processedBytes;

		for (unsigned i = 0; i < rest; i++)
			inputBuffer_[i] = inputBuffer_[processedBytes + i];
		inputBufferIndex_ = rest;
	}
}

unsigned short TelnetConnection::parseFrame(char * frame, int size)
{
	String<> line;

	if (frame[0] == 0xFF)
	{
		return 3;
	}


	int processedSize = 0;
	bool found = false;
	for (int i = 0; i < size; i++)
	{
		if (frame[i] >= 32 && frame[i] < 127)
			line.appendChar(frame[i]);
		if (frame[i] == 13 || frame[i] == 10)
		{
			found = true;
			processedSize = i + 1;
			break;
		}
	}

	if (!found)
		return 0;

	String<> cmd = getParameter(line, 0);
	if (cmd.size() == 0)
	{
		//print("hController:/# ");
		return processedSize;
	}

	bool processed = false;

	if (cmd == "dac")
	{
		String<> number = getParameter(line, 1);
		String<> value = getParameter(line, 2);
		int num = atoi(number);
		int v = atoi(value);
		app.dac()->setChannel(num, v);
		processed = true;
	}

	if (cmd == "help" || cmd == "h")
	{
		printLine("");
		printLine("AutioUstrojstvo - help");
		printLine("");
		printLine("led <0-99>     - display number to led-display");
		printLine("dac <ch> <val> - set DAC channel. Channel is 0 - 8.");
		printLine("");
		processed = true;
	}


	if (!processed)
	{
		print("Unknown command: ");
		printLine(cmd.c_str());
	}

	print("AudioUstrojstvo:/# ");
	return processedSize;
}

String<> TelnetConnection::getParameter(String<> line, int index)
{
	int currentIndex = 0;

	String<> result;
	for (int i = 0; i < line.size(); i++)
	{
		if (line[i] == ' ')
		{
			currentIndex++;
		}
		else
		{
			if (currentIndex == index)
			{
				result.appendChar(line[i]);
			}
		}
	}
	return result;
}

void TelnetConnection::printLine(String<> str)
{
	output(str.str(), str.size());
	char enter[2] = { '\r', '\n' };
	output(enter, 2);
}

void TelnetConnection::print(String<> str)
{
	output(str.str(), str.size());
}
