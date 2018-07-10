#pragma once
class SystemClass
{
public:
	SystemClass();
	SystemClass(const SystemClass&);
	~SystemClass();

	bool Initialze();
	void Shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	bool Frame();
	void InitializeWindows(int&, int&);
	void ShutdownWindows();

private:
	LPCSTR applicationName;
		
};

static LRESULT CALLBACK WindProc(HWND, UINT, WPARAM, LPARAM);
static SystemClass* ApplicationHandle = 0;