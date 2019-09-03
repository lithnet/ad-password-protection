#pragma once
#define SECURITY_WIN32

class rpc_server
{
private:
	bool initialized;
	void MakeSpn() const;

public:
	void InitializeRpcServer();
	void Start(bool wait) const;
	void Stop() const;
	void WaitForEnd() const;

	rpc_server()
	{
		this->initialized = false;
	}
};