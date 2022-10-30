#ifndef _BINARY_SERVER_API_H_
#define _BINARY_SERVER_API_H_

class BinaryConnection;

enum FunctionCodes
{
    ID = 0x00000000,
    Ping = 0x00000001,

    // LED
    LedGreen = 0x00010001,
    LedRed = 0x00010002,
    LedSevenSeg = 0x00010003,

    // DAC
    DacSetOne = 0x00020001,
    DacSetAll = 0x00020002,

    // ADC
    AdcGetAll = 0x00030001,

    // Service
    ServiceGetStatistics = 0x00040001,

	// Scan
	ScanStart = 0x00080001,
	ScanStop = 0x00080002,

	ScanWriteSettings = 0x00080101,

    // Reset
    Reset = 0x7FFFDEAD
};

class BinaryServerApi
{
public:
	void request(BinaryConnection * conn, const Transaction & tr, const char * data, int size);

	void ID(BinaryConnection * conn, const Transaction & tr, const char * data, int size);

	void LedSevenSeg(BinaryConnection * conn, const Transaction & tr, const char * data, int size);

	void AdcGetAll(BinaryConnection * conn, const Transaction & tr, const char * data, int size);

	void DacSetOne(BinaryConnection * conn, const Transaction & tr, const char * data, int size);

	void ScanStart(BinaryConnection * conn, const Transaction & tr, const char * data, int size);
	void ScanStop(BinaryConnection * conn, const Transaction & tr, const char * data, int size);
	void ScanWriteSettings(BinaryConnection * conn, const Transaction & tr, const char * data, int size);

	static bool getBool(const char * data, int offset);
	static int getInt(const char * data, int offset);
	static unsigned getUInt(const char * data, int offset);
	static unsigned short getUShort(const char * data, int offset);
	static double getDouble(const char * data, int offset);
private:
};

#endif // _BINARY_SERVER_API_H_
