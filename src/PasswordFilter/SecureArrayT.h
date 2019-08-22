#pragma once
template <class T>
class SecureArrayT
{
protected:
	std::shared_ptr<T[]> internalptr;
	size_t size;

public:

	SecureArrayT(const T* item, const size_t size)
	{
		this->internalptr = std::shared_ptr<T[]>(item);
		this->size = size;
	}

	SecureArrayT(T* item, const size_t size)
	{
		this->internalptr = std::shared_ptr<T[]>(item);
		this->size = size;
	}

	SecureArrayT(const size_t size)
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

	size_t getSize() const
	{
		return this->size;
	}

	operator T*() const
	{
		return this->internalptr.get();
	}
};