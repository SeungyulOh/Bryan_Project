#include "Input_Device.h"

using namespace Engine;
IMPLEMENT_SINGLETON(CInput_Device)

Engine::CInput_Device::CInput_Device(void)
: m_pInputSDK(NULL)
, m_pKeyBoard(NULL)
, m_pMouse(NULL)
{

}

Engine::CInput_Device::~CInput_Device()
{

}

HRESULT Engine::CInput_Device::Ready_InputDevice(HINSTANCE hInst, HWND hWnd)
{
	// 내키보드를 생성하기위한 사전객체를 생서앟낟.
	if(FAILED(DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInputSDK, NULL)))
		return E_FAIL;	

	// 내 컴퓨터에 연결되어있는 키보드객체를 생성하낟.
	if(FAILED(m_pInputSDK->CreateDevice(GUID_SysKeyboard, &m_pKeyBoard, NULL)))
		return E_FAIL;

	// 생성도니 키보드 객체의 대한 정보를 전달한다.
	m_pKeyBoard->SetDataFormat(&c_dfDIKeyboard);

	// 생성된 키보드의 동작 범위를 설정하낟.
	m_pKeyBoard->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

	// 생성된 키보드를 활성화시킨다.
	m_pKeyBoard->Acquire();

	// 내 컴퓨터에 연결되어있는 키보드객체를 생성하낟.
	if(FAILED(m_pInputSDK->CreateDevice(GUID_SysMouse, &m_pMouse, NULL)))
		return E_FAIL;

	// 생성도니 키보드 객체의 대한 정보를 전달한다.
	m_pMouse->SetDataFormat(&c_dfDIMouse);

	// 생성된 키보드의 동작 범위를 설정하낟.
	m_pMouse->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

	// 생성된 키보드를 활성화시킨다.
	m_pMouse->Acquire();

	return S_OK;
}

void Engine::CInput_Device::SetUp_InputState()
{
	m_pKeyBoard->GetDeviceState(256, m_byKeyState);
	m_pMouse->GetDeviceState(sizeof(m_MouseState), &m_MouseState);
}

void Engine::CInput_Device::Free(void)
{
	Engine::Safe_Release(m_pKeyBoard);
	Engine::Safe_Release(m_pMouse);
	Engine::Safe_Release(m_pInputSDK);
}

