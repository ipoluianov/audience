#include "main.h"
#include "dac.h"
#include "system.h"

//extern SPI_HandleTypeDef hspi1;
//extern SPI_HandleTypeDef hspi2;

//extern TIM_HandleTypeDef htim1;

struct Stat
{
	int bufferLoading = 0;
	int overloadBuffer = 0;
	int countOf16 = 0;
	int countOf24 = 0;
	int countOf32 = 0;
};

Stat stat;


// 0001 0000

#define CS0_ON GPIOF->ODR |= 0x0010
#define CS0_OFF GPIOF->ODR &= 0xFFEF
#define CS1_ON GPIOF->ODR |= 0x0008
#define CS1_OFF GPIOF->ODR &= 0xFFF7

//unsigned char dac1_dma_buffer1[3] __attribute__((section(".TxDecripSection"))) = { 0x00, 0x00, 0x0 };
//unsigned char dac1_dma_buffer2[3] __attribute__((section(".TxDecripSection"))) = { 0x00, 0x00, 0x0 };



unsigned char dac1_dma_buffer_tx[3] __attribute__((section(".TxDecripSection"))) = { 0x00, 0x00, 0x0 };
unsigned char dac1_dma_buffer_rx[3] __attribute__((section(".TxDecripSection"))) = { 0x00, 0x00, 0x0 };
unsigned char dac2_dma_buffer_tx[3] __attribute__((section(".TxDecripSection"))) = { 0x00, 0x00, 0x0 };
unsigned char dac2_dma_buffer_rx[3] __attribute__((section(".TxDecripSection"))) = { 0x00, 0x00, 0x0 };
//unsigned char dac1_dma_buffer_tx[3]  = { 0x00, 0x00, 0x0 };
//unsigned char dac1_dma_buffer_rx[3]  = { 0x00, 0x00, 0x0 };

//unsigned char dac1_dma_buffer[1*10] __attribute__((section(".TxDecripSection")));


int dac_scan_i = 0;
int dac_program[5] = { 65535, 65535, 65535, 65535, 65535 };



void setDMA();

void Dac::init()
{

	LL_SPI_Enable(SPI1);

	LL_SPI_EnableDMAReq_TX(SPI1);
	LL_SPI_EnableDMAReq_RX(SPI1);

	LL_DMA_EnableIT_TC(DMA1, LL_DMA_STREAM_0);
	LL_DMA_EnableIT_TC(DMA1, LL_DMA_STREAM_1);

	LL_DMA_SetDataLength(DMA1, LL_DMA_STREAM_0, 3);
	LL_DMA_SetDataLength(DMA1, LL_DMA_STREAM_1, 3);

    LL_DMA_ConfigAddresses(DMA1, LL_DMA_STREAM_0, (uint32_t)&(SPI1->RXDR), (uint32_t)&dac1_dma_buffer_rx, LL_DMA_GetDataTransferDirection(DMA1, LL_DMA_STREAM_0));
    LL_DMA_ConfigAddresses(DMA1, LL_DMA_STREAM_1, (uint32_t)dac1_dma_buffer_tx, (uint32_t)&(SPI1->TXDR), LL_DMA_GetDataTransferDirection(DMA1, LL_DMA_STREAM_1));

	LL_SPI_Enable(SPI2);

	LL_SPI_EnableDMAReq_TX(SPI2);
	LL_SPI_EnableDMAReq_RX(SPI2);

	LL_DMA_EnableIT_TC(DMA1, LL_DMA_STREAM_2);
	LL_DMA_EnableIT_TC(DMA1, LL_DMA_STREAM_3);

	LL_DMA_SetDataLength(DMA1, LL_DMA_STREAM_2, 3);
	LL_DMA_SetDataLength(DMA1, LL_DMA_STREAM_3, 3);

    LL_DMA_ConfigAddresses(DMA1, LL_DMA_STREAM_2, (uint32_t)&(SPI2->RXDR), (uint32_t)&dac2_dma_buffer_rx, LL_DMA_GetDataTransferDirection(DMA1, LL_DMA_STREAM_2));
    LL_DMA_ConfigAddresses(DMA1, LL_DMA_STREAM_3, (uint32_t)dac2_dma_buffer_tx, (uint32_t)&(SPI2->TXDR), LL_DMA_GetDataTransferDirection(DMA1, LL_DMA_STREAM_3));

	LL_TIM_EnableIT_UPDATE(TIM1);
	LL_TIM_EnableCounter(TIM1);
}

void Dac::process()
{
	//generateSin();
	//generateMiander();
}

