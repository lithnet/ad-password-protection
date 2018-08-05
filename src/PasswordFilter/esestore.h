#pragma once

class esestore
{
public:
	static esestore& getInstance()
	{
		static esestore instance; 
		if (instance.closed)
		{
			static esestore instance;
		}

		return instance;
	}
private:
	bool closed;
	esestore();
	~esestore();

public:
	esestore(esestore const&) = delete;
	void operator=(esestore const&) = delete;
	bool IsHashInDb(BYTE* hash);
	void Close();
};

