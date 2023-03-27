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

// 0100 0000 0000

#define CS_1_ON GPIOG->ODR |= 0x0400
#define CS_1_OFF GPIOG->ODR &= 0xFBFF
#define CS_2_ON GPIOB->ODR |= 0x0200
#define CS_2_OFF GPIOB->ODR &= 0xFDFF

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

	/*LL_SPI_Enable(SPI1);

	LL_SPI_EnableDMAReq_TX(SPI1);
	LL_SPI_EnableDMAReq_RX(SPI1);

	LL_DMA_EnableIT_TC(DMA1, LL_DMA_STREAM_0);
	LL_DMA_EnableIT_TC(DMA1, LL_DMA_STREAM_1);

	LL_DMA_SetDataLength(DMA1, LL_DMA_STREAM_0, 3);
	LL_DMA_SetDataLength(DMA1, LL_DMA_STREAM_1, 3);

    LL_DMA_ConfigAddresses(DMA1, LL_DMA_STREAM_0, (uint32_t)&(SPI1->RXDR), (uint32_t)&dac1_dma_buffer_rx, LL_DMA_GetDataTransferDirection(DMA1, LL_DMA_STREAM_0));
    LL_DMA_ConfigAddresses(DMA1, LL_DMA_STREAM_1, (uint32_t)dac1_dma_buffer_tx, (uint32_t)&(SPI1->TXDR), LL_DMA_GetDataTransferDirection(DMA1, LL_DMA_STREAM_1));*/

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

extern SPI_HandleTypeDef hspi1;

void Dac::process()
{
	/*DMA1_Stream0->NDTR = 3;
	DMA1_Stream1->NDTR = 3;

	DMA1_Stream0->CR |= 0x00000001;
	DMA1_Stream1->CR |= 0x00000001;
	LL_SPI_StartMasterTransfer(SPI1);*/

	//SPI1->TXDR = 0x11;
	//SPI1->CR1 = SPI1->CR1 | 0b00000100;

/*	LL_GPIO_ResetOutputPin(CS1_GPIO_Port, CS1_Pin);
	LL_SPI_TransmitData8(SPI1, 0xAA);
	LL_SPI_StartMasterTransfer(SPI1);
	LL_GPIO_SetOutputPin(CS1_GPIO_Port, CS1_Pin);

	LL_SPI_TransmitData8(SPI2, 0xAA);
	LL_SPI_StartMasterTransfer(SPI2);*/

	LL_GPIO_ResetOutputPin(CS1_GPIO_Port, CS1_Pin);
	volatile HAL_StatusTypeDef err = HAL_SPI_Transmit(&hspi1, dac1_dma_buffer_tx, 3, 1000);
	LL_GPIO_SetOutputPin(CS1_GPIO_Port, CS1_Pin);


	HAL_Delay(1);

	//generateSin();
	//generateMiander();
}

void Dac::addData(char * data, int size)
{
	short * dataAsShorts = (short *) data;
	int shortsCount = size / 4;

	__disable_irq();
	for (int i = 0; i < shortsCount; i += 2)
	{
		int newIndex = inputIndex_;
		newIndex++;
		if (newIndex == bufferSizeSamples_) {
			newIndex = 0;
		}
		if (newIndex == outputIndex_)
			break;
		channelA_[inputIndex_] = dataAsShorts[i];
		channelB_[inputIndex_] = dataAsShorts[i + 1];
		inputIndex_ = newIndex;
	}
	__enable_irq();
}

int Dac::currentDataSize()
{
	int result;

	__disable_irq();

	if (inputIndex_ == outputIndex_)
		result = 0;

	if (inputIndex_ < outputIndex_) {

	}

	__enable_irq();

	return result;
}

void Dac::sendBytes1DMA(unsigned char * bytes)
{
	//HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_RESET);
	//__DSB();
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
	/*unsigned char rvcBuffer[2];
	//HAL_Delay(1);
	HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_RESET);
	//HAL_Delay(1);
	//HAL_SPI_TransmitReceive(&hspi1, bytes, rvcBuffer, 3, 20);
	//HAL_Delay(1);
	HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_SET);*/
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
	//LL_GPIO_ResetOutputPin(SPI1_CS_GPIO_Port, SPI1_CS_Pin);

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
	//LL_GPIO_SetOutputPin(SPI1_CS_GPIO_Port, SPI1_CS_Pin);
	//HAL_GPIO_WritePin(ADC8_CS_GPIO_Port, ADC8_CS_Pin, GPIO_PIN_SET);
	for (int k = 0; k < 5; k++)
	{
		asm("nop");
	}
	/*LDAC0_OFF;
	for (int k = 0; k < 5; k++)
	{
		asm("nop");
	}
	LDAC0_ON;
	for (int k = 0; k < 5; k++)
	{
		asm("nop");
	}*/

}

void setDMA()
{
}

void Dac::timer1()
{
	return;

	if (outputIndex_ == inputIndex_)
		return; // No data to play

	int currentCodeA = channelA_[outputIndex_];
	int currentCodeB = channelB_[outputIndex_];

	__disable_irq();
	outputIndex_++;
	if (outputIndex_ == bufferSizeSamples_)
		outputIndex_ = 0;
	__enable_irq();

	dac1_dma_buffer_tx[0] = (currentCodeA >> 12) & 0xFF;
	dac1_dma_buffer_tx[1] = (currentCodeA >> 4) & 0xFF;
	dac1_dma_buffer_tx[2] = (currentCodeA << 4) & 0xF0;
	dac2_dma_buffer_tx[0] = (currentCodeB >> 12) & 0xFF;
	dac2_dma_buffer_tx[1] = (currentCodeB >> 4) & 0xFF;
	dac2_dma_buffer_tx[2] = (currentCodeB << 4) & 0xF0;
	//CS_1_OFF;
	//CS_2_OFF;
	LL_GPIO_ResetOutputPin(CS1_GPIO_Port, CS1_Pin);
	LL_GPIO_ResetOutputPin(CS2_GPIO_Port, CS2_Pin);
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

	LL_GPIO_SetOutputPin(CS1_GPIO_Port, CS1_Pin);
}

void Dac::dmaIT2()
{
	asm("nop");
	asm("nop");
	asm("nop");

	LL_GPIO_SetOutputPin(CS2_GPIO_Port, CS2_Pin);
}