void Dac::addFrame(char * data, int bits, int dataSize)
{
	stat.bufferLoading++;

	int count = 0;


	if (bits == 16)
	{
		stat.countOf16++;
		count = dataSize / 4;
	}
	else
	{
		if (bits == 24)
		{
			stat.countOf24++;
			count = dataSize / 6;
		}
		else
			if (bits == 32)
			{
				stat.countOf32++;
				count = dataSize / 8;
			}
	}

	register unsigned outputIndex = this->outputIndex_;
	register unsigned inputIndex = this->inputIndex_;

	register unsigned nextInputIndex = inputIndex + 1;
	if (nextInputIndex >= FRAMES_COUNT) // Переход в начало буфера
		nextInputIndex = 0;
	if (nextInputIndex == outputIndex) // Переполнение
	{
		stat.overloadBuffer++;
		return;
	}

	Frame * frame;
	frame = &frames_[nextInputIndex];

	if (bits == 16)
	{
		signed short * convertedDataA = (signed short *) data;
		signed short * convertedDataB = (signed short *) (data + 2);
		for (int i = 0; i < count; i++)
		{
			int valueA = convertedDataA[i * 2];
			int valueB = convertedDataB[i * 2];
			frame->channelA[i] = (valueA + 32767) * 16;
			frame->channelB[i] = (valueB + 32767) * 16;
		}
	}

	if (bits == 32)
	{
		signed int * convertedDataA = (signed int *) data;
		signed int * convertedDataB = (signed int *) (data + 4);

		for (int i = 0; i < count; i++)
		{
			int valueA = convertedDataA[i * 4];
			if (valueA & 0x800000)
				valueA = (valueA & 0x7FFFFF) - 0x800000;
			int valueB = convertedDataB[i * 4];
			if (valueB & 0x800000)
				valueB = (valueB & 0x7FFFFF) - 0x800000;

			frame->channelA[i] = (valueA + 2048 * 1024) / 16;
			frame->channelB[i] = 0;
		}
	}

	if (bits == 24)
	{
		unsigned char * convertedData = (unsigned char *) data;
		for (int i = 0; i < count * 6; i += 6)
		{
			int value = convertedData[i + 2];
			value <<= 8;
			value |= convertedData[i + 1];
			value <<= 8;
			value += convertedData[i];
			if (value & 0x800000)
				value = (value & 0x7FFFFF) - 0x800000;
			//value = value >> 4;
			frame->channelA[i / 6] = (value + 512 * 1024) / 16;
			frame->channelB[i / 6] = 0;

			/*value = convertedData[i + 2 + 3];
			value <<= 8;
			value |= convertedData[i + 1 + 3];
			value <<= 8;
			value += convertedData[i + 3];
			if (value & 0x800000)
				value = 0x800000 | (~(value & 0x7FFFFF));
			value = value >> 4;
			frame->channelB[i / 6] = value;*/
		}
	}
	frame->size = count;

	inputIndex_ = nextInputIndex;
}

void Dac::sendBytes1DMA(unsigned char * bytes)
{
	HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_RESET);
	__DSB();
	//HAL_SPI_Transmit_DMA(&hspi1, (uint8_t*) bytes, 3);
}

/*void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
	if (hspi == &hspi1)
	{
		app.dac()->busy_ = false;
	}

	if (hspi == &hspi1)
	{
		CS0_ON;
		//HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_SET);
		LDAC0_OFF;
		LDAC0_ON;
	}
}*/


void Dac::sendBytes1(unsigned char * bytes)
{
	unsigned char rvcBuffer[2];
	//HAL_Delay(1);
	HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_RESET);
	//HAL_Delay(1);
	//HAL_SPI_TransmitReceive(&hspi1, bytes, rvcBuffer, 3, 20);
	//HAL_Delay(1);
	HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_SET);
	//HAL_Delay(1);
}

void Dac::setChannel(int index, unsigned int code)
{
	if (index == 8)
	{
		dac1_dma_buffer_tx[0] = (code >> 12) & 0xFF;
		dac1_dma_buffer_tx[1] = (code >> 4) & 0xFF;
		dac1_dma_buffer_tx[2] = (code << 4) & 0xF0;
		//sendBytes1DMA(dac1_dma_buffer);
	}
}


int currentCodeA = 0;
int currentCodeB = 0;

