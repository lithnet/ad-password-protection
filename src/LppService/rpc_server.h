#pragma once
#define SECURITY_WIN32

class rpc_server
{
private:
	bool initialized;
	void MakeSpn() const;
	static std::wstring GenerateSpn();
	static void TryRegisterSpn(const std::wstring& serviceClass);
	static void RegisterSpn(const std::wstring& serviceClass);
	static RPC_STATUS __stdcall RpcServerIfCallback(IN void *ifx, IN void *context);

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