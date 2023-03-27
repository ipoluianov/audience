#include "display.h"
#include "main.h"

void Display::init()
{

	/*LL_GPIO_ResetOutputPin(DISPLAY_RS_GPIO_Port, DISPLAY_RS_Pin);
	HAL_Delay(1);
	LL_GPIO_ResetOutputPin(DISPLAY_E_GPIO_Port, DISPLAY_E_Pin);
	HAL_Delay(1);
	GPIOD->ODR = GPIOD->ODR | (((0x00100000 >> 4) & 0x0F) << 2);
	HAL_Delay(1);
	LL_GPIO_SetOutputPin(DISPLAY_E_GPIO_Port, DISPLAY_E_Pin);
	HAL_Delay(1);
	LL_GPIO_ResetOutputPin(DISPLAY_E_GPIO_Port, DISPLAY_E_Pin);
	HAL_Delay(1);

	sendCommand(0x28);
	sendCommand(0x0f);
	sendCommand(0x07);
	sendCommand(0x01);

	sendData('0');
	sendData('U');
	sendData(230);*/
}

void Display::process()
{

}

void Display::setOutput(char data)
{
	/*LL_GPIO_ResetOutputPin(DISPLAY_E_GPIO_Port, DISPLAY_E_Pin);
	HAL_Delay(1);
	GPIOD->ODR = GPIOD->ODR | (((data >> 4) & 0x0F) << 2);
	HAL_Delay(1);
	LL_GPIO_SetOutputPin(DISPLAY_E_GPIO_Port, DISPLAY_E_Pin);
	HAL_Delay(1);
	LL_GPIO_ResetOutputPin(DISPLAY_E_GPIO_Port, DISPLAY_E_Pin);
	HAL_Delay(1);
	GPIOD->ODR = GPIOD->ODR | (((data >> 0) & 0x0F) << 2);
	HAL_Delay(1);
	LL_GPIO_SetOutputPin(DISPLAY_E_GPIO_Port, DISPLAY_E_Pin);
	HAL_Delay(1);
	LL_GPIO_ResetOutputPin(DISPLAY_E_GPIO_Port, DISPLAY_E_Pin);
	HAL_Delay(1);*/
}

void Display::sendCommand(char ch)
{
	/*HAL_Delay(1);
	LL_GPIO_ResetOutputPin(DISPLAY_RS_GPIO_Port, DISPLAY_RS_Pin);
	HAL_Delay(1);
	setOutput(ch);*/
}

void Display::sendData(char ch)
{
	/*HAL_Delay(1);
	LL_GPIO_SetOutputPin(DISPLAY_RS_GPIO_Port, DISPLAY_RS_Pin);
	HAL_Delay(1);
	setOutput(ch);*/
}
