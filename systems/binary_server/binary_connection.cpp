#include <binary_server/binary_connection.h>
#include "system.h"

void BinaryConnection::start(tcp_pcb *pcb)
{
	connected_ = true;
	pcb_ = pcb;
	inputBufferIndex_ = 0;
}

void BinaryConnection::stop()
{
	connected_ = false;
}

bool BinaryConnection::input(char * data, int size)
{
	int restOfBuffer = INPUT_BUFFER_SIZE - inputBufferIndex_;
	if (size <= restOfBuffer)
	{
		// Add data to input buffer
		memcpy(inputBuffer_ + inputBufferIndex_, data, size);
		inputBufferIndex_ += size;

		// processing frames
		parseInputBuffer();

		// send ack
		tcp_output(pcb_);

		return true;
	}
	return false; // Buffer overflow
}

void BinaryConnection::output(const char * data, int size)
{
	streamOut.enqueue(data, size);
}

void BinaryConnection::process()
{
	sendOutput();
}

void BinaryConnection::sendOutput()
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

int BinaryConnection::parseInputBuffer()
{
	int processedBytes = 0;
	char * currentFrame = inputBuffer_;
	int rest = inputBufferIndex_;

	while (rest > 4)
	{
		currentFrame = inputBuffer_ + processedBytes;
		int frameSize = ((int *)currentFrame)[0];
		if (frameSize >= 8)
		{
			if (rest >= frameSize)
			{
				Transaction tr;
				tr.transactionId = ((int *)currentFrame)[1];
				tr.function = ((int *)currentFrame)[2];
				processFrame(tr, currentFrame + 16, frameSize - 16);
				processedBytes += frameSize;
				rest -= frameSize;
			}
			else
			{
				break;
			}
		}
		else
		{
			processedBytes += frameSize;
			break;
		}
	}

	for (int i = 0; i < rest; i++)
		inputBuffer_[i] = inputBuffer_[processedBytes + i];
	inputBufferIndex_ -= processedBytes;

	return processedBytes;
}

void BinaryConnection::processFrame(const Transaction & tr, const char * data, int size)
{
	app.api()->request(this, tr, data, size);
}

bool BinaryConnection::validateDataSize(const Transaction & tr, int size, int needSize)
{
	if (size != needSize)
	{
		sendResponseErrorWrongParameters(tr);
		return false;
	}
	return true;
}

void BinaryConnection::sendResponse(const Transaction & tr, unsigned errorCode, const char * data, int size)
{

	int frameSize = 16 + size;
	output((char *)&frameSize, 4);
	output((char *)&tr.transactionId, 4);
	output((char *)&tr.function, 4);
	output((char *)&errorCode, 4);
	output(data, size);
}

void BinaryConnection::sendResponseOK(const Transaction & tr)
{
	sendResponse(tr, ErrorOK, nullptr, 0);
}

void BinaryConnection::sendResponseError(const Transaction & tr, unsigned errorCode)
{
	sendResponse(tr, errorCode, nullptr, 0);
}

void BinaryConnection::sendResponseErrorWrongParameters(const Transaction & tr)
{
	sendResponse(tr, ErrorWrongParameters, nullptr, 0);
}

void BinaryConnection::sendResponseErrorWrongFunction(const Transaction & tr)
{
	sendResponse(tr, ErrorWrongFunction, nullptr, 0);
}

void BinaryConnection::sendResponseData(const Transaction & tr, const char * data, int size)
{
	sendResponse(tr, ErrorOK, data, size);
}

void BinaryConnection::sendResponseInt(const Transaction & tr, int data)
{
	sendResponse(tr, ErrorOK, reinterpret_cast<char *>(&data), 4);
}

void BinaryConnection::sendResponseDouble(const Transaction & tr, double data)
{
	sendResponse(tr, ErrorOK, reinterpret_cast<char *>(&data), 8);
}
