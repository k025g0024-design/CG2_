#pragma once
#include<Windows.h>
#include <cstdint>

class WinApp
{
public:

	static LRESULT CALLBACK Windowproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);


	void Initialize();

	void Update();

	void Finalize();

	bool ProcessMessage();


	static const int32_t kClientWidth = 1280;
	static const int32_t kClientHeight = 720;
	//getter
	HWND GetHwnd()const { return hwnd; }
	HINSTANCE GetHInstance() const { return wc.hInstance; }
private:

	HWND hwnd = nullptr;
	WNDCLASS wc{};




};

