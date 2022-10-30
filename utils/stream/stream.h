#ifndef _STREAM_H_
#define _STREAM_H_

#include <string.h>
//#include <cmsis_gcc.h>

/*
Поток данных. Очередь с круговым буфером.
Служит для взаимодействия между двумя различными вычислительными потоками.
Один всегда только пишет - другой всегда только читает.
*/

template<typename T, int STREAM_SIZE>
class Stream
{
public:
	Stream()
	{
	}
	virtual ~Stream()
	{
	}

	// Чтение данных без копирования
	unsigned peekNonCopy(T ** outbuffer)
	{
		//register unsigned count = 0;
		register unsigned output_index = this->outputIndex_;
		register unsigned input_index = this->inputIndex_;
		register unsigned size;

		// Определение размера
		if (input_index == output_index)
			size = 0;
		if (input_index > output_index)
			size = input_index - output_index;
		if (input_index < output_index)
			size = this->size_ - output_index;

		*outbuffer = (T *)(this->buffer_ + this->outputIndex_);
		return size;
	}

	// Чтение данных с копированием
	unsigned peek(T * outbuffer, unsigned maxsize)
	{
		register unsigned count = 0;
		register unsigned output_index = this->outputIndex_;
		register unsigned input_index = this->inputIndex_;
		register unsigned size;

		// Определение размера
		if (input_index == output_index)
			size = 0;
		if (input_index > output_index)
			size = input_index - output_index;
		if (input_index < output_index)
			size = this->size - output_index + input_index;

		if (size > maxsize)
			size = maxsize;

		if (size < 1)
			return 0;

		if (input_index > output_index)
		{
			memcpy(outbuffer, (char *)(this->buffer_ + output_index), sizeof(T) * size); // Буфер без разрыва
			return size;
		}

		if (input_index < output_index) // Буфер с разрывом
		{
			register unsigned first_block = this->size_ - output_index;
			register unsigned second_block;
			if (first_block > size)
				first_block = size;
			memcpy(outbuffer, (char *)(this->buffer_ + output_index), sizeof(T)*first_block);
			second_block = size - first_block;
			if (second_block > 0)
				memcpy(outbuffer + first_block, (char *)this->buffer_, sizeof(T)*second_block);
			return size;
		}

		return 0;
	}

	// Освобождение буфера
	void dequeue(unsigned size)
	{
		register unsigned output_index = this->outputIndex_;
		//register unsigned input_index = inputIndex_;

		if (output_index + size >= this->size_)
		{
			size -= (this->size_ - output_index);
			output_index = 0;
		}
		output_index += size;
		this->outputIndex_ = output_index;
	}

	// Запись
	void enqueue(const T * data, unsigned size)
	{
		register unsigned count = 0;
		register unsigned output_index = this->outputIndex_;
		register unsigned input_index = this->inputIndex_;

		while(count < size)
		{
			register unsigned next_input_index = input_index + 1;
			if (next_input_index >= this->size_) // Переход в начало буфера
				next_input_index = 0;
			if (next_input_index == output_index) // Переполнение
				break;
			this->buffer_[input_index] = data[count];
			count++;
			input_index = next_input_index;
		}

		this->inputIndex_ = input_index;
	}

	// Получение резмера свободной области
	unsigned getSpace()
	{
		register unsigned output_index = this->outputIndex_;
		register unsigned input_index = this->inputIndex_;

		if (input_index == output_index) // Буфер пуст
			return this->size_ - 1;
		if (input_index > output_index) // Буфер без разрыва
			return this->size_ - input_index + output_index - 1;
		if (input_index < output_index) // Буфер с разрывом
			return output_index - input_index - 1;
		return 0;
	}

	// Очистка данных
	void reset()
	{
		this->inputIndex_ = 0;
		this->outputIndex_ = 0;
	}

	// Текущий размер содержащихся данных
	unsigned datasize()
	{
		return this->size_ - getSpace() - 1;
	}

private:
	//inline static void lock() { __disable_irq(); }
	//inline static void unlock() { __disable_irq(); }

	volatile T buffer_[STREAM_SIZE];
	volatile unsigned size_ = STREAM_SIZE;
	
	volatile unsigned inputIndex_; // Куда писать следующий байт
	volatile unsigned outputIndex_; // Откуда
	
	volatile unsigned id_; // Идентификатор для отслеживания и отладки
};

#endif // _STREAM_H_
