#pragma once
class eventlog
{
public:
	void logw(WORD severity, DWORD eventID, int argCount, ...);
	void log(WORD severity, DWORD eventID, int argCount, ...);

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

