#include "Input.h"
#include<windows.h>
#include<cassert>

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")



void Input::Initialize(HINSTANCE hInstance, HWND hwnd)
{
	HRESULT result;

	result = DirectInput8Create
	(
		hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput, nullptr
	);
	assert(SUCCEEDED(result));
	//キーボードデバイスの生成
	//Microsoft::WRL::ComPtr < IDirectInputDevice8> keyboard = nullptr;
	result = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, nullptr);
	assert(SUCCEEDED(result));
	//入力データの形式セット
	result = keyboard->SetDataFormat(&c_dfDIKeyboard);
	assert(SUCCEEDED(result));
	//排他制御レベルのセット
	result = keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));
}

void Input::Update()
{
	//HRESULT result;


	//前回のキー入力を保存
	memcpy(keyPre, key, sizeof(key));

	//キーボード情報の取得開始
	keyboard->Acquire();
	//全キーの入力状態の取得
	keyboard->GetDeviceState(sizeof(key), key);
}

bool Input::PushKey(BYTE keyNumber)
{
	if (key[keyNumber])
	{
		return true;
	}
	else
	{
		return false;
	}

}

bool Input::TriggerKey(BYTE keyNumber)
{

	if (keyPre[keyNumber]!= keyPre[keyNumber])
	{
		return true;
	}
	else
	{
		return false;
	}


}
