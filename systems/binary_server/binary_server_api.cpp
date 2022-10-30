#include "main.h"
#include "system.h"
#include "binary_connection.h"
#include "binary_server_api.h"

void BinaryServerApi::request(BinaryConnection * conn, const Transaction & tr, const char * data, int size)
{
	switch(tr.function)
	{
	case FunctionCodes::ID:
		ID(conn, tr, data, size);
		break;
	case FunctionCodes::Ping:
		break;
	case FunctionCodes::LedGreen:
		break;
	case FunctionCodes::LedRed:
		break;
	case FunctionCodes::LedSevenSeg:
		LedSevenSeg(conn, tr, data, size);
		break;
	case FunctionCodes::DacSetOne:
		DacSetOne(conn, tr, data, size);
		break;
	case FunctionCodes::DacSetAll:
		break;
	case FunctionCodes::AdcGetAll:
		AdcGetAll(conn, tr, data, size);
		break;
	case FunctionCodes::ScanStart:
		ScanStart(conn, tr, data, size);
		break;
	case FunctionCodes::ScanStop:
		ScanStop(conn, tr, data, size);
		break;
	case FunctionCodes::ScanWriteSettings:
		ScanWriteSettings(conn, tr, data, size);
		break;
	case FunctionCodes::ServiceGetStatistics:
		break;
	case FunctionCodes::Reset:
		break;
	default:
		conn->sendResponseErrorWrongFunction(tr);
		break;
	}
}

void BinaryServerApi::ID(BinaryConnection * conn, const Transaction & tr, const char * data, int size)
{
	if (!conn->validateDataSize(tr, size, 0))
		return;
	String<> IdString = "HC 0.1";
	conn->sendResponseData(tr, IdString.c_str(), IdString.size());
}

void BinaryServerApi::LedSevenSeg(BinaryConnection * conn, const Transaction & tr, const char * data, int size)
{
	if (!conn->validateDataSize(tr, size, 4))
		return;
	int value = getInt(data, 0);
}

void BinaryServerApi::AdcGetAll(BinaryConnection * conn, const Transaction & tr, const char * data, int size)
{
}

void BinaryServerApi::DacSetOne(BinaryConnection * conn, const Transaction & tr, const char * data, int size)
{
	if (!conn->validateDataSize(tr, size, 8))
		return;
	int channel = getInt(data, 0);
	int value = getInt(data, 4);
	app.dac()->setChannel(channel, value);
}

void BinaryServerApi::ScanStart(BinaryConnection * conn, const Transaction & tr, const char * data, int size)
{
	if (!conn->validateDataSize(tr, size, 0))
		return;
}

void BinaryServerApi::ScanStop(BinaryConnection * conn, const Transaction & tr, const char * data, int size)
{
	if (!conn->validateDataSize(tr, size, 0))
		return;
}

void BinaryServerApi::ScanWriteSettings(BinaryConnection * conn, const Transaction & tr, const char * data, int size)
{
	if (size < 4)
	{
		conn->sendResponseErrorWrongParameters(tr);
		return;
	}

	int offset = getInt(data, 0);
}

int BinaryServerApi::getInt(const char * data, int offset)
{
	const int value = reinterpret_cast<const int *>(data + offset)[0];
	return value;
}

unsigned BinaryServerApi::getUInt(const char * data, int offset)
{
	const unsigned value = reinterpret_cast<const unsigned *>(data + offset)[0];
	return value;
}

unsigned short BinaryServerApi::getUShort(const char * data, int offset)
{
	const unsigned short value = reinterpret_cast<const unsigned short *>(data + offset)[0];
	return value;
}

double BinaryServerApi::getDouble(const char * data, int offset)
{
	const double value = reinterpret_cast<const double *>(data + offset)[0];
	return value;
}

