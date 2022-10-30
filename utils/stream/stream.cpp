//#include "stream.h"


//circular buffer shared by two threads
//access to the buffer data is controlled by register variables input_index, output_index
//buffer size is defined on creation (stream_init)

//used in RS-485 (in-out), single SIM mode (data points received from regsys)
//receiving data from RegSys during experiments
//logging system
//TCP server in-out buffers

/*void memcpy_(char * dest, char * src, unsigned size)
{
	unsigned i = 0;
	for (i = 0; i < size; i++)
	{
		dest[i] = src[i];
	}
}*/
