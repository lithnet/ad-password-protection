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
	
	static void init();
	static void writeToFileLog(std::wstring message);
	static void writeToFileLog(std::string message);

	eventlog(eventlog const&) = delete;
	void operator=(eventlog const&) = delete;

private:
	static HANDLE getHandle();
	static HANDLE hlog;
	eventlog();
	~eventlog();
};

