#pragma once

#include <wrl.h>
#define DIRECTINPUT_VERSION 0x0800
#include<dinput.h>

class Input
{
public:
	void Initialize(HINSTANCE hInstance,HWND hwnd);
	void Update();

	template <class T > using ComPtr = Microsoft::WRL::ComPtr<T>;

	

private:

	//キーボードデバイスの生成をヘッダーに移動
ComPtr< IDirectInputDevice8 > keyboard ;

};

