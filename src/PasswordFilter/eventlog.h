#pragma once
class eventlog
{
public:
	void logw(const WORD &severity, const DWORD &eventID, const int argCount, ...);
	void log(const WORD &severity, const DWORD &eventID, const int argCount, ...);

	static eventlog& getInstance()
	{
		static eventlog instance;
		return instance;
	}
	eventlog(eventlog const&) = delete;
	void operator=(eventlog const&) = delete;

private:
	
	eventlog();
	~eventlog();
};

