#pragma once
template <class T>
class SecureArrayT
{
private:
	std::shared_ptr<T[]> internalptr;
	int size;

public:
	SecureArrayT(int size)
	{
		internalptr = std::make_unique<T[]>(size);
		this->size = size;
	}

	SecureArrayT(const SecureArrayT& p)
	{
		internalptr = p.internalptr;
		size = p.size;
	}

	~SecureArrayT()
	{
		if (internalptr && internalptr.use_count() <= 1)
		{
			SecureZeroMemory(internalptr.get(), size);
		}
	}

	T* get()
	{
		return internalptr.get();
	}

	operator T*() const 
	{
		return internalptr.get();
	}
};