void setDirect()
{
	//HAL_GPIO_WritePin(ADC8_CS_GPIO_Port, ADC8_CS_Pin, GPIO_PIN_RESET);
	LL_GPIO_ResetOutputPin(SPI1_CS_GPIO_Port, SPI1_CS_Pin);

	__DSB();

	for (int i = 0; i < 4; i++)
	{
		//while(!LL_SPI_IsActiveFlag_TXE(SPI1));
		LL_SPI_TransmitData8(SPI1, dac1_dma_buffer_tx[i]);
		LL_SPI_StartMasterTransfer(SPI1);
		for (int k = 0; k < 5; k++)
		{
			asm("nop");
		}
		//while(!LL_SPI_IsActiveFlag_RXNE(SPI1));
		//dac1_dma_buffer_rx[i] = LL_SPI_ReceiveData8(SPI1);
	}

	//HAL_SPI_TransmitReceive(&hspi5, adc8_dma_buffer_tx, adc8_dma_buffer_rx, 4, 10);
	LL_GPIO_SetOutputPin(SPI1_CS_GPIO_Port, SPI1_CS_Pin);
	//HAL_GPIO_WritePin(ADC8_CS_GPIO_Port, ADC8_CS_Pin, GPIO_PIN_SET);
	for (int k = 0; k < 5; k++)
	{
		asm("nop");
	}
	LDAC0_OFF;
	for (int k = 0; k < 5; k++)
	{
		asm("nop");
	}
	LDAC0_ON;
	for (int k = 0; k < 5; k++)
	{
		asm("nop");
	}

}

void setDMA()
{
}

void Dac::timer1()
{
	/*rateCounter_++;
	if (rateCounter_ >= rateDiv_)
		rateCounter_ = 0;
	if (rateCounter_ != 0)
		return;*/

	if (currentFrame_== nullptr) // from silence
	{
		int inputIndex = inputIndex_;
		if (outputIndex_ == inputIndex) // No frames available
			return;

		// Start playing new frame
		currentFrame_ = &frames_[outputIndex_];
		playingPoint_ = 0;
	}

	while(true) // Find frame/point for playing
	{
		if (playingPoint_ >= currentFrame_->size)
		{
			int inputIndex = inputIndex_;
			int outputIndex = outputIndex_;
			if (outputIndex == inputIndex) // No frames available
			{
				currentFrame_ = nullptr; // To silent mode
				return;
			}

			stat.bufferLoading--;
			outputIndex++;
			if (outputIndex >= FRAMES_COUNT)
				outputIndex = 0;
			outputIndex_ = outputIndex;

			// Start playing new frame
			currentFrame_ = &frames_[outputIndex];
			playingPoint_ = 0;
		}
		else
		{
			break;
		}
	}

	currentCodeA = currentFrame_->channelA[playingPoint_];
	currentCodeB = currentFrame_->channelB[playingPoint_];
	playingPoint_ += 1;

	dac1_dma_buffer_tx[0] = (currentCodeA >> 12) & 0xFF;
	dac1_dma_buffer_tx[1] = (currentCodeA >> 4) & 0xFF;
	dac1_dma_buffer_tx[2] = (currentCodeA << 4) & 0xF0;
	dac2_dma_buffer_tx[0] = (currentCodeB >> 12) & 0xFF;
	dac2_dma_buffer_tx[1] = (currentCodeB >> 4) & 0xFF;
	dac2_dma_buffer_tx[2] = (currentCodeB << 4) & 0xF0;
	CS0_OFF;
	CS1_OFF;
	__DSB();

	//LL_GPIO_ResetOutputPin(SPI1_CS_GPIO_Port, SPI1_CS_Pin);

	DMA1_Stream0->NDTR = 3;
	DMA1_Stream1->NDTR = 3;

	DMA1_Stream0->CR |= 0x00000001;
	DMA1_Stream1->CR |= 0x00000001;
	LL_SPI_StartMasterTransfer(SPI1);

	//LL_GPIO_ResetOutputPin(SPI2_CS_GPIO_Port, SPI2_CS_Pin);

	DMA1_Stream2->NDTR = 3;
	DMA1_Stream3->NDTR = 3;

	DMA1_Stream2->CR |= 0x00000001;
	DMA1_Stream3->CR |= 0x00000001;
	LL_SPI_StartMasterTransfer(SPI2);
}

void Dac::dmaIT1()
{
	asm("nop");
	asm("nop");
	asm("nop");

	LL_GPIO_SetOutputPin(SPI1_CS_GPIO_Port, SPI1_CS_Pin);
}

void Dac::dmaIT2()
{
	asm("nop");
	asm("nop");
	asm("nop");

	LL_GPIO_SetOutputPin(SPI2_CS_GPIO_Port, SPI2_CS_Pin);
}

void Dac::generateSin()
{
	int index = 0;
	for (double x = 0; x < 6.28; x += 0.01)
	{
		double v = sin(x) * 32000;
		genBuffer_[index] = v;
		genBuffer_[index + 1] = v;
		index += 2;
	}

	addFrame((char *)genBuffer_, 16, index / 2);
}

void Dac::generateMiander()
{
	int index = 0;
	for (int x = 0; x < 1000; x += 1)
	{
		int v = -30000;
		if (x < 500)
			v = 30000;
		genBuffer_[index] = v;
		genBuffer_[index + 1] = v;
		index += 2;
	}

	addFrame((char *)genBuffer_, 16, index / 2);
}
