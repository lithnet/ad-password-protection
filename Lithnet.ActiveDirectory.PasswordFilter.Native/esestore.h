#pragma once

class esestore
{
public:
	static esestore& getInstance()
	{
		static esestore instance; 
		return instance;
	}
private:
	esestore();
	~esestore();

public:
	esestore(esestore const&) = delete;
	void operator=(esestore const&) = delete;
	bool IsHashInDb(BYTE* hash);
};

