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

	bool PushKey(BYTE keyNumber);
	bool TriggerKey(BYTE keyNumber);

private:

	BYTE key[256] = {};
	BYTE keyPre[256] = {};

	//キーボードデバイスの生成をヘッダーに移動
ComPtr< IDirectInputDevice8 > keyboard ;
Microsoft::WRL::ComPtr <IDirectInput8> directInput = nullptr;

};

