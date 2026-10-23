#pragma once
#include<Windows.h>
class WinApp
{
public:

	static LRESULT CALLBACK Windowproc(HWND hwnd, UINT msg, WPARAM wparm, LPARAM lparam);
		 

	void Initialize();

	void Update();

private:


};

