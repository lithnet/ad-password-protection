#pragma once
template <class T>
class SecureArrayT
{
protected:
	std::shared_ptr<T[]> internalptr;
	int size;

public:

	SecureArrayT(T* item, const int size)
	{
		this->internalptr = std::shared_ptr<T[]>(item);
		this->size = size;
	}

	SecureArrayT(const int size)
	{
		this->internalptr = std::make_unique<T[]>(size);
		this->size = size;
	}

	SecureArrayT(const SecureArrayT& p)
	{
		this->internalptr = p.internalptr;
		this->size = p.size;
	}

	~SecureArrayT()
	{
		if (this->internalptr && this->internalptr.use_count() <= 1)
		{
			SecureZeroMemory(this->internalptr.get(), this->size);
		}
	}

	T* get() const
	{
		return this->internalptr.get();
	}

	int getSize() const
	{
		return this->size;
	}

	operator T*() const
	{
		return this->internalptr.get();
	}
};