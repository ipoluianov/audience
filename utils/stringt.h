#ifndef _STRING11_H_
#define _STRING11_H_

template<int MAX_SIZE>
class String;

template<int MAX_SIZE = 32>
class StringIterator
{
public:

	StringIterator(String<MAX_SIZE> * str, int pos)
	{
		str_ = str;
		pos_ = pos;
	}

	~StringIterator()
	{
	}

	int pos() { return pos_; }
	bool operator<(const StringIterator& a) const
	{
		if (pos_ < a.pos_)
			return true;
		return false;
	}

	const char operator*()
	{
		return str_->c_str()[pos_];
	}

	bool operator!=(const StringIterator& a) const
	{
		if (pos_ != a.pos_)
			return true;
		return false;
	}

	StringIterator & operator++()
	{
		pos_++;
		return *this;
	}

	StringIterator operator++(int)
	{
		pos_++;
		return *this;
	}


	bool operator==(const StringIterator& a) const
	{
		if (str_ == a.str_ && pos_ == a.pos_)
			return true;
		return false;
	}


	typedef int difference_type;


private:
	String<MAX_SIZE> * str_ = nullptr;
	int pos_ = 0;
};

template<int MAX_SIZE = 32>
class String
{
public:
	String()
	{
		clear();
	}

	String(const char * str)
	{
		clear();
		appendStr(str);
	}

	~String()
	{
	}

	void clear()
	{
		data_[0] = 0;
	}

	int size() const
	{
		int res = 0;
		while (data_[res] != 0 && res < MAX_SIZE)
			res++;
		return res;
	}

	void appendStr(const char * str)
	{
		int index = size();
		int indexInStr = 0;
		while (str[indexInStr] != 0 && index < MAX_SIZE)
		{
			data_[index] = str[indexInStr];
			index++;
			indexInStr++;
		}
		data_[index] = 0;
	}

	void appendChar(char ch)
	{
		int index = size();
		if (index < MAX_SIZE)
		{
			data_[index] = ch;
			index++;
		}
		data_[index] = 0;
	}

	const char * c_str() const { return data_; }
	char * str() { return data_; }

	StringIterator<MAX_SIZE> begin()
	{
		return StringIterator<MAX_SIZE>(this, 0);
	}

	StringIterator<MAX_SIZE> end()
	{
		return StringIterator<MAX_SIZE>(this, size());
	}

	String& operator=(const String &a)
	{
		clear();
		appendStr(a.c_str());
		return *this;
	}

	//static const int MAX_SIZE = 47;

	bool operator<(const String& a) const
	{
		if (size() < a.size())
		{
			return true;
		}

		if (size() > a.size())
		{
			return false;
		}

		for (int i = 0; i < size(); i++)
		{
			if (data_[i] != a.data_[i])
			{
				if (data_[i] < a.data_[i])
					return true;
				break;
			}
		}

		return false;
	}

	bool operator==(const String& a) const
	{
		if (size() != a.size())
			return false;
		for (int i = 0; i < size(); i++)
			if (data_[i] != a.data_[i])
				return false;
		return true;
	}

	bool operator==(const char * str) const
	{
		String a(str);
		if (size() != a.size())
			return false;
		for (int i = 0; i < size(); i++)
			if (data_[i] != a.data_[i])
				return false;
		return true;
	}

	bool operator!=(const String& a) const
	{
		return !(operator==(a));
	}

	bool operator!=(const char * str) const
	{
		return !(operator==(str));
	}

	operator bool()
	{
		return size() > 0;
	}

	operator char *()
	{
		return data_;
	}

private:
	char data_[MAX_SIZE + 1];
};

typedef String<16> String16;

#endif // _STRING_H_